#pragma once

#include <string_view>

#include "../containers/buffered_list.hpp"
#include "../containers/buffered_string.hpp"
#include "../defines.hpp"
#include "../util/assert.hpp"
#include "../util/concepts.hpp"
#include "../util/primitives.hpp"

namespace rsl
{
#if !defined(RSL_ERR_MESSAGE_MAX_LENGTH)
	#define RSL_ERR_MESSAGE_MAX_LENGTH 512
#endif

#if !defined(RSL_WARN_MAX_COUNT)
	#define RSL_WARN_MAX_COUNT 16
#endif

#if !defined(RSL_ERR_UNDERLYING)
	#define RSL_ERR_UNDERLYING int64
#endif

#if !defined(RSL_ERR_ID_UNDERLYING)
	#define RSL_ERR_ID_UNDERLYING uint8
#endif

#if defined(RSL_ERR_MAX_COUNT)
	RYTHE_COMPILER_ERROR("RSL_ERR_MAX_COUNT cannot be user defined, use RSL_ERR_ID_UNDERLYING instead")
#endif

	using errid = RSL_ERR_ID_UNDERLYING;

#define RSL_ERR_MAX_COUNT std::numeric_limits<errid>::max()

	constexpr errid invalid_err_id = RSL_ERR_MAX_COUNT;

	using errc = RSL_ERR_UNDERLYING;
	constexpr errc no_error_code = 0;

	template <typename ERRC>
	concept error_code = std::is_enum_v<ERRC> && rsl::is_same_v<std::underlying_type_t<ERRC>, errc>;

	enum struct error_severity : uint8
	{
		warning,
		error,
		fatal
	};

	struct error_type
	{
		errc code;
		buffered_string<RSL_ERR_MESSAGE_MAX_LENGTH> message;
		error_severity severity;
		errid errorBlockStart;

		[[nodiscard]] [[rythe_always_inline]] constexpr operator errc() const noexcept { return code; }

		[[nodiscard]] [[rythe_always_inline]] constexpr bool operator==(const error_type& other) const noexcept
		{
			return code == other.code;
		}
	};

	constexpr static error_type success = error_type{
		.code = no_error_code, .message = {}, .severity = static_cast<error_severity>(-1), .errorBlockStart = 0
	};

	using error_list = buffered_list<error_type, RSL_ERR_MAX_COUNT>;
	using error_view = typename error_list::view_type;

	struct error_handler
	{
		virtual void handle_assert(
			std::string_view expression, std::string_view file, size_type line, std::string_view message, bool soft
		)
		{
			rsl::asserts::internal::default_assert_handler(expression, file, line, message, soft);
		}

		virtual void handle_error(const error_type& error, bool assertError);
	};

	struct error_context
	{
		thread_local static error_list errors;
		thread_local static errid currentError;
		thread_local static error_handler* errorHandlerOverride;
		thread_local static bool assertOnError;
		static error_handler defaultErrorHandler;
	};

	void enable_assert_on_error(bool enabled = true) noexcept;
	void disable_assert_on_error() noexcept;
	bool assert_on_error_enabled() noexcept;

	struct scoped_assert_on_error final
	{
		bool wasEnabled;
		scoped_assert_on_error(bool enabled = true);
		~scoped_assert_on_error();
	};

	void set_error_handler(error_handler* errorHandler) noexcept;
	error_handler* get_error_handler() noexcept;

	struct scoped_error_handler final
	{
		error_handler* previousErrorHandler;
		scoped_error_handler(error_handler* errorHandler);
		~scoped_error_handler();
	};

	struct error_signal
	{
	};

	struct result_base;

	namespace internal
	{
		errid& get_errid(result_base&) noexcept;
	}

	struct result_base
	{
	protected:
		errid m_errid = invalid_err_id;

		constexpr result_base() noexcept = default;
		result_base(error_signal) noexcept
			: m_errid(error_context::currentError)
		{
		}

		friend errid& internal::get_errid(result_base&) noexcept;

	public:
		[[nodiscard]] [[rythe_always_inline]] bool is_okay() const noexcept { return get_error() == success; }

		[[nodiscard]] [[rythe_always_inline]] const error_type& get_error() const noexcept
		{
			return m_errid == invalid_err_id ? success : error_context::errors[m_errid];
		}

		[[nodiscard]] [[rythe_always_inline]] error_view get_errors() const noexcept
		{
			if (m_errid == invalid_err_id)
			{
				return error_view{};
			}

			errid blockStart = error_context::errors[m_errid].errorBlockStart;
			return error_view{&error_context::errors[blockStart], static_cast<size_type>((m_errid - blockStart) + 1)};
		}

		[[rythe_always_inline]] void resolve() noexcept
		{
			rsl_hard_assert_msg(!is_okay(), "Tried to resolve a valid result without error.");
			rsl_hard_assert_msg(
				m_errid == error_context::currentError,
				"Errors not resolved in order. Earlier result remains unresolved."
			);

			auto& error = error_context::errors[error_context::currentError];
			error_context::errors.pop_back((error_context::currentError - error.errorBlockStart) + 1);
			error_context::currentError = static_cast<errid>(error_context::errors.size() - 1);
			m_errid = invalid_err_id;
		}

		[[nodiscard]] [[rythe_always_inline]] errid id() const noexcept { return m_errid; }

		errc report_errors() noexcept
		{
			error_handler* errorHandler = get_error_handler();

			for (auto& error : get_errors())
			{
				errorHandler->handle_error(error, assert_on_error_enabled());
			}

			errc errorCode = no_error_code;
			if (m_errid != invalid_err_id)
			{
				errorCode = get_error().code;
				resolve();
			}

			return errorCode;
		}

		virtual ~result_base()
		{
#if RYTHE_VALIDATION_LEVEL != RYTHE_VALIDATION_LEVEL_NONE
			report_errors();
#endif
		}
	};

	template <typename...>
	struct result;

	template <typename T>
	struct result<T> final : public result_base
	{
	private:
		union
		{
			T m_value;
			byte m_dummy;
		};

	public:
		using result_type = remove_cvr_t<T>;

		[[rythe_always_inline]] result(error_signal) noexcept
			: result_base(error_signal{}),
			  m_dummy(0)
		{
		}

		template <typename... Args>
			requires(rsl::constructible_from<T, Args...>)
		[[rythe_always_inline]] constexpr result(Args&&... args) noexcept(rsl::is_nothrow_constructible_v<T, Args...>)
			: result_base(),
			  m_value(rsl::forward<Args>(args)...)
		{
		}

		[[nodiscard]] [[rythe_always_inline]] bool carries_value() const noexcept
		{
			for (auto& error : get_errors())
			{
				if (error.severity != error_severity::warning)
				{
					return false;
				}
			}
			return true;
		}

		[[nodiscard]] [[rythe_always_inline]] result_type& value() noexcept
		{
			rsl_hard_assert_msg(is_okay(), "Tried to get value of result with unresolved error.");
			return m_value;
		}

		[[nodiscard]] [[rythe_always_inline]] const result_type& value() const noexcept
		{
			rsl_hard_assert_msg(is_okay(), "Tried to get value of result with unresolved error.");
			return m_value;
		}
	};

	template <>
	struct result<void> final : public result_base
	{
		[[rythe_always_inline]] result(error_signal) noexcept
			: result_base(error_signal{})
		{
		}

		[[rythe_always_inline]] constexpr result() noexcept
			: result_base()
		{
		}
	};

	namespace internal
	{
		template <error_code ErrorType>
		[[rythe_always_inline]] constexpr void append_error(
			errid errorBlockStart, ErrorType errorType, std::string_view message, error_severity severity
		) noexcept
		{
			rsl_hard_assert_msg(
				error_context::errors.size() != RSL_ERR_MAX_COUNT,
				"Max error count overflow, consider changing RSL_ERR_ID_UNDERLYING to a larger type."
			);

			error_context::currentError = static_cast<errid>(error_context::errors.size());
			error_context::errors.emplace_back(error_type{
				.code = static_cast<errc>(errorType),
				.message = message,
				.severity = severity,
				.errorBlockStart = errorBlockStart,
			});
		}
	} // namespace internal

	template <error_code ErrorType>
	[[nodiscard]] [[rythe_always_inline]] constexpr auto
	make_error(ErrorType errorType, std::string_view message, error_severity severity = error_severity::error) noexcept
	{
		internal::append_error(static_cast<errid>(error_context::errors.size()), errorType, message, severity);
		return error_signal{};
	}

	template <error_code ErrorType>
	[[nodiscard]] [[rythe_always_inline]] constexpr auto
	make_warning(ErrorType errorType, std::string_view message) noexcept
	{
		return make_error(errorType, message, error_severity::warning);
	}

	template <error_code ErrorType>
	[[nodiscard]] [[rythe_always_inline]] constexpr auto
	make_fatal(ErrorType errorType, std::string_view message) noexcept
	{
		return make_error(errorType, message, error_severity::fatal);
	}

	template <typename T, error_code ErrorType>
	[[rythe_always_inline]] constexpr void append_error(
		result<T>& result, ErrorType errorType, std::string_view message,
		error_severity severity = error_severity::error
	) noexcept
	{
		errid blockStart = 0;
		if (error_context::currentError != invalid_err_id)
		{
			blockStart = error_context::errors[error_context::currentError].errorBlockStart;
		}
		else if (result.id() != invalid_err_id)
		{
			blockStart = result.id();
		}

		internal::append_error(blockStart, errorType, message, severity);
		internal::get_errid(result) = error_context::currentError;
	}

	template <typename T, error_code ErrorType>
	[[rythe_always_inline]] constexpr void
	append_warning(result<T>& result, ErrorType errorType, std::string_view message) noexcept
	{
		append_error(result, errorType, message, error_severity::warning);
	}

	template <typename T, error_code ErrorType>
	[[rythe_always_inline]] constexpr void
	append_fatal(result<T>& result, ErrorType errorType, std::string_view message) noexcept
	{
		append_error(result, errorType, message, error_severity::fatal);
	}

	template <typename T, typename... Args>
	[[nodiscard]] [[rythe_always_inline]] constexpr result<T> make_result(Args&&... args)
		noexcept(rsl::is_nothrow_constructible_v<result<T>, Args...>)
	{
		return result<T>(rsl::forward<Args>(args)...);
	}

	inline static result<void> okay = result<void>();

	struct unspecified_error
	{
		enum struct unspecified_error_code : errc
		{
			unspecified = -1,
		};

		template <typename T>
		operator result<T>()
		{
			return make_error(unspecified_error_code::unspecified, "Unspecified error");
		}
	};

	inline static unspecified_error error = unspecified_error{};
} // namespace rsl

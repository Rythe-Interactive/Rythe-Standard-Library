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
	RYTHE_COMPILER_WARNING("RSL_ERR_MAX_COUNT cannot be user defined, use RSL_ERR_ID_UNDERLYING instead")
#endif

	using errid = RSL_ERR_ID_UNDERLYING;

#define RSL_ERR_MAX_COUNT std::numeric_limits<errid>::max()

	constexpr errid invalid_err_id = RSL_ERR_MAX_COUNT;

	using errc = RSL_ERR_UNDERLYING;

	template <typename ERRC>
	concept error_code = std::is_enum_v<ERRC> && std::is_same_v<std::underlying_type_t<ERRC>, errc>;

	struct error_type
	{
		errc code;
		buffered_string<RSL_ERR_MESSAGE_MAX_LENGTH> message;

		[[nodiscard]] constexpr operator errc() const noexcept { return code; }

		constexpr bool operator==(const error_type& other) const noexcept { return code == other.code; }
	};

	constexpr static error_type success = error_type{.code = 0, .message = {}};

	using warning_type = error_type;
	using warning_list = buffered_list<warning_type, RSL_WARN_MAX_COUNT>;
	using warning_view = warning_list::view_type;

	struct error_info
	{
		error_type error;
		warning_list warnings;
		size_type warningCursor;
	};

	struct error_context
	{
		thread_local static buffered_list<error_info, RSL_ERR_MAX_COUNT> errors;
		thread_local static errid currentError;
	};

	struct error_signal
	{
	};

	struct result_base
	{
	protected:
		errid m_errid = invalid_err_id;

		constexpr result_base() noexcept = default;
		result_base(error_signal) noexcept
			: m_errid(error_context::currentError)
		{
			error_context::currentError = invalid_err_id;
		}

	public:
		[[nodiscard]] const error_type& getError() const noexcept
		{
			return error_context::errors[m_errid].error;
		}

		[[nodiscard]] warning_view getWarnings() const noexcept
		{
			return m_errid == invalid_err_id ? warning_view{} : error_context::errors[m_errid].warnings.view();
		}

		virtual ~result_base()
		{
			rsl_assert_msg(getError() == success, getError().message);

#if RYTHE_VALIDATION_LEVEL >= RYTHE_WARNING_VALIDATION_LEVEL
			for (auto& warning : getWarnings())
			{
				rsl_soft_assert_msg(warning == success, warning.message);
			}
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
		result(error_signal) noexcept
			: result_base(error_signal{})
		{
		}

		template <typename... Args>
			requires(std::constructible_from<T, Args...>)
		constexpr result(Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
			: result_base(),
			  m_value(std::forward<Args>(args)...)
		{
		}
	};

	template <error_code ErrorType>
	constexpr auto make_error(ErrorType&& errorType, std::string_view message) noexcept
	{
		if (error_context::currentError == invalid_err_id)
		{
			error_context::currentError = error_context::errors.size();
			error_context::errors.emplace_back();
		}

		auto& error = error_context::errors[error_context::currentError].error;
		error.code = static_cast<errc>(errorType);
		error.message = message;

		return error_signal{};
	}

	template <typename T, typename... Args>
	constexpr result<T> make_result(Args&&... args) noexcept(std::is_nothrow_constructible_v<result<T>, Args...>)
	{
		return result<T>(std::forward<Args>(args)...);
	}
} // namespace rsl

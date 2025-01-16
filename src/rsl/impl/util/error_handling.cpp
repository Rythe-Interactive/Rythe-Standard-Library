#include "error_handling.hpp"
#include "../../logging"

namespace rsl
{
	void error_handler::handle_error(const error_type& err, bool assertError)
	{
		switch (err.severity)
		{
			case error_severity::warning:
			{
				if (assertError)
				{
					rsl_soft_assert_msg_consistent(false, err.message);
				}
				else
				{
					log::warn("{}: {}", err.code, err.message);
				}
				break;
			}
			case error_severity::error:
			{
				if (assertError)
				{
					rsl_hard_assert_msg(false, err.message);
				}
				else
				{
					log::error("{}: {}", err.code, err.message);
				}
				break;
			}
			case error_severity::fatal:
			{
				if (assertError)
				{
					rsl_hard_assert_msg(false, err.message);
				}
				else
				{
					log::fatal("{}: {}", err.code, err.message);
				}
				break;
			}
		}
	}

	thread_local error_list error_context::errors;
	thread_local errid error_context::currentError = invalid_err_id;
	thread_local error_handler* error_context::errorHandlerOverride = nullptr;
	thread_local bool error_context::assertOnError = true;
	error_handler error_context::defaultErrorHandler;

	namespace internal
	{
		errid& get_errid(result_base& r) noexcept
		{
			return r.m_errid;
		}
	} // namespace internal

	void enable_assert_on_error(bool enabled) noexcept
	{
		error_context::assertOnError = enabled;
	}

	void disable_assert_on_error() noexcept
	{
		enable_assert_on_error(false);
	}

	bool assert_on_error_enabled() noexcept
	{
		return error_context::assertOnError;
	}

	scoped_assert_on_error::scoped_assert_on_error(bool enabled)
		: wasEnabled(assert_on_error_enabled())
	{
		enable_assert_on_error(enabled);
	}

	scoped_assert_on_error::~scoped_assert_on_error()
	{
		enable_assert_on_error(wasEnabled);
	}

	namespace
	{
		void error_handler_based_assert_handler(
			std::string_view expression, std::string_view file, size_type line, std::string_view message, bool soft
		)
		{
			get_error_handler()->handle_assert(expression, file, line, message, soft);
		}
	} // namespace

	void set_error_handler(error_handler* errorHandler) noexcept
	{
		asserts::assert_handler = &error_handler_based_assert_handler;
		error_context::errorHandlerOverride = errorHandler;
	}

	error_handler* get_error_handler() noexcept
	{
		return error_context::errorHandlerOverride ? error_context::errorHandlerOverride
												   : &error_context::defaultErrorHandler;
	}

	scoped_error_handler::scoped_error_handler(error_handler* errorHandler)
		: previousErrorHandler(get_error_handler())
	{
		set_error_handler(errorHandler);
	}

	scoped_error_handler::~scoped_error_handler()
	{
		set_error_handler(previousErrorHandler);
	}

} // namespace rsl

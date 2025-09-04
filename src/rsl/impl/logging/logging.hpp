#pragma once
#include "logger.hpp"
#include "sink.hpp"

namespace rsl
{
	struct logging_context
	{
		dynamic_string logFile = "logs/rythe.log"_ds;
	    log::logger* logger = nullptr;
		log::logger* undecoratedLogger = nullptr;
	};
    DECLARE_SINGLETON(logging_context)

	namespace log
    {
        template <typename... Args>
        [[rythe_always_inline]] void println(severity s, format_string format, Args&&... args) noexcept
        {
            get_logging_context().logger->log(s, format, rsl::forward<Args>(args)...);
        }

        template <typename... Args>
        [[rythe_always_inline]] void undecoratedln(severity s, format_string format, Args&&... args) noexcept
        {
            get_logging_context().undecoratedLogger->log(s, format, rsl::forward<Args>(args)...);
        }

        [[rythe_always_inline]] static void filter(const severity level)
        {
            logging_context& context = get_logging_context();
            context.logger->filter(level);
            context.undecoratedLogger->filter(level);
        }

        template <typename... Args>
        [[rythe_always_inline]] void undecorated_trace(format_string format, Args&&... args)
        {
            undecoratedln(severity::trace, format, rsl::forward<Args>(args)...);
        }

        template <typename... Args>
        [[rythe_always_inline]] void undecorated_debug(format_string format, Args&&... args)
        {
            undecoratedln(severity::debug, format, rsl::forward<Args>(args)...);
        }

        template <typename... Args>
        [[rythe_always_inline]] void undecorated_info(format_string format, Args&&... args)
        {
            undecoratedln(severity::info, format, rsl::forward<Args>(args)...);
        }

        template <typename... Args>
        [[rythe_always_inline]] void undecorated_warn(format_string format, Args&&... args)
        {
            undecoratedln(severity::warn, format, rsl::forward<Args>(args)...);
        }

        template <typename... Args>
        [[rythe_always_inline]] void undecorated_error(format_string format, Args&&... args)
        {
            undecoratedln(severity::error, format, rsl::forward<Args>(args)...);
        }

        template <typename... Args>
        [[rythe_always_inline]] void undecorated_fatal(format_string format, Args&&... args)
        {
            undecoratedln(severity::fatal, format, rsl::forward<Args>(args)...);
        }

        template <typename... Args>
        [[rythe_always_inline]] void trace(format_string format, Args&&... args)
        {
            println(severity::trace, format, rsl::forward<Args>(args)...);
        }

        template <typename... Args>
        [[rythe_always_inline]] void debug(format_string format, Args&&... args)
        {
            println(severity::debug, format, rsl::forward<Args>(args)...);
        }

        template <typename... Args>
        [[rythe_always_inline]] void info(format_string format, Args&&... args)
        {
            println(severity::info, format, rsl::forward<Args>(args)...);
        }

        template <typename... Args>
        [[rythe_always_inline]] void warn(format_string format, Args&&... args)
        {
            println(severity::warn, format, rsl::forward<Args>(args)...);
        }

        template <typename... Args>
        [[rythe_always_inline]] void error(format_string format, Args&&... args)
        {
            println(severity::error, format, rsl::forward<Args>(args)...);
        }

        template <typename... Args>
        [[rythe_always_inline]] void fatal(format_string format, Args&&... args)
        {
            println(severity::fatal, format, rsl::forward<Args>(args)...);
        }
    } // namespace log
} // namespace rsl

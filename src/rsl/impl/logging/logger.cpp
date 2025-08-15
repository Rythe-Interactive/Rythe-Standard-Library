#include "logger.hpp"

#include "time/time_point.hpp"
#include "time/stopwatch.hpp"

#include "severity.hpp"

#include "logging.hpp"
#include "containers/string.hpp"
#include "threading/current_thread.hpp"

#include "message.hpp"

namespace rsl::log
{
	void basic_logger::log(const log::severity s, const format_string format, const fmt::format_args args) noexcept
	{
		const log::message logMessage
		{
			.loggerName = m_name,
			.threadId = current_thread::get_id(),
			.timestamp = time::main_clock.current_point(),
			.sourceLocation = format.srcLoc,
			.severity = s,
			.msg = format.str,
			.formatArgs = args,
		};

		log(logMessage);
	}

	void basic_logger::flush()
	{
		for (auto* sink : m_sinks)
		{
			sink->flush();
		}
	}

	void synchronous_logger::log(const log::message& message)
	{
		const bool logEnabled = message.severity >= m_severity && message.severity != log::severity::off;
		if (!logEnabled)
		{
			return;
		}

		fmt::memory_buffer buf;
		fmt::vformat_to(fmt::appender(buf), fmt::string_view(message.msg.data(), message.msg.size()),
						message.formatArgs);

		for (auto* sink : m_sinks)
		{
			sink->log(message);
		}

		if (message.severity >= m_flushSeverity)
		{
			flush();
		}
	}
} // namespace rsl::log

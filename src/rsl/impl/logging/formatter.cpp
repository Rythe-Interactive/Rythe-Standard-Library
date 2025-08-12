#include "formatter.hpp"

#include "../platform/platform.hpp"
#include "message.hpp"

namespace rsl::log
{
	void genesis_flag_formatter::format([[maybe_unused]] const message& msg, const time::point32 time,
	                                    fmt::memory_buffer& dest)
	{
		fmt::format_to(fmt::appender(dest), "{}", (time - time::genesis).seconds());
	}

	void logger_name_formatter::format(const message& msg, [[maybe_unused]] const time::point32 time,
										fmt::memory_buffer& dest)
	{
		dest.append(msg.loggerName.data(), msg.loggerName.data() + msg.loggerName.size());
	}

	void thread_name_formatter_flag::format(const message& msg, [[maybe_unused]] const time::point32 time,
										fmt::memory_buffer& dest)
	{
		string_view threadName = platform::get_thread_name(msg.threadId);
		dest.append(threadName.data(), threadName.data() + threadName.size());
	}
} // namespace rsl

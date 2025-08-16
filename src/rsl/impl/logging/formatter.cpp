#include "formatter.hpp"

#include "../platform/platform.hpp"
#include "../time/stopwatch.hpp"
#include "message.hpp"

namespace rsl::log
{
	namespace
	{
		class forwarding_formatter final : public flag_formatter
		{
		public:
			forwarding_formatter(string_view section) noexcept : patternSection(section) {}

			string_view patternSection;

			void format([[maybe_unused]] const message& msg, [[maybe_unused]] const time::point32 time,
			            fmt::memory_buffer& dest) override
			{
				dest.append(patternSection.data(), patternSection.data() + patternSection.size());
			}
		};
	}

	void pattern_formatter::format(const message& msg, fmt::memory_buffer& dest)
	{
		const time::point32 time = time::main_clock.current_point();

		for (auto&)

	}

	void pattern_formatter::add_flag(const char flag, temporary_object<flag_formatter>&& formatter)
	{
		m_flags.emplace(flag, formatter);
	}

	void pattern_formatter::set_pattern(string_view pattern)
	{
		m_pattern = dynamic_string::from_view(pattern);
	}

	void pattern_formatter::compile_pattern()
	{
		m_formatters.clear();

		string_view forwardingSection;
		char const * const end = m_pattern.end();
		for (const char* character = m_pattern.begin(); character != end; ++character)
		{
			if (*character == '%')
			{
				if (!forwardingSection.empty())
				{
					m_formatters.push_back(
						unique_object<forwarding_formatter>::create_in_place(forwardingSection)
						);
					forwardingSection.reset();
				}

				if (character != end)
				{
					flag_formatter_ptr* result = m_flags.find(*character);
					rsl_assert_invalid_pattern(result);

				}
				else
				{
					break;
				}
			}
			else // chars not following the % sign should be displayed as is
			{
				if (forwardingSection.empty())
				{
					forwardingSection.set_data(character, 1ull);
				}
				else
				{
					forwardingSection.set_data(forwardingSection.data(), forwardingSection.size() + 1ull);
				}
			}
		}

		if (!forwardingSection.empty())
		{
			m_formatters.push_back(
				unique_object<forwarding_formatter>::create_in_place(forwardingSection)
				);
			forwardingSection.reset();
		}

	}

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

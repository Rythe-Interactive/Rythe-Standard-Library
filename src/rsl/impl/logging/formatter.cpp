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

            void format(
                    [[maybe_unused]] const message& msg,
                    [[maybe_unused]] const time::point32 time,
                    fmt::memory_buffer& dest
                    ) override
            {
                dest.append(patternSection.data(), patternSection.data() + patternSection.size());
            }
        };
    }

    void pattern_formatter::format(const message& msg, fmt::memory_buffer& dest)
    {
        const time::point32 time = time::main_clock.current_point();

        for (auto& formatter : m_formatters)
        {
            formatter->format(msg, time, dest);
        }
    }

    void pattern_formatter::compile_pattern(array_view<temporary_object<flag_formatter>> formatters)
    {
        m_formatters.clear();

        size_type formatterIndex = 0ull;

        string_view forwardingSection;
        char const* const end = m_pattern.end();
        for (const char* character = m_pattern.begin(); character != end; ++character)
        {
            if (*character == '{')
            {
                if (!forwardingSection.empty())
                {
                    m_formatters.emplace_back(
                            unique_object<forwarding_formatter>::create_in_place(forwardingSection)
                            );
                    forwardingSection.reset();
                }

                if (character != end)
                {
                    unique_object<flag_formatter>& formatter = m_formatters.emplace_back(rsl::move(formatters[formatterIndex++]));
                    if (*character != '}')
                    {
                        const char* start = character;
                        while (*character != '}')
                        {
                            ++character;
                        }
                        formatter->set_flag_options({ start, static_cast<size_type>(character - start) });
                    }
                }
                else
                {
                    break;
                }
            }
            else // chars not following the { sign should be displayed as is
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
            m_formatters.emplace_back(
                    unique_object<forwarding_formatter>::create_in_place(forwardingSection)
                    );
            forwardingSection.reset();
        }

    }

    void genesis_flag_formatter::format(
            [[maybe_unused]] const message& msg,
            const time::point32 time,
            fmt::memory_buffer& dest
            )
    {
        fmt::format_to(fmt::appender(dest), "{}", (time - time::genesis).seconds());
    }

    void logger_name_formatter::format(
            const message& msg,
            [[maybe_unused]] const time::point32 time,
            fmt::memory_buffer& dest
            )
    {
        dest.append(msg.loggerName.data(), msg.loggerName.data() + msg.loggerName.size());
    }

    void thread_name_formatter_flag::format(
            const message& msg,
            [[maybe_unused]] const time::point32 time,
            fmt::memory_buffer& dest
            )
    {
        string_view threadName = platform::get_thread_name(msg.threadId);
        dest.append(threadName.data(), threadName.data() + threadName.size());
    }
} // namespace rsl

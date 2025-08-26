#pragma once

namespace rsl::log
{
    template <derived_from<flag_formatter>... FlagFormatterTypes>
    pattern_formatter::pattern_formatter(string_view pattern, FlagFormatterTypes&&... flagFormatters)
    {
        set_pattern(pattern, rsl::forward<FlagFormatterTypes>(flagFormatters)...);
    }

    template <derived_from<flag_formatter>... FlagFormatterTypes>
    void pattern_formatter::set_pattern(const string_view pattern, FlagFormatterTypes&&... flagFormatters)
    {
        using temporary_formatter_array = array<temporary_object<flag_formatter>, sizeof...(flagFormatters)>;
        m_pattern = dynamic_string::from_view(pattern);
        temporary_formatter_array temporaryFlags = temporary_formatter_array::from_variadic_items(rsl::forward<FlagFormatterTypes>(flagFormatters)...);

        compile_pattern(temporaryFlags);
    }
}

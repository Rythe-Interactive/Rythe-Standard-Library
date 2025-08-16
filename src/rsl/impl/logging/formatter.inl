#pragma once

namespace rsl::log
{
	template <typename Iter, typename ConstIter>
	pattern_formatter::pattern_formatter(const string_view pattern,
	                                     const iterator_view<pattern_formatter::input_flag_pair, Iter, ConstIter> flags)
		: m_pattern(dynamic_string::from_view(pattern)), m_flags(formatters_map::move_from_view(flags)) {}

	template <derived_from<flag_formatter>... Formatters>
	void pattern_formatter::set_pattern(const string_view pattern, Formatters&&... formatters)
	{
		m_pattern = dynamic_string::from_view(pattern);
		static_array<temporary_object<flag_formatter>, sizeof...(formatters)> temporaryFlags;

	}
}

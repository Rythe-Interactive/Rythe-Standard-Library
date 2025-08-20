#pragma once

namespace rsl::log
{
	inline basic_logger::basic_logger(const string_view name, const log::severity severity, const log::severity flushSeverity)
		: m_name(dynamic_string::from_view(name)), m_severity(severity), m_flushSeverity(flushSeverity)
	{}

	inline void basic_logger::set_sinks(array_view<sink*> sinks)
	{
		m_sinks = dynamic_array<sink*>::from_view(sinks);
	}

	inline array_view<sink* const> basic_logger::view_sinks() const noexcept { return m_sinks.view(); }

	template <typename... Args>
	constexpr void basic_logger::log(const log::severity s, const format_string format, Args&&... args) noexcept
	{
		log(s, format, fmt::make_format_args(rsl::forward<Args>(args)...));
	}

	inline void basic_logger::filter(const severity s) noexcept
	{
		m_severity = s;
	}

	inline severity basic_logger::filter_severity() const noexcept
	{
		return m_severity;
	}

	inline void basic_logger::flush_at(const severity s) noexcept
	{
		m_flushSeverity = s;
	}

	inline severity basic_logger::flush_severity() const noexcept
	{
		return m_flushSeverity;
	}
} // namespace rsl::log

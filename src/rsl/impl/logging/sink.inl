#pragma once

namespace rsl::log
{
	inline void sink::set_formatter(temporary_object<formatter>&& sinkFormatter)
	{
		m_formatter = rsl::move(sinkFormatter);
	}
	
	template <derived_from<formatter> FormatterType, typename ... Args>
	void sink::set_formatter(Args&&... args)
	{
		m_formatter = unique_object<FormatterType>::create_in_place(rsl::forward<Args>(args)...);
	}
	
	inline void sink::filter(const severity s) noexcept
	{
		m_severity = s;
	}

	inline severity sink::filter_severity() const noexcept
	{
		return m_severity;
	}
}

#pragma once

namespace rsl::log
{
	inline void sink::set_formatter(formatter&& sinkFormatter)
	{
		m_formatter = unique_object<formatter>::create_in_place(rsl::move(sinkFormatter));
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

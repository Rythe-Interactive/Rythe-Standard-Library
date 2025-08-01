#pragma once

namespace rsl::log
{
	inline void sink::filter(const severity s) noexcept
	{
		m_severity = s;
	}

	inline severity sink::filter_severity() const noexcept
	{
		return m_severity;
	}

	inline bool sink::should_log(const severity s) const noexcept
	{
		return s >= m_severity;
	}
}

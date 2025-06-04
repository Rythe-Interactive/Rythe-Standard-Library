#pragma once

#include "current_thread.hpp"
#include "platform/platform.hpp"

namespace rsl::current_thread
{
	template <time::duration_rep Precision>
	void sleep_for(time::span<Precision> duration) noexcept
	{
		platform::sleep_current_thread(duration.template milliseconds<uint64>());
	}

	template <time::duration_rep Precision, time::clock_type ClockType>
	void sleep_until(time::point<Precision, ClockType> timepoint) noexcept
	{
		sleep_for(timepoint - time::stopwatch<Precision, ClockType>::current_point());
	}
}

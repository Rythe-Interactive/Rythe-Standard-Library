#pragma once

#include "../time/stopwatch.hpp"

#include "thread_id.hpp"
#include "../platform/platform.hpp"

namespace rsl::current_thread
{
    void yield();
    thread_id get_id();

	template <time::duration_rep Precision = time32>
	void sleep_for(time::span<Precision> duration);

	template <time::duration_rep Precision = time32, time::clock_type ClockType = time::timer32::clock_type>
	void sleep_until(time::point<Precision, ClockType> timepoint);

	string_view get_name();
	void set_name(string_view name);
}

#include "current_thread.inl"

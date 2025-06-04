#pragma once

#include "../containers/string_view.hpp"
#include "../time/stopwatch.hpp"

#include "thread_id.hpp"

namespace rsl::current_thread
{
    void yield() noexcept;
    thread_id get_id() noexcept;

	template <time::duration_rep Precision = time32>
	void sleep_for(time::span<Precision> duration) noexcept;

	template <time::duration_rep Precision = time32, time::clock_type ClockType = time::timer32::clock_type>
	void sleep_until(time::point<Precision, ClockType> timepoint) noexcept;

	string_view get_name();
	void set_name(string_view name);
}

#include "current_thread.inl"

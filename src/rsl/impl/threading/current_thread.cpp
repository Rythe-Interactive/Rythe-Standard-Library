#include "current_thread.hpp"

#include "../platform/platform.hpp"

namespace rsl::current_thread
{
	void yield()
	{
		platform::yield_current_thread();
	}

	thread_id get_id()
	{
		return platform::get_current_thread_id();
	}

	string_view get_name()
	{
		return platform::get_thread_name(get_id());
	}

	void set_name(string_view name)
	{
		platform::set_thread_name(get_id(), name);
	}
}

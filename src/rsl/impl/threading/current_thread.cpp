#include "current_thread.hpp"

#include "../platform/platform.hpp"
#include "../containers/string.hpp"

namespace rsl::current_thread
{
	namespace
	{
		dynamic_map<thread_id, dynamic_string> thread_names;
	}

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
		thread_id id = get_id();
		dynamic_string* result = thread_names.find(id);

		if (result)
		{
			return *result;
		}

		return thread_names.emplace(id, dynamic_string::from_array("unknown thread") /*fmt::format("thread {}", id)*/);
	}

	void set_name(string_view name)
	{
		thread_names.emplace_or_replace(get_id(), dynamic_string::from_view(name));
	}
}

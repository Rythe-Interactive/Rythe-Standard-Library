#include "current_thread.hpp"

#include "../platform/platform.hpp"

namespace rsl::current_thread
{
	namespace
	{
		dynamic_map<thread_id, string> thread_names;
	}

	void yield() noexcept
	{
		platform::yield_current_thread();
	}

	thread_id get_id() noexcept
	{
		return platform::get_current_thread_id();
	}

	string_view get_name() noexcept
	{
		thread_id id = get_id();
		string* result = thread_names.find(id);

		if (result)
		{
			return *result;
		}

		return thread_names.emplace(id, string::from_array("unknown thread") /*fmt::format("thread {}", id)*/);
	}

	void set_name(string_view name)
	{
		thread_names.emplace_or_replace(get_id(), name);
	}
}

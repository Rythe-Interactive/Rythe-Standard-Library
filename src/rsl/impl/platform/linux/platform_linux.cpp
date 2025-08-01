#include "../platform.hpp"

#if RYTHE_PLATFORM_LINUX

#include <dlfcn.h>
#include <errno.h>
#include <sched.h>
#include <pthread.h>
#include <utmpx.h>
#include <sys/syscall.h>
#include <sys/prctl.h>

namespace rsl
{
	dynamic_library platform::load_library(cstring path)
	{
		dynamic_library result;
		result.m_handle = dlopen(path, RTLD_NOW);
		return result;
	}

	void platform::release_library(const dynamic_library library)
	{
		dlclose(library.m_handle);
	}

	void* platform::get_symbol(dynamic_library library, cstring symbolName)
	{
		return dlsym(library.m_handle, symbolName);
	}

	thread_id platform::get_current_thread_id()
	{
        // return thread_id{ .nativeId = static_cast<id_type>(syscall( SYS_gettid ) );
		return thread_id{ .nativeId = static_cast<id_type>(gettid()) };
	}

	void platform::yield_current_thread()
	{
		sched_yield();
	}

	void platform::sleep_current_thread(uint32 milliseconds)
	{
		timespec sleepTime;
		timespec remainingTime;
		sleepTime.tv_sec = milliseconds / 1000u;
		sleepTime.tv_nsec = (milliseconds - (sleepTime.tv_sec * 1000u)) * 1000000u;

		while (true)
		{
			int32 result = nanosleep(&sleepTime, &remainingTime);

			int error = 0;
			if (result == -1)
			{
				error = errno;
			}

			if (result == 0 || error != EINTR)
			{
				break;
			}

			sleepTime = remainingTime;
		}
	}
} // namespace rsl

#endif

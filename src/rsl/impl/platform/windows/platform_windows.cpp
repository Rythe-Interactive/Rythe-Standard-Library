#include "../../defines.hpp"

#if RYTHE_PLATFORM_WINDOWS

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#define NOMINMAX
#define NOMCX
#define NOSERVICE
#define NOHELP
#include "winplatformdef.h"
#include <libloaderapi.h>
#include <windef.h>
#include <winbase.h>
#include <processthreadsapi.h>
#include <process.h>

#define RYTHE_DYNAMIC_LIBRARY_HANDLE_IMPL HMODULE
#include "../platform.hpp"

#define RYTHE_THREAD_HANDLE_IMPL HANDLE
#include "threading/thread.hpp"

namespace rsl
{
	namespace
	{
		struct native_thread_context
		{
			pmu_allocator* allocator;
			string name;
			platform::native_thread_start function;
			void* userData;
		};

		DWORD internal_native_thread_start(void* args)
		{
			native_thread_context& context = *static_cast<native_thread_context*>(args);
			pmu_allocator* allocator = context.allocator;

			wstring wideName = to_utf16(context.name);
			[[maybe_unused]] HRESULT _ = ::SetThreadDescription(::GetCurrentThread(), wideName.data());

			const uint32 result = context.function(context.userData);

			internal::default_destroy<native_thread_context>(&context, 1);
			allocator->deallocate(&context);

			return result;
		}
	}

	dynamic_library platform::load_library(const cstring path)
	{
		dynamic_library result;
		result.m_handle = ::LoadLibraryA(path);
		return result;
	}

	void platform::release_library(const dynamic_library library)
	{
		::FreeLibrary(library.m_handle);
	}

	void* platform::get_symbol(const dynamic_library library, const cstring symbolName)
	{
		return bit_cast<void*>(::GetProcAddress(library.m_handle, symbolName));
	}

	thread platform::create_thread(const native_thread_start startFunction, void* userData, string_view name,	pmu_allocator& allocator)
	{
		rsl_always_assert(startFunction);

		thread result;

		native_thread_context* threadContext = allocator.allocate<native_thread_context>();
		if (!threadContext)
		{
			return result;
		}

		internal::default_construct<native_thread_context>(threadContext, 1);

		threadContext->allocator = &allocator;
		threadContext->name = name;
		threadContext->function = startFunction;
		threadContext->userData = userData;

		const HANDLE threadHandle = ::CreateThread(nullptr, 0, &internal_native_thread_start, threadContext, CREATE_SUSPENDED | STACK_SIZE_PARAM_IS_A_RESERVATION, nullptr);

		if (!threadHandle)
		{
			internal::default_destroy<native_thread_context>(threadContext, 1);
			allocator.deallocate(threadContext);
			return result;
		}

		result.m_handle = threadHandle;

		::ResumeThread( threadHandle );

		return result;
	}

	uint32 platform::destroy_thread(const thread thread)
	{
		::WaitForSingleObject( thread.m_handle, INFINITE );

		DWORD exitCode = 0u;
		::GetExitCodeThread( thread.m_handle, &exitCode );

		::CloseHandle( thread.m_handle );

		return static_cast<uint32>(exitCode);
	}

	bool platform::is_thread_active(const thread thread)
	{
		if (!thread.m_handle)
		{
			return false;
		}

		const DWORD waitResult = ::WaitForSingleObject( thread.m_handle, 0 );
		DWORD exitCode = 0u;
		const bool exitCodeResult = ::GetExitCodeThread( thread.m_handle, &exitCode );

		const bool threadIsInactive = (waitResult == WAIT_OBJECT_0 || waitResult == WAIT_FAILED) && (exitCode != STILL_ACTIVE || !exitCodeResult);
		return !threadIsInactive;
	}

	thread_id platform::get_current_thread_id()
	{
		return thread_id{ .nativeId = static_cast<id_type>(::GetCurrentThreadId()) };
	}

	thread_id platform::get_thread_id(const thread thread)
	{
		return thread_id{ .nativeId = static_cast<id_type>(::GetThreadId(thread.m_handle)) };
	}

	void platform::yield_current_thread()
	{
		::SwitchToThread();
	}

	void platform::sleep_current_thread(const uint64 milliseconds)
	{
		::Sleep( milliseconds );
	}
} // namespace rsl

#endif

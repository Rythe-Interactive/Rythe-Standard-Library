#pragma once

#include "../util/common.hpp"
#include "../util/primitives.hpp"

#include "../threading/thread_id.hpp"

namespace rsl
{
	class dynamic_library;
	class thread;

	class platform
	{
	public:
		using native_thread_start = uint32(*)(void* userData);

		static dynamic_library load_library(cstring path);
		static void release_library(dynamic_library library);

		static void* get_symbol(dynamic_library library, cstring symbolName);

		static thread create_thread(native_thread_start startFunction, void* userData = nullptr, string_view name = "unknown thread", pmu_allocator& allocator = *allocator_context::globalAllocator);
		static uint32 destroy_thread(thread thread);

		static bool is_thread_active(thread thread);

		static thread_id get_current_thread_id();
		static thread_id get_thread_id(thread thread);
		static void yield_current_thread();
		static void sleep_current_thread(uint32 milliseconds);

		static void set_thread_name(thread thread, string_view name);
		static void set_thread_name(thread_id threadId, string_view name);
		static string_view get_thread_name(thread thread);
		static string_view get_thread_name(thread_id threadId);
	};

#if !defined(RYTHE_DYNAMIC_LIBRARY_HANDLE_IMPL)
	#define RYTHE_DYNAMIC_LIBRARY_HANDLE_IMPL void*
#endif

#if !defined(RYTHE_DYNAMIC_LIBRARY_HANDLE_DEFAULT_VALUE)
	#define RYTHE_DYNAMIC_LIBRARY_HANDLE_DEFAULT_VALUE nullptr
#endif

	class dynamic_library
	{
	public:
		template <typename T>
		T get_symbol(const cstring symbolName) const
		{
			return bit_cast<T>(platform::get_symbol(*this, symbolName));
		}

		operator bool() const { return m_handle; }

		void release()
		{
			platform::release_library(*this);
			m_handle = RYTHE_DYNAMIC_LIBRARY_HANDLE_DEFAULT_VALUE;
		}

	private:
		using platform_specific_handle = RYTHE_DYNAMIC_LIBRARY_HANDLE_IMPL;

		platform_specific_handle m_handle = RYTHE_DYNAMIC_LIBRARY_HANDLE_DEFAULT_VALUE;

		friend class platform;
	};
} // namespace rsl

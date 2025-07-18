#pragma once
#include "thread_id.hpp"

namespace rsl
{
	#if !defined(RYTHE_THREAD_HANDLE_IMPL)
	#define RYTHE_THREAD_HANDLE_IMPL void*
	#endif

	#if !defined(RYTHE_THREAD_HANDLE_DEFAULT_VALUE)
	#define RYTHE_THREAD_HANDLE_DEFAULT_VALUE nullptr
	#endif

	class thread
	{
	public:
		operator bool() const;

		thread_id get_id() const;
		uint32 join() const;

	private:
		using platform_specific_handle = RYTHE_THREAD_HANDLE_IMPL;

		platform_specific_handle m_handle = RYTHE_THREAD_HANDLE_DEFAULT_VALUE;

		friend class platform;
	};
}

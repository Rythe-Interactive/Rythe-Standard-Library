#pragma once

#include "../util/primitives.hpp"
#include "../defines.hpp"

#if RYTHE_PLATFORM_WINDOWS
	#include "windows/winplatformdef.h"
#endif

namespace rsl
{
	class dynamic_library;

	class platform
	{
	public:
		static dynamic_library load_library(cstring path);

		static void* get_symbol(dynamic_library library, cstring symbolName);
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
		template<typename T>
		T get_symbol(cstring symbolName) const { return reinterpret_cast<T>(platform::get_symbol(*this, symbolName)); }

		operator bool() const { return m_handle; }

	private:
		using platform_specific_handle = RYTHE_DYNAMIC_LIBRARY_HANDLE_IMPL;

		platform_specific_handle m_handle = RYTHE_DYNAMIC_LIBRARY_HANDLE_DEFAULT_VALUE;

		friend class platform;
	};
} // namespace rsl

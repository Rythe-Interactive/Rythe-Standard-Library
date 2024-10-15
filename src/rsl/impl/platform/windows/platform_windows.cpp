#include "../../defines.hpp"

#if RYTHE_PLATFORM_WINDOWS

	#define WIN32_LEAN_AND_MEAN
	#define VC_EXTRALEAN
	#define NOMINMAX
	#include <Windows.h>

	#define RYTHE_DYNAMIC_LIBRARY_HANDLE_IMPL HMODULE
	#include "../platform.hpp"

namespace rsl
{
	dynamic_library platform::load_library(cstring path)
	{
		dynamic_library result;
		result.m_handle = LoadLibraryA(path);
		return result;
	}

	void* platform::get_symbol(dynamic_library library, cstring symbolName)
	{
		return reinterpret_cast<void*>(GetProcAddress(library.m_handle, symbolName));
	}
} // namespace rsl

#endif

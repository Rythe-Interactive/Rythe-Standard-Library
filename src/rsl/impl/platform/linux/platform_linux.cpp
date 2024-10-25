#include "../platform.hpp"

#if RYTHE_PLATFORM_LINUX

	#include <dlfcn.h>

namespace rsl
{
	dynamic_library platform::load_library(cstring path)
	{
		dynamic_library result;
		result.m_handle = dlopen(path, RTLD_NOW);
		return result;
	}

	void* platform::get_symbol(dynamic_library library, cstring symbolName)
	{
		return dlsym(library.m_handle, symbolName);
	}

	void dynamic_library::release()
	{
		dlclose(m_handle);
		m_handle = nullptr;
	}
} // namespace rsl

#endif

#pragma once

#include "dynamic_array.hpp"

namespace rsl
{
	using string = dynamic_array<utf8>;
	using wstring = dynamic_array<utf16>;

	wstring to_utf16(string::view_type str);
}

#pragma once
#include "primitives.hpp"
#include "concepts.hpp"

namespace rsl
{
	template<char_type T>
	[[nodiscard]] constexpr size_type string_length(add_const_t<T>* str, T terminator = T{}) noexcept
	{
		size_type length = 0;
		while (*str != terminator)
		{
			++length;
			++str;
		}

		return length;
	}

	template <class T, size_type N>
	constexpr size_type arr_length(T[N])
	{
		return N;
	}
} // namespace rsl

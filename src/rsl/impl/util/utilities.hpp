#pragma once
#include <bit>
#include <vector>

#include "../defines.hpp"
#include "common.hpp"
#include "primitives.hpp"

namespace rsl
{
	enum class endian
	{
#if RYTHE_PLATFORM_WINDOWS
		little = 0,
		big = 1,
		native = little
#else
		little = __ORDER_LITTLE_ENDIAN__,
		big = __ORDER_BIG_ENDIAN__,
		native = __BYTE_ORDER__
#endif
	};

	template <typename T, typename U>
	constexpr T force_value_cast(U value)
	{
		static_assert(alignof(T) == alignof(U), "Illegal cast of unaligned types.");
		static_assert(sizeof(T) <= sizeof(U), "Illegal cast of non size similar types.");

		return *bit_cast<T*>(&value);
	}

	template <typename To, typename From>
	[[rythe_always_inline]] constexpr To force_cast(From& f) noexcept
	{
		return *bit_cast<To*>(&f);
	}

	template <typename To, typename From>
	[[rythe_always_inline]] constexpr To force_cast(const From& f) noexcept
	{
		return *bit_cast<To*>(&f);
	}
} // namespace rsl

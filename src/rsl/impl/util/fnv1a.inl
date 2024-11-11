#pragma once

#include "../defines.hpp"
#include "primitives.hpp"

namespace rsl::internal::fnv1a
{
	constexpr u64 offset_basis_64 = 14695981039346656037ULL;
	constexpr u64 prime_64 = 1099511628211ULL;

	constexpr u32 offset_basis_32 = 2166136261u;
	constexpr u32 prime_32 = 16777619u;

	[[rythe_always_inline]] constexpr id_type select_prime() noexcept
	{
		if constexpr (sizeof(id_type) >= sizeof(u64))
		{
			return prime_64;
		}
		else
		{
			return prime_32;
		}
	}

	[[rythe_always_inline]] constexpr id_type select_basis() noexcept
	{
		if constexpr (sizeof(id_type) >= sizeof(u64))
		{
			return offset_basis_64;
		}
		else
		{
			return offset_basis_32;
		}
	}

	constexpr id_type prime = select_prime();
	constexpr id_type offset_basis = select_basis();
} // namespace internal::fnv1a

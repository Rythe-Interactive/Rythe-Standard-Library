#pragma once

#include "../defines.hpp"
#include "primitives.hpp"

namespace rsl::internal::fnv1a
{
	constexpr u64 offset_basis_64 = 14695981039346656037ull;
	constexpr u64 prime_64 = 1099511628211ull;

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

	constexpr id_type hash_bytes(span<const byte> bytes) noexcept
	{
		static_assert(sizeof(id_type) == 8 || sizeof(id_type) == 4);
		constexpr size_type blockSize = (sizeof(id_type) == 8) ? 8 : 4;
		const size_type blockCount = bytes.size() / blockSize;
		const size_type remainder = bytes.size() - blockCount;
		const byte* const data = bytes.data();

		id_type hash = internal::fnv1a::offset_basis;

		for (size_type i = 0; i < blockCount; i++)
		{
			hash ^= *bit_cast<const id_type*>(data + i * blockSize);
			hash *= internal::fnv1a::prime;
		}

		if (remainder > 0)
		{
			id_type remainderVal = 0;
			constexpr_memcpy(&remainderVal, data + blockCount * blockSize, remainder);

			hash ^= remainderVal;
			hash *= internal::fnv1a::prime;
		}

		return hash;
	}
} // namespace rsl::internal::fnv1a

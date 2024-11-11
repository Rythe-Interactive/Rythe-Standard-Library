#include "hash.hpp"
#pragma once

#include <bit>

namespace rsl
{
	constexpr id_type hash_bytes(span<const byte> bytes, id_type seed) noexcept
	{
		for (byte b : bytes)
		{
			seed ^= b;
			seed *= internal::fnv1a::prime;
		}

		return seed;
	}

	constexpr id_type hash_string(string_view str, id_type seed) noexcept
	{
		for (char b : str)
		{
			seed ^= b;
			seed *= internal::fnv1a::prime;
		}

		return seed;
	}

	template <same_as<id_type>... hash_types>
	constexpr id_type combine_hash(id_type a_seed, id_type a_hash, hash_types... a_hashes) noexcept
	{
		if constexpr (sizeof(id_type) >= 8)
		{
			a_seed ^= (a_hash + (0x517cc1b727220a95 + (a_seed << 6) + (a_seed >> 2)));
		}
		else
		{
			a_seed ^= (a_hash + (0x9e3779b9 + (a_seed << 6) + (a_seed >> 2)));
		}

		if constexpr (sizeof...(hash_types) != 0)
		{
			return combine_hash(a_seed, a_hashes...);
		}
		else
		{
			return a_seed;
		}
	}

	template <typename T>
	constexpr id_type hash_value(const T& val) noexcept
	{
		return hash_bytes(internal::fnv1a::offset_basis, std::span(std::bit_cast<const byte*>(&val), sizeof(T)));
	}

} // namespace rsl

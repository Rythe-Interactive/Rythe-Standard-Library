#include "hash.hpp"
#pragma once

#include "common.hpp"

namespace rsl::internal
{
	constexpr bool hash_static_assert_check =
		hash_algorithm::default_hash == hash_algorithm::fnv1a || hash_algorithm::default_hash == hash_algorithm::robin;
	template <typename T, hash_algorithm Algorithm>
	struct hash_strategy
	{
		[[rythe_always_inline]] constexpr static id_type hash(const T& val) noexcept
		{
			return hash_bytes<Algorithm>(std::span(bit_cast<const byte*>(&val), sizeof(T)));
		}
	};

	template <enum_type Enum, hash_algorithm Algorithm>
	struct hash_strategy<Enum, Algorithm>
	{
		[[rythe_always_inline]] constexpr static id_type hash(const Enum& val) noexcept
		{
			using underlying = std::underlying_type_t<Enum>;
			return hash_strategy<underlying, Algorithm>::hash(static_cast<underlying>(val));
		}
	};
} // namespace rsl::internal

#include "fnv1a.inl"
#include "robin_hash.inl"

namespace rsl
{
	template <hash_algorithm Algorithm>
	constexpr id_type hash_bytes(span<const byte> bytes) noexcept
	{
		if constexpr (Algorithm == hash_algorithm::fnv1a)
		{
			return internal::fnv1a::hash_bytes(bytes);
		}
		else if constexpr (Algorithm == hash_algorithm::robin)
		{
			return internal::robin_hash::hash_bytes(bytes);
		}
		else
		{
			static_assert(internal::hash_static_assert_check, "unknown hash algorithm.");
		}
	}

	constexpr id_type hash_bytes(span<const byte> bytes) noexcept
	{
		return hash_bytes<hash_algorithm::default_hash>(bytes);
	}

	template <hash_algorithm Algorithm>
	constexpr id_type hash_string(string_view str) noexcept
	{
		return hash_bytes<Algorithm>(span<const byte>(bit_cast<const byte*>(str.data()), str.size()));
	}

	constexpr id_type hash_string(string_view str) noexcept
	{
		return hash_string<hash_algorithm::default_hash>(str);
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

	template <hash_algorithm Algorithm, typename T>
	constexpr id_type hash_value(const T& val) noexcept
	{
		return internal::hash_strategy<remove_cvr_t<T>, Algorithm>::hash(val);
	}

	template <typename T>
	constexpr id_type hash_value(const T& val) noexcept
	{
		return hash_value<hash_algorithm::default_hash>(val);
	}

} // namespace rsl

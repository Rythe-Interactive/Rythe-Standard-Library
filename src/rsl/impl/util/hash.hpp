#pragma once

#include "../containers/span.hpp"
#include "../containers/string_view.hpp"
#include "../defines.hpp"
#include "primitives.hpp"
#include "string_util.hpp"
#include "type_util.hpp"

#ifndef RSL_DEFAULT_HASH_ALGORITHM
	#define RSL_DEFAULT_HASH_ALGORITHM robin
#endif

namespace rsl
{
	enum struct hash_algorithm : u8
	{
		fnv1a,
		robin,
		default_hash = RSL_DEFAULT_HASH_ALGORITHM
	};

	template <hash_algorithm Algorithm>
	[[rythe_always_inline]] constexpr id_type hash_bytes(span<const byte> bytes) noexcept;

	[[rythe_always_inline]] constexpr id_type hash_bytes(span<const byte> bytes) noexcept;

	template <hash_algorithm Algorithm>
	[[rythe_always_inline]] constexpr id_type hash_string(string_view str) noexcept;

	[[rythe_always_inline]] constexpr id_type hash_string(string_view str) noexcept;

	template <same_as<id_type>... hash_types>
	[[rythe_always_inline]] constexpr id_type combine_hash(id_type seed, id_type hash, hash_types... hashes) noexcept;

	template <hash_algorithm Algorithm, typename T>
	[[rythe_always_inline]] constexpr id_type hash_value(const T& val) noexcept;

	template <typename T>
	[[rythe_always_inline]] constexpr id_type hash_value(const T& val) noexcept;

	template <typename T>
	struct hash
	{
		[[nodiscard]] [[rythe_always_inline]] constexpr id_type operator()(const T& val) const noexcept
		{
			return hash_value(val);
		}
	};

	template <>
	struct hash<id_type>
	{
		[[nodiscard]] [[rythe_always_inline]] constexpr id_type operator()(id_type val) const noexcept { return val; }
	};
} // namespace rsl

#include "hash.inl"

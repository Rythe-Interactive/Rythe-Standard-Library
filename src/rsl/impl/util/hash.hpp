#pragma once

#include "../containers/span.hpp"
#include "../containers/string_view.hpp"
#include "../defines.hpp"
#include "primitives.hpp"
#include "string_util.hpp"
#include "type_util.hpp"

#include "fnv1a.inl"

namespace rsl
{
	constexpr id_type hash_bytes(span<const byte> bytes, id_type seed = internal::fnv1a::offset_basis) noexcept;

	constexpr id_type hash_string(string_view str, id_type seed = internal::fnv1a::offset_basis) noexcept;

	template <same_as<id_type>... hash_types>
	constexpr id_type combine_hash(id_type seed, id_type hash, hash_types... hashes) noexcept;

	template <typename T>
	constexpr id_type hash_value(const T& val) noexcept;

	template <typename T>
	struct hash
	{
		[[nodiscard]] rythe_always_inline constexpr id_type operator()(const T& val) const noexcept
		{
			return hash_value(val);
		}
	};

	template <>
	struct hash<id_type>
	{
		[[nodiscard]] rythe_always_inline constexpr id_type operator()(id_type val) const noexcept { return val; }
	};
} // namespace rsl

#include "hash.inl"

#include "hash.hpp"
#pragma once

namespace rsl
{
	template <typename hash_type, typename... hash_types>
	rythe_always_inline constexpr static id_type combine_hash(id_type a_seed, hash_type a_hash, hash_types... a_hashes)
	{
		static_assert(std::is_same_v<id_type, hash_type>, "Hashes need to be of type: id_type");

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
	rythe_always_inline constexpr static id_type hash(const T& val)
	{
		constexpr ::std::hash<T> hasher{};
		return hasher(val);
	}

	rythe_always_inline constexpr name_hash::operator id_type() const noexcept
	{
		return value;
	}

	template <typename T>
	rythe_always_inline constexpr type_hash<T>::operator id_type() const noexcept
	{
		return value;
	}

	rythe_always_inline constexpr name_hash::name_hash() noexcept {}

	rythe_always_inline constexpr name_hash::name_hash(id_type val) noexcept
		: value(val)
	{
	}

	template <size_type N>
	rythe_always_inline constexpr name_hash::name_hash(const char (&name)[N]) noexcept
		: value(nameHash<N>(name))
	{
	}

	template <size_type N>
	rythe_always_inline constexpr name_hash::name_hash(const string_literal<N>& name) noexcept
		: value(nameHash<N>(name))
	{
	}

	rythe_always_inline constexpr name_hash literals::operator""_hs(cstring src)
	{
		return name_hash{nameHash(src)};
	}

} // namespace rsl

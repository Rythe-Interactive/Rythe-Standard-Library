#pragma once
#include "../../memory/allocator.hpp"
#include "../../util/hash.hpp"
#include <map>
#include <unordered_map>

namespace rsl
{
	template <template <typename...> typename MapType>
	struct map_traits;

	template <>
	struct map_traits<std::unordered_map>
	{
		template <typename Key, typename Value>
		using value_type = std::pair<const Key, Value>;

		template <typename Key>
		using default_comparer = std::equal_to<Key>;

		template <typename Key, typename Value>
		using default_allocator = stl_allocator_compatible_wrapper<allocator<value_type<Key, Value>>>;

		template <typename Key, typename Value, universal_allocator_type Alloc, constructor_type Constructor>
			requires same_as<value_type<Key, Value>, typename Constructor::value_type>
		using allocator_type = stl_allocator_compatible_wrapper<allocator<value_type<Key, Value>, Alloc, Constructor>>;

		template <typename Key, typename Value>
		using default_type =
			std::unordered_map<Key, Value, hash<Key>, default_comparer<Key>, default_allocator<Key, Value>>;

		template <
			typename Key, typename Value, typename Compare, universal_allocator_type Alloc,
			constructor_type Constructor>
			requires same_as<value_type<Key, Value>, typename Constructor::value_type>
		using customized_type =
			std::unordered_map<Key, Value, hash<Key>, Compare, allocator_type<Key, Value, Alloc, Constructor>>;
	};

	template <>
	struct map_traits<std::map>
	{
		template <typename Key, typename Value>
		using value_type = std::pair<const Key, Value>;

		template <typename Key>
		using default_comparer = std::less<Key>;

		template <typename Key, typename Value>
		using default_allocator = stl_allocator_compatible_wrapper<allocator<std::pair<const Key, Value>>>;

		template <typename Key, typename Value, universal_allocator_type Alloc, constructor_type Constructor>
			requires same_as<value_type<Key, Value>, typename Constructor::value_type>
		using allocator_type = stl_allocator_compatible_wrapper<allocator<value_type<Key, Value>, Alloc, Constructor>>;

		template <typename Key, typename Value>
		using default_type = std::map<Key, Value, default_comparer<Key>, default_allocator<Key, Value>>;

		template <
			typename Key, typename Value, typename Compare, universal_allocator_type Alloc,
			constructor_type Constructor>
			requires same_as<value_type<Key, Value>, typename Constructor::value_type>
		using customized_type = std::map<Key, Value, Compare, allocator_type<Key, Value, Alloc, Constructor>>;
	};
} // namespace rsl

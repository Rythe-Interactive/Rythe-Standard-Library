#pragma once
#include "../../memory/stl_compatibility.hpp"
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
		using default_allocator = stl_allocator_compatible_wrapper<typed_allocator<value_type<Key, Value>>>;

		template <typename Key, typename Value, allocator_type Alloc, factory_type Factory>
			requires same_as<value_type<Key, Value>, typename Factory::value_type>
		using alloc_type = stl_allocator_compatible_wrapper<typed_allocator<value_type<Key, Value>, Alloc, Factory>>;

		template <typename Key, typename Value>
		using default_type =
			std::unordered_map<Key, Value, hash<Key>, default_comparer<Key>, default_allocator<Key, Value>>;

		template <typename Key, typename Value, typename Compare, allocator_type Alloc, factory_type Factory>
			requires same_as<value_type<Key, Value>, typename Factory::value_type>
		using customized_type =
			std::unordered_map<Key, Value, hash<Key>, Compare, alloc_type<Key, Value, Alloc, Factory>>;
	};

	template <>
	struct map_traits<std::map>
	{
		template <typename Key, typename Value>
		using value_type = std::pair<const Key, Value>;

		template <typename Key>
		using default_comparer = std::less<Key>;

		template <typename Key, typename Value>
		using default_allocator = stl_allocator_compatible_wrapper<typed_allocator<std::pair<const Key, Value>>>;

		template <typename Key, typename Value, allocator_type Alloc, factory_type Factory>
			requires same_as<value_type<Key, Value>, typename Factory::value_type>
		using alloc_type = stl_allocator_compatible_wrapper<typed_allocator<value_type<Key, Value>, Alloc, Factory>>;

		template <typename Key, typename Value>
		using default_type = std::map<Key, Value, default_comparer<Key>, default_allocator<Key, Value>>;

		template <typename Key, typename Value, typename Compare, allocator_type Alloc, factory_type Factory>
			requires same_as<value_type<Key, Value>, typename Factory::value_type>
		using customized_type = std::map<Key, Value, Compare, alloc_type<Key, Value, Alloc, Factory>>;
	};
} // namespace rsl

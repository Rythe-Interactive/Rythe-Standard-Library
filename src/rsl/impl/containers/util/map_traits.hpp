#pragma once
#include "../../util/hash.hpp"
#include <map>
#include <unordered_map>
#include "../../memory/allocator.hpp"

namespace rsl
{
	template <template <typename...> typename MapType>
	struct map_traits;

	template <>
	struct map_traits<std::unordered_map>
	{
		template <typename Key>
        using default_comparer = std::equal_to<Key>;
		template <typename Key, typename Value>
        using default_allocator = stl_allocator_compatible_wrapper<allocator<std::pair<const Key, Value>>>;

		template <typename Key, typename Value>
		using default_type = std::unordered_map<Key, Value, hash<Key>, default_comparer<Key>, default_allocator<Key, Value>>;
		template <typename Key, typename Value, typename Compare>
		using comparer_type = std::unordered_map<Key, Value, hash<Key>, Compare, default_allocator<Key, Value>>;
		template <typename Key, typename Value, typename Alloc>
		using allocator_type = std::unordered_map<
			Key, Value, hash<Key>, default_comparer<Key>,
			stl_allocator_compatible_wrapper<typename Alloc::template retarget<std::pair<const Key, Value>>>>;
		template <typename Key, typename Value, typename Compare, typename Alloc>
		using customized_type = std::unordered_map<
			Key, Value, hash<Key>, Compare,
			stl_allocator_compatible_wrapper<typename Alloc::template retarget<std::pair<const Key, Value>>>>;
	};

	template <>
	struct map_traits<std::map>
	{
		template <typename Key>
        using default_comparer = std::less<Key>;
		template <typename Key, typename Value>
        using default_allocator = stl_allocator_compatible_wrapper<allocator<std::pair<const Key, Value>>>;

		template <typename Key, typename Value>
		using default_type = std::map<Key, Value, default_comparer<Key>, default_allocator<Key, Value>>;
		template <typename Key, typename Value, typename Compare>
		using comparer_type = std::map<Key, Value, Compare, default_allocator<Key, Value>>;
		template <typename Key, typename Value, typename Alloc>
		using allocator_type = std::map<Key, Value, default_comparer<Key>, stl_allocator_compatible_wrapper<typename Alloc::template retarget<std::pair<const Key, Value>>>>;
		template <typename Key, typename Value, typename Compare, typename Alloc>
		using customized_type = std::map<Key, Value, Compare, stl_allocator_compatible_wrapper<typename Alloc::template retarget<std::pair<const Key, Value>>>>;
	};
} // namespace rsl

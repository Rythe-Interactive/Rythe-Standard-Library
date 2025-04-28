#pragma once

#include "hash_map.hpp"
#include "map_info.hpp"

namespace rsl
{
	template <
		typename Key, typename Value, allocator_type Alloc = default_allocator, bool IsFlat = true,
		typed_factory_type FactoryType = default_factory<internal::map_value_type<Key, Value, IsFlat>>,
		typename Hash = ::rsl::hash<Key>, typename KeyEqual = equal<Key>,
		ratio_type MaxLoadFactor = ::std::ratio<80, 100>, bool IsLarge = true,
		size_type FingerprintSize = internal::recommended_fingerprint_size<IsLarge>>
	class dynamic_map :
		public hash_map_base<
			map_info<Key, Value, Hash, KeyEqual, IsFlat, Alloc, FactoryType, MaxLoadFactor, FingerprintSize>>
	{
	private:
		using base_class = hash_map_base<
			map_info<Key, Value, Hash, KeyEqual, IsFlat, Alloc, FactoryType, MaxLoadFactor, FingerprintSize>>;

	public:
		using hash_map_base<
			map_info<Key, Value, Hash, KeyEqual, IsFlat, Alloc, FactoryType, MaxLoadFactor, FingerprintSize>>::hash_map_base;
	};
} // namespace rsl

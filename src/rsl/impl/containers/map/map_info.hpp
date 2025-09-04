#pragma once

#include "../../util/hash.hpp"
#include "../util/comparers.hpp"

#include "hasher_wrapper.hpp"
#include "map_bucket.hpp"

namespace rsl
{
	namespace internal
	{
		template <typename Key, typename Value, bool IsFlat>
		using map_value_type = typename conditional<
			is_void<Value>::value, Key, pair<typename conditional<IsFlat, Key, const Key>::type, Value>>::type;
	}

	enum struct [[rythe_open_enum]] hash_map_flags
	{
		none = 0,
		flat = 1 << 0,
		large = 1 << 1,
		all = flat | large,
		default_flags = all,
	};

	RYTHE_BIT_FLAG_OPERATORS(hash_map_flags)

	constexpr bool hash_map_flags_is_flat(const hash_map_flags flags) noexcept
	{
		return (flags & hash_map_flags::flat) != hash_map_flags::none;
	}

	constexpr bool hash_map_flags_is_large(const hash_map_flags flags) noexcept
	{
		return (flags & hash_map_flags::large) != hash_map_flags::none;
	}

	template <
		typename Key, typename Value, hash_map_flags Flags = hash_map_flags::default_flags, allocator_type Alloc = default_allocator,
		typed_factory_type FactoryType = default_factory<internal::map_value_type<Key, Value, hash_map_flags_is_flat(Flags)>>,
		typename Hash = ::rsl::hash<Key>, typename KeyEqual = equal<Key>,
		ratio_type MaxLoadFactor = ::std::ratio<80, 100>,
		size_type FingerprintSize = internal::recommended_fingerprint_size<hash_map_flags_is_large(Flags)>>
	struct map_info
	{
		constexpr static float32 max_load_factor = static_cast<float32>(MaxLoadFactor::num) / static_cast<float32>(MaxLoadFactor::den);
		static_assert(max_load_factor > 0.1f && max_load_factor <= 0.99f, "MaxLoadFactor needs to be > 0.1 && < 0.99");

		using key_type = Key;
		using mapped_type = Value;

		constexpr static bool is_flat = hash_map_flags_is_flat(Flags);
		constexpr static bool is_large = hash_map_flags_is_large(Flags);

		using bucket_type = internal::hash_map_bucket<is_large, FingerprintSize>;
		using psl_type = typename bucket_type::psl_type;
		using storage_type = typename bucket_type::storage_type;

		using hasher_type = internal::hasher_wrapper<key_type, Hash>;
		using key_comparer_type = KeyEqual;

		static constexpr bool is_map = !is_void<mapped_type>::value;
		static constexpr bool is_set = !is_map;
		static constexpr bool is_transparent =
			has_is_transparent<hasher_type>::value && has_is_transparent<key_comparer_type>::value;

		using value_type = internal::map_value_type<Key, Value, is_flat>;

		using allocator_t = Alloc;

		template <typename T>
		using factory_t = typename FactoryType::template retarget<T>;

		constexpr static bool nothrow_constructible =
			is_nothrow_constructible_v<hasher_type> && is_nothrow_constructible_v<key_comparer_type>;
		constexpr static bool nothrow_copy_constructible =
			is_nothrow_copy_constructible_v<hasher_type> && is_nothrow_copy_constructible_v<key_comparer_type>;
		constexpr static bool nothrow_hasher_copy_constructible =
			is_nothrow_copy_constructible_v<hasher_type> && is_nothrow_constructible_v<key_comparer_type>;
		constexpr static bool nothrow_comparer_copy_constructible =
			is_nothrow_constructible_v<hasher_type> && is_nothrow_copy_constructible_v<key_comparer_type>;
	};
} // namespace rsl

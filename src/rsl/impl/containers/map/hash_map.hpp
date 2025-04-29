#pragma once

#include "map_iterator.hpp"
#include "../../memory/memory_pool.hpp"
#include "../any.hpp"
#include "../dynamic_array.hpp"
#include "../optional.hpp"
#include "../util/comparers.hpp"
#include "../pair.hpp"

#include "map_node.hpp"

namespace rsl
{
	// Default implementation of a hash table using robin hood hashing
	template <typename MapInfo>
	class hash_map_base
	{
	public:
		static constexpr bool is_flat = MapInfo::is_flat;
		static constexpr bool is_large = MapInfo::is_large;
		static constexpr bool is_dense = MapInfo::is_dense;

		using key_type = typename MapInfo::key_type;
		using mapped_type = typename MapInfo::mapped_type;
		using hasher_type = typename MapInfo::hasher_type;
		using key_comparer_type = typename MapInfo::key_comparer_type;

		static constexpr bool is_map = MapInfo::is_map;
		static constexpr bool is_set = MapInfo::is_set;
		static constexpr bool is_transparent = MapInfo::is_transparent;

		static constexpr float32 max_load_factor = MapInfo::max_load_factor;

		using value_type = typename MapInfo::value_type;
		using node_type = internal::map_node<MapInfo>;
		using bucket_type = typename MapInfo::bucket_type;
		using psl_type = typename MapInfo::psl_type;
		using storage_type = typename MapInfo::storage_type;

		using allocator_t = typename MapInfo::allocator_t;
		using allocator_storage_type = allocator_storage<allocator_t>;

		using factory_t = typename MapInfo::template factory_t<mapped_type>;
		using factory_storage_type = factory_storage<factory_t>;

		using bucket_factory_t = typename MapInfo::template factory_t<bucket_type>;
		using node_factory_t = typename MapInfo::template factory_t<node_type>;

	private:
		using data_pool = conditional_storage<!is_flat, memory_pool<value_type, allocator_t>>;
		using value_container = dynamic_array<node_type, allocator_t, node_factory_t>;
		using bucket_container = dynamic_array<bucket_type, allocator_t, bucket_factory_t>;

		constexpr static bool nothrow_constructible_alloc =
			is_nothrow_constructible_v<data_pool, const allocator_storage_type&> &&
			is_nothrow_constructible_v<value_container, const allocator_storage_type&> &&
			is_nothrow_constructible_v<bucket_container, const allocator_storage_type&>;

		constexpr static bool nothrow_constructible_fact =
			is_nothrow_constructible_v<value_container, const factory_storage_type&> &&
			is_nothrow_constructible_v<bucket_container, const factory_storage<bucket_factory_t>&>;

		constexpr static bool nothrow_constructible_alloc_fact =
			nothrow_constructible_alloc && nothrow_constructible_fact;

	public:
		using iterator_type = hash_map_iterator<hash_map_base, typename value_container::iterator_type>;
		using const_iterator_type = hash_map_iterator<hash_map_base, typename value_container::const_iterator_type>;
		using reverse_iterator_type = hash_map_iterator<hash_map_base, typename value_container::reverse_iterator_type>;
		using const_reverse_iterator_type = hash_map_iterator<hash_map_base, typename value_container::const_reverse_iterator_type>;

		[[rythe_always_inline]] constexpr hash_map_base() noexcept(MapInfo::nothrow_constructible);

		[[rythe_always_inline]] constexpr hash_map_base(const hasher_type& h, const key_comparer_type& equal)
			noexcept(MapInfo::nothrow_copy_constructible);

		[[rythe_always_inline]] explicit constexpr hash_map_base(const hasher_type& h)
			noexcept(MapInfo::nothrow_hasher_copy_constructible);

		[[rythe_always_inline]] explicit constexpr hash_map_base(const key_comparer_type& equal)
			noexcept(MapInfo::nothrow_comparer_copy_constructible);

		[[rythe_always_inline]] explicit constexpr hash_map_base(const allocator_storage_type& allocStorage)
			noexcept(nothrow_constructible_alloc);

		[[rythe_always_inline]] explicit constexpr hash_map_base(const factory_storage_type& factoryStorage)
			noexcept(nothrow_constructible_fact);

		[[rythe_always_inline]] constexpr hash_map_base(
			const allocator_storage_type& allocStorage,
			const factory_storage_type& factoryStorage
		) noexcept(nothrow_constructible_alloc_fact);

		[[nodiscard]] [[rythe_always_inline]] constexpr size_type size() const noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr bool empty() const noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr size_type capacity() const noexcept;

		void reserve(size_type newCapacity)
			noexcept(noexcept(declval<bucket_container>().reserve(0)) && noexcept(declval<value_container>().reserve(0))
			);

		void clear() noexcept;

		bool contains(const key_type& key) const noexcept;

		const mapped_type* find(const key_type& key) const noexcept
			requires (MapInfo::is_map);
		mapped_type* find(const key_type& key) noexcept
		requires (MapInfo::is_map);

		const mapped_type& at(const key_type& key) const
			requires (MapInfo::is_map);
		mapped_type& at(const key_type& key)
			requires (MapInfo::is_map);

		template <typename... Args>
		mapped_type& emplace(const key_type& key, Args&&... args);

		template <typename... Args>
		mapped_type& emplace_or_replace(const key_type& key, Args&&... args);

		template <typename... Args>
		pair<mapped_type&, bool> try_emplace(const key_type& key, Args&&... args);

		[[rythe_always_inline]] constexpr void erase(const key_type& key) noexcept;

		[[nodiscard]] [[rythe_always_inline]] constexpr memory_pool<value_type>& get_memory_pool() noexcept
			requires(!is_flat);
		[[nodiscard]] [[rythe_always_inline]] constexpr const memory_pool<value_type>& get_memory_pool() const noexcept
			requires(!is_flat);

		[[nodiscard]] [[rythe_always_inline]] constexpr allocator_t& get_allocator() noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const allocator_t& get_allocator() const noexcept;

		[[nodiscard]] [[rythe_always_inline]] constexpr factory_t& get_factory() noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const factory_t& get_factory() const noexcept;

		[[nodiscard]] [[rythe_always_inline]] constexpr iterator_type begin() noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const_iterator_type begin() const noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const_iterator_type cbegin() const noexcept;

		[[nodiscard]] [[rythe_always_inline]] constexpr iterator_type end() noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const_iterator_type end() const noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const_iterator_type cend() const noexcept;

		[[nodiscard]] [[rythe_always_inline]] constexpr reverse_iterator_type rbegin() noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const_reverse_iterator_type rbegin() const noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const_reverse_iterator_type crbegin() const noexcept;

		[[nodiscard]] [[rythe_always_inline]] constexpr reverse_iterator_type rend() noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const_reverse_iterator_type rend() const noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const_reverse_iterator_type crend() const noexcept;

	protected:
		template <typename... Args>
		[[nodiscard]] [[rythe_always_inline]] constexpr node_type create_node(const key_type& key, Args&&... args);

		[[rythe_always_inline]] constexpr void destroy_node(node_type& node) noexcept;

		constexpr void rehash(const bucket_container& oldBuckets) noexcept;

		[[rythe_always_inline]] constexpr void maybe_grow() noexcept(noexcept(reserve(0)));

		[[rythe_always_inline]] constexpr static storage_type pack_bucket_psl(const psl_type& unpackedPsl) noexcept;
		[[rythe_always_inline]] constexpr static psl_type unpack_bucket_psl(const bucket_type& bucket) noexcept;

		struct hash_result
		{
			storage_type fingerprint;
			index_type homeIndex;
		};

		[[rythe_always_inline]] constexpr hash_result get_hash_result(const key_type& key) const noexcept;

		enum struct search_result_type : uint8
		{
			newInsertion,
			swap,
			existingItem,
			itemNotFound,
		};

		struct bucket_search_result
		{
			psl_type unpackedPsl;
			search_result_type type;
		};

		constexpr bucket_search_result find_next_available(
			index_type homeIndex, storage_type startPsl, storage_type fingerprint, const key_type& key, bool earlyOut
		) const noexcept;

		enum struct insert_result_type : uint8
		{
			newInsertion,
			existingItem,
		};

		struct insert_result
		{
			index_type index;
			insert_result_type type;
		};

		constexpr insert_result insert_key_internal(
			const key_type& key, index_type valueIndexHint
		) noexcept(noexcept(reserve(0)));

	private:
		value_container m_values;
		bucket_container m_buckets;

		size_type m_lastValueBucketIndex;

		storage_type m_minPsl;
		storage_type m_maxPsl;

		hasher_type m_hasher;
		key_comparer_type m_keyComparer;
		allocator_storage_type m_alloc;
		factory_storage_type m_factory;
		data_pool m_memoryPool;
	};
} // namespace rsl

#include "hash_map.inl"

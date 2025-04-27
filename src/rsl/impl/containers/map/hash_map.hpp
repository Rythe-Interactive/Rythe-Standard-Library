#pragma once

#include "../../memory/memory_pool.hpp"
#include "../any.hpp"
#include "../dynamic_array.hpp"
#include "../optional.hpp"
#include "../util/comparers.hpp"
#include "../pair.hpp"

#include "map_node.hpp"
#include "spdlog/fmt/bundled/format.h"

namespace rsl
{
	// Default implementation of a hash table using robin hood hashing
	template <typename MapInfo>
	class hash_map_base
	{
	public:
		static constexpr bool is_flat = MapInfo::is_flat;
		using key_type = typename MapInfo::key_type;
		using mapped_type = typename MapInfo::mapped_type;
		using hasher_type = typename MapInfo::hasher_type;
		using key_comparer_type = typename MapInfo::key_comparer_type;

		static constexpr bool is_map = MapInfo::is_map;
		static constexpr bool is_set = MapInfo::is_set;
		static constexpr bool is_transparent = MapInfo::is_transparent;

		static constexpr float32 max_load_factor = MapInfo::max_load_factor;

		using value_type = typename MapInfo::value_type;
		using node_type = internal::map_node<hash_map_base>;
		using bucket_type = typename MapInfo::bucket_type;
		using psl_type = typename MapInfo::psl_type;
		using storage_type = typename MapInfo::storage_type;

		using allocator_t = typename MapInfo::allocator_t;
		using allocator_storage_type = allocator_storage<allocator_t>;

		using factory_t = typename MapInfo::template factory_t<mapped_type>;
		using factory_storage_type = factory_storage<factory_t>;

	private:
		using bucket_factory_t = typename MapInfo::template factory_t<bucket_type>;
		using node_factory_t = typename MapInfo::template factory_t<node_type>;

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
		hash_map_base() noexcept(MapInfo::nothrow_constructible)
			: m_values(),
			  m_buckets(),
			  m_hasher(),
			  m_keyComparer(),
			  m_alloc(),
			  m_factory(),
			  m_memoryPool()
		{
		}

		hash_map_base(const hasher_type& h, const key_comparer_type& equal)
			noexcept(MapInfo::nothrow_copy_constructible)
			: m_values(),
			  m_buckets(),
			  m_hasher(h),
			  m_keyComparer(equal),
			  m_alloc(),
			  m_factory(),
			  m_memoryPool()
		{
		}

		explicit hash_map_base(const hasher_type& h) noexcept(MapInfo::nothrow_hasher_copy_constructible)
			: m_values(),
			  m_buckets(),
			  m_hasher(h),
			  m_keyComparer(),
			  m_alloc(),
			  m_factory(),
			  m_memoryPool()
		{
		}

		explicit hash_map_base(const key_comparer_type& equal) noexcept(MapInfo::nothrow_comparer_copy_constructible)
			: m_values(),
			  m_buckets(),
			  m_hasher(),
			  m_keyComparer(equal),
			  m_alloc(),
			  m_factory(),
			  m_memoryPool()
		{
		}

		[[rythe_always_inline]] explicit constexpr hash_map_base(const allocator_storage_type& allocStorage)
			noexcept(nothrow_constructible_alloc)
			: m_values(allocStorage),
			  m_buckets(allocStorage),
			  m_hasher(),
			  m_keyComparer(),
			  m_alloc(allocStorage),
			  m_factory(),
			  m_memoryPool(allocStorage)
		{
		}

		[[rythe_always_inline]] explicit constexpr hash_map_base(const factory_storage_type& factoryStorage)
			noexcept(nothrow_constructible_fact)
			: m_values(factoryStorage),
			  m_buckets(factoryStorage),
			  m_hasher(),
			  m_keyComparer(),
			  m_alloc(),
			  m_factory(factoryStorage),
			  m_memoryPool()
		{
		}

		[[rythe_always_inline]] constexpr hash_map_base(
			const allocator_storage_type& allocStorage, const factory_storage_type& factoryStorage
		) noexcept(nothrow_constructible_alloc_fact)
			: m_values(allocStorage, factoryStorage),
			  m_buckets(allocStorage, factoryStorage),
			  m_hasher(),
			  m_keyComparer(),
			  m_alloc(allocStorage),
			  m_factory(factoryStorage),
			  m_memoryPool(allocStorage)
		{
		}

		[[nodiscard]] [[rythe_always_inline]] constexpr size_type capacity() const noexcept
		{
			return m_buckets.size();
		}

		void reserve(size_type newCapacity) noexcept(noexcept(declval<bucket_container>().reserve(0)) && noexcept(declval<value_container>().reserve(0)))
		{
			m_values.reserve(newCapacity);

			if constexpr (data_pool::holds_value)
			{
				m_memoryPool->reserve(newCapacity);
			}

			if (newCapacity > m_buckets.size())
			{
				bucket_container oldBuckets = move(m_buckets);
				m_buckets = bucket_container(newCapacity, in_place_signal);

				rehash(oldBuckets);
			}
		}

		void clear() noexcept
		{
			m_values.clear();
			m_buckets.clear();

			if constexpr (data_pool::holds_value)
			{
				m_memoryPool->clear();
			}
		}

		template <typename... Args>
		mapped_type& emplace(const key_type& key, Args&&... args)
		{
			return try_emplace(key, std::forward<Args>(args)...).first;
		}

		template <typename... Args>
		mapped_type& emplace_or_replace(const key_type& key, Args&&... args)
		{
			insert_result insertResult = insert_key_internal(key, m_values.size());

			if (insertResult.type == insert_result_type::newInsertion)
			{
				return m_values.emplace_back(*this, key, forward<Args>(args)...).value();
			}

			mapped_type& value = m_values[m_buckets[insertResult.index].index].value();
			value = move(mapped_type(forward<Args>(args)...));
			return value;
		}

		template <typename... Args>
		pair<mapped_type&, bool> try_emplace(const key_type& key, Args&&... args)
		{
			insert_result insertResult = insert_key_internal(key, m_values.size());

			if (insertResult.type == insert_result_type::newInsertion)
			{
				return {ref(m_values.emplace_back(*this, key, forward<Args>(args)...).value()), true};
			}

			return {ref(m_values[m_buckets[insertResult.index].index].value()), false};
		}

		bool contains(const key_type& key) const noexcept
		{
			if (m_buckets.empty())
			{
				return false;
			}

			const hash_result hash = get_hash_result(key);

			bucket_search_result searchResult = find_next_available(hash.homeIndex, 0u, hash.fingerprint, key);

			return searchResult.type == search_result_type::existingItem;
		}

		[[nodiscard]] [[rythe_always_inline]] constexpr memory_pool<value_type>& get_memory_pool() noexcept
			requires(!is_flat)
		{
			return *m_memoryPool;
		}

		[[nodiscard]] [[rythe_always_inline]] constexpr const memory_pool<value_type>& get_memory_pool() const noexcept
			requires(!is_flat)
		{
			return *m_memoryPool;
		}

		[[nodiscard]] [[rythe_always_inline]] constexpr allocator_t& get_allocator() noexcept { return *m_alloc; }

		[[nodiscard]] [[rythe_always_inline]] constexpr const allocator_t& get_allocator() const noexcept
		{
			return *m_alloc;
		}

		[[nodiscard]] [[rythe_always_inline]] constexpr factory_t& get_factory() noexcept { return *m_factory; }

		[[nodiscard]] [[rythe_always_inline]] constexpr const factory_t& get_factory() const noexcept
		{
			return *m_factory;
		}

	protected:
		constexpr void rehash(const bucket_container& oldBuckets)
		{
			for (const bucket_type& bucket : oldBuckets)
			{
				size_type oldIndex = bucket.index;
				key_type key = m_values[oldIndex].key();

				const hash_result hash = get_hash_result(key);

				bucket_search_result searchResult = find_next_available(hash.homeIndex, 0u, hash.fingerprint, key);

				const insert_result result{
					.index = hash.homeIndex + searchResult.unpackedPsl.psl,
					.type =
						(searchResult.type == search_result_type::existingItem ? insert_result_type::existingItem
																			   : insert_result_type::newInsertion)
				};

				bucket_type insertBucket{
					.pslAndFingerprint = pack_bucket_psl(searchResult.unpackedPsl), .index = oldIndex
				};

				index_type currentIndex = result.index;
				while (searchResult.type == search_result_type::swap)
				{
					rsl::swap(m_buckets[currentIndex], insertBucket);
					psl_type insertPsl = unpack_bucket_psl(insertBucket);

					index_type homeIndex = currentIndex - insertPsl.psl;

					searchResult = find_next_available(
						homeIndex, insertPsl.psl + 1, insertPsl.fingerprint, m_values[insertBucket.index].key()
					);
					currentIndex = homeIndex + searchResult.unpackedPsl.psl;

					rsl_assert_frequent(searchResult.type != search_result_type::existingItem);
				}

				rsl_assert_invalid_object(searchResult.type == search_result_type::newInsertion);
				rsl_assert_invalid_object(currentIndex < m_buckets.size());
				m_buckets[currentIndex] = insertBucket;
			}
		}

		[[rythe_always_inline]] constexpr void maybe_grow() noexcept(noexcept(reserve(0)))
		{
			const size_type currentCapacity = capacity();
			if (currentCapacity == 0)
			{
				if constexpr (max_load_factor >= 0.5f)
				{
					reserve(4);
				}
				else if constexpr (max_load_factor >= 0.25f)
				{
					reserve(8);
				}
				else
				{
					reserve(16);
				}
				return;
			}

			float32 currentLoadFactor = m_values.size() / static_cast<float32>(m_buckets.size());

			if (currentLoadFactor >= max_load_factor)
			{
				reserve(currentCapacity * 2);
			}
		}

		static storage_type pack_bucket_psl(const psl_type& unpackedPsl)
		{
			return storage_type((unpackedPsl.psl << bucket_type::fingerprint_size) | unpackedPsl.fingerprint);
		}

		static psl_type unpack_bucket_psl(const bucket_type& bucket)
		{
			psl_type result{};
			result.psl = bucket.pslAndFingerprint & bucket_type::psl_mask;
			result.fingerprint = bucket.pslAndFingerprint & bucket_type::fingerprint_mask;

			return result;
		}

		struct hash_result
		{
			storage_type fingerprint;
			index_type homeIndex;
		};

		hash_result get_hash_result(const key_type& key) const noexcept
		{
			id_type hash = m_hasher.hash(key);

			hash_result result{};

			result.fingerprint = hash & bucket_type::fingerprint_mask;
			result.homeIndex = static_cast<index_type>(hash % m_buckets.size());

			return result;
		}

		enum struct search_result_type : uint8
		{
			newInsertion,
			swap,
			existingItem,
		};

		struct bucket_search_result
		{
			psl_type unpackedPsl;
			search_result_type type;
		};

		bucket_search_result find_next_available(
			index_type homeIndex, storage_type startPsl, storage_type fingerprint, const key_type& key
		) const noexcept
		{
			psl_type insertPsl{.psl = startPsl, .fingerprint = fingerprint};

			index_type searchIndex = homeIndex + insertPsl.psl;
			const size_type bucketCount = m_buckets.size();
			while (searchIndex < bucketCount)
			{
				const bucket_type& bucket = m_buckets[searchIndex];
				if (bucket.pslAndFingerprint == 0u)
				{
					return bucket_search_result{.unpackedPsl = insertPsl, .type = search_result_type::newInsertion};
				}

				psl_type unpackedPsl = unpack_bucket_psl(bucket);

				if (unpackedPsl.psl < insertPsl.psl)
				{
					return bucket_search_result{.unpackedPsl = insertPsl, .type = search_result_type::swap};
				}

				if (unpackedPsl.psl == insertPsl.psl && unpackedPsl.fingerprint == insertPsl.fingerprint)
				{
					if (m_keyComparer(m_values[bucket.index].key(), key))
					{
						return bucket_search_result{.unpackedPsl = insertPsl, .type = search_result_type::existingItem};
					}
				}

				++insertPsl.psl;
				searchIndex = homeIndex + insertPsl.psl;
			}

			rsl_assert_consistent(insertPsl.psl == (m_buckets.size() - homeIndex));
			return bucket_search_result{.unpackedPsl = insertPsl, .type = search_result_type::newInsertion};
		}

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

		insert_result insert_key_internal(const key_type& key, const index_type valueIndexHint)
		{
			maybe_grow();
			const hash_result hash = get_hash_result(key);

			bucket_search_result searchResult = find_next_available(hash.homeIndex, 0u, hash.fingerprint, key);

			const insert_result result{
				.index = hash.homeIndex + searchResult.unpackedPsl.psl,
				.type =
					(searchResult.type == search_result_type::existingItem ? insert_result_type::existingItem
																		   : insert_result_type::newInsertion)
			};

			bucket_type insertBucket{
				.pslAndFingerprint = pack_bucket_psl(searchResult.unpackedPsl), .index = valueIndexHint
			};

			index_type currentIndex = result.index;
			while (searchResult.type == search_result_type::swap)
			{
				rsl::swap(m_buckets[currentIndex], insertBucket);
				psl_type insertPsl = unpack_bucket_psl(insertBucket);

				index_type homeIndex = currentIndex - insertPsl.psl;

				searchResult = find_next_available(
					homeIndex, insertPsl.psl + 1, insertPsl.fingerprint, m_values[insertBucket.index].key()
				);
				currentIndex = homeIndex + searchResult.unpackedPsl.psl;

				rsl_assert_frequent(searchResult.type != search_result_type::existingItem);
			}

			if (searchResult.type == search_result_type::newInsertion)
			{
				rsl_assert_invalid_object(currentIndex < m_buckets.size());
				m_buckets[currentIndex] = insertBucket;
			}

			return result;
		}

	private:
		value_container m_values;
		bucket_container m_buckets;

		hasher_type m_hasher;
		key_comparer_type m_keyComparer;
		allocator_storage_type m_alloc;
		factory_storage_type m_factory;
		data_pool m_memoryPool;
	};
} // namespace rsl

#include "hash_map.inl"

#pragma once

#include "../math/basic/constraint.hpp"
#include "../memory/memory_pool.hpp"
#include "../util/hash.hpp"
#include "../util/utilities.hpp"
#include "dynamic_array.hpp"
#include "iterators.hpp"
#include "pair.hpp"
#include "util/comparers.hpp"

namespace rsl
{
	template <typename T, typename = void>
	struct has_is_transparent : public false_type
	{
	};

	template <typename T>
	struct has_is_transparent<T, void_t<typename T::is_transparent>> : public true_type
	{
	};

	namespace internal
	{
		template <typename MapType>
		class hash_map_node
		{
		public:
			using map_type = MapType;
			using key_type = typename MapType::key_type;
			using mapped_type = typename MapType::mapped_type;
			using value_type = typename MapType::value_type;

			template <typename... Args>
			explicit hash_map_node(map_type& map, Args&&... args)
				: m_data(map.get_memory_pool().allocate())
			{
				map.get_factory().construct(m_data, 1, forward<Args>(args)...);
			}

			hash_map_node(map_type&, hash_map_node&& other) noexcept
				: m_data(other.m_data)
			{
			}

			void destroy(map_type& map) noexcept
			{
				map.get_factory().destroy(m_data, 1);
				map.get_memory_pool().deallocate(m_data);
			}

			value_type* operator->() noexcept { return m_data; }
			const value_type* operator->() const noexcept { return m_data; }

			value_type& operator*() { return *m_data; }
			const value_type& operator*() const { return *m_data; }

			[[nodiscard]] key_type& key() noexcept
				requires map_type::is_map
			{
				return m_data->first;
			}

			[[nodiscard]] const key_type& key() const noexcept
				requires map_type::is_map
			{
				return m_data->first;
			}

			[[nodiscard]] value_type& key() noexcept
				requires map_type::is_set
			{
				return *m_data;
			}

			[[nodiscard]] const value_type& key() const noexcept
				requires map_type::is_set
			{
				return *m_data;
			}

			[[nodiscard]] mapped_type& value() noexcept
				requires map_type::is_map
			{
				return m_data->second;
			}

			[[nodiscard]] const mapped_type& value() const noexcept
				requires map_type::is_map
			{
				return m_data->second;
			}

		private:
			value_type* m_data;
		};

		template <typename MapType>
		class flat_hash_map_node
		{
		public:
			using map_type = MapType;
			using key_type = typename MapType::key_type;
			using mapped_type = typename MapType::mapped_type;
			using value_type = typename MapType::value_type;

			template <typename... Args>
			explicit flat_hash_map_node(map_type& map, Args&&... args)
				noexcept(is_nothrow_constructible_v<value_type, Args...>)
			{
				map.get_factory().construct(&m_data, 1, forward<Args>(args)...);
			}

			flat_hash_map_node(map_type& map, flat_hash_map_node&& other)
				noexcept(is_nothrow_move_constructible_v<value_type>)
			{
				map.get_factory().move(&m_data, &other.m_data, 1);
			}

			void destroy(map_type& map) noexcept { map.get_factory().destroy(&m_data, 1); }

			value_type* operator->() noexcept { return &m_data; }
			const value_type* operator->() const noexcept { return &m_data; }

			value_type& operator*() noexcept { return m_data; }
			const value_type& operator*() const noexcept { return m_data; }

			[[nodiscard]] key_type& key() noexcept
				requires map_type::is_map
			{
				return m_data.first;
			}

			[[nodiscard]] const key_type& key() const noexcept
				requires map_type::is_map
			{
				return m_data.first;
			}

			[[nodiscard]] value_type& key() noexcept
				requires map_type::is_set
			{
				return m_data;
			}

			[[nodiscard]] const value_type& key() const noexcept
				requires map_type::is_set
			{
				return m_data;
			}

			[[nodiscard]] mapped_type& value() noexcept
				requires map_type::is_map
			{
				return m_data.second;
			}

			[[nodiscard]] const mapped_type& value() const noexcept
				requires map_type::is_map
			{
				return m_data.second;
			}

		private:
			union
			{
				value_type m_data;
				byte m_dummy;
			};
		};

		template <typename MapType, bool isFlat = false>
		struct select_node_type
		{
			using type = hash_map_node<MapType>;
		};

		template <typename MapType>
		struct select_node_type<MapType, true>
		{
			using type = flat_hash_map_node<MapType>;
		};

		template <typename MapType>
		using map_node = typename select_node_type<MapType, MapType::is_flat>::type;

		template <bool Large>
		constexpr bool recommended_fingerprint_size = Large ? 16 : 8;

		template <bool Large = false, size_type FingerprintSize = recommended_fingerprint_size<Large>>
		struct hash_map_bucket
		{
			static_assert(FingerprintSize < 32);
			static constexpr size_type fingerprintSize = FingerprintSize; // bits

			// increment the Nth bit that skips all fingerprint bits.
			static constexpr size_type pslIncrement = 1 << fingerprintSize;
			static constexpr size_type fingerprintMask = pslIncrement - 1; // 0xFF

			uint32 pslAndFingerprint;
			uint32 index;
		};

		template <size_type FingerprintSize>
		struct hash_map_bucket<true, FingerprintSize>
		{
			static_assert(FingerprintSize < 64);
			static constexpr size_type fingerprintSize = FingerprintSize; // bits

			// increment the Nth bit that skips all fingerprint bits.
			static constexpr size_type pslIncrement = 1 << fingerprintSize;
			static constexpr size_type fingerprintMask = pslIncrement - 1; // 0xFFFF

			uint64 pslAndFingerprint;
			uint64 index;
		};
	} // namespace internal

	struct move_to_next_tag_type
	{
	};

	[[maybe_unused]] constexpr move_to_next_tag_type move_to_next_tag{};

	template <typename MapType, bool IsConst = false>
	class hash_map_iterator
	{
	public:
		using map_type = MapType;
		using key_type = typename map_type::key_type;
		using mapped_type = typename map_type::mapped_type;
		using value_type = typename map_type::value_type;
		using node_type = typename map_type::node_type;
		using node_ptr = conditional_t<IsConst, const node_type*, node_type*>;

		using ref_type = conditional_t<IsConst, const value_type&, value_type&>;
		using ptr_type = conditional_t<IsConst, const value_type*, value_type*>;

		hash_map_iterator() noexcept = default;

		template <bool OtherConst>
		hash_map_iterator(const hash_map_iterator<map_type, OtherConst>& other) noexcept
			requires(IsConst && !OtherConst)
			: m_node(other.m_node),
			  m_info(other.m_info)
		{
		}

		hash_map_iterator(node_ptr node, const uint8* info) noexcept
			: m_node(node),
			  m_info(info)
		{
		}

		hash_map_iterator(node_ptr node, const uint8* info, move_to_next_tag_type) noexcept
			: m_node(node),
			  m_info(info)
		{
			find_next();
		}

		template <bool OtherConst>
		hash_map_iterator& operator=(const hash_map_iterator<map_type, OtherConst>& other) noexcept
			requires(IsConst && !OtherConst)
		{
			m_node = other.m_node;
			m_info = other.m_info;
			return *this;
		}

		hash_map_iterator& operator++() noexcept
		{
			m_info++;
			m_node++;
			find_next();
			return *this;
		}

		hash_map_iterator operator++(int) noexcept
		{
			hash_map_iterator tmp = *this;
			++(*this);
			return tmp;
		}

		ref_type operator*() const { return **m_node; }
		ptr_type operator->() const { return &**m_node; }

		template <bool OtherConst>
		bool operator==(const hash_map_iterator<map_type, OtherConst>& other) const noexcept
		{
			return m_node == other.m_node;
		}

		template <bool OtherConst>
		bool operator!=(const hash_map_iterator<map_type, OtherConst>& other) const noexcept
		{
			return m_node != other.m_node;
		}

	private:
		void find_next() noexcept
		{
			size_type n = 0ull;
			while ((n = unaligned_load<size_type>(m_info)) == 0ull)
			{
				m_info += sizeof(size_type);
				m_node += sizeof(size_type);
			}

			size_type inc;
			if constexpr (endian::native == endian::little)
			{
				inc = count_trailing_zeros(n) / 8ull;
			}
			else
			{
				inc = count_leading_zeros(n) / 8ull;
			}

			m_info += inc;
			m_node += inc;
		}

		node_ptr m_node{nullptr};
		const uint8* m_info{nullptr};
	};

	template <typename MapType>
	using const_hash_map_iterator = hash_map_iterator<MapType, true>;


	template <typename KeyType, typename Hasher, bool hasIsTransparent = has_is_transparent<Hasher>::value>
	struct hasher_wrapper;

	template <typename KeyType, typename Hasher>
	struct hasher_wrapper<KeyType, Hasher, false>
	{
		Hasher hasher{};

		[[nodiscard]] [[rythe_always_inline]] constexpr id_type hash(const KeyType& val) const noexcept
		{
			id_type hash = hasher(val);
			hash *= 0xc4ceb9fe1a85ec53ull;
			hash ^= hash >> 33;
			return hash;
		}
	};

	template <typename KeyType, typename Hasher>
	struct hasher_wrapper<KeyType, Hasher, true>
	{
		using is_transparent = Hasher::is_transparent;

		Hasher hasher{};

		[[nodiscard]] [[rythe_always_inline]] constexpr id_type hash(const KeyType& val) const noexcept
		{
			id_type hash = hasher(val);
			hash *= 0xc4ceb9fe1a85ec53ull;
			hash ^= hash >> 33;
			return hash;
		}
	};

	template <typename KeyType, typename T>
	struct hasher_wrapper<KeyType, ::rsl::hash<T>, false>
	{
		::rsl::hash<T> hasher{};

		[[nodiscard]] [[rythe_always_inline]] constexpr id_type hash(const KeyType& val) const noexcept
		{
			return hasher(val);
		}
	};

	template <typename KeyType, typename T>
	struct hasher_wrapper<KeyType, fast_hash<T>, false>
	{
		fast_hash<T> hasher{};

		[[nodiscard]] [[rythe_always_inline]] constexpr id_type hash(const KeyType& val) const noexcept
		{
			return hasher(val);
		}
	};

	template <typename KeyType, typename T>
	struct hasher_wrapper<KeyType, protected_hash<T>, false>
	{
		protected_hash<T> hasher{};

		[[nodiscard]] [[rythe_always_inline]] constexpr id_type hash(const KeyType& val) const noexcept
		{
			return hasher(val);
		}
	};

	namespace internal
	{
		template <typename Key, typename Value, bool IsFlat>
		using map_value_type = typename conditional<
			is_void<Value>::value, Key, pair<typename conditional<IsFlat, Key, const Key>::type, Value>>::type;
	}

	// MaxLoadFactor could be a ratio instead
	template <
		typename Key, typename Value, typename Hash = ::rsl::hash<Key>, typename KeyEqual = equal<Key>,
		bool IsFlat = true, allocator_type Alloc = default_allocator,
		typed_factory_type FactoryType = default_factory<internal::map_value_type<Key, Value, IsFlat>>,
		ratio_type MaxLoadFactor = ::std::ratio<80, 100>, bool IsLarge = false,
		size_type FingerprintSize = internal::recommended_fingerprint_size<IsLarge>>
	struct map_info
	{
		constexpr static float32 max_load_factor = static_cast<float32>(MaxLoadFactor::num) / MaxLoadFactor::den;
		static_assert(max_load_factor > 0.1f && max_load_factor <= 0.99f, "MaxLoadFactor needs to be > 0.1 && < 0.99");

		using key_type = Key;
		using mapped_type = Value;

		constexpr static bool is_flat = IsFlat;

		using bucket_type = internal::hash_map_bucket<IsLarge, FingerprintSize>;
		using hasher_type = hasher_wrapper<key_type, Hash>;
		using key_comparer_type = KeyEqual;

		static constexpr bool is_map = !is_void<mapped_type>::value;
		static constexpr bool is_set = !is_map;
		static constexpr bool is_transparent =
			has_is_transparent<hasher_type>::value && has_is_transparent<key_comparer_type>::value;

		using value_type = internal::map_value_type<Key, Value, IsFlat>;

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

		static constexpr size_type max_load_factor = MapInfo::max_load_factor;

		using value_type = typename MapInfo::value_type;
		using node_type = internal::map_node<hash_map_base>;
		using bucket_type = typename MapInfo::bucket_type;

		using allocator_t = typename MapInfo::allocator_t;
		using allocator_storage_type = allocator_storage<allocator_t>;

		using factory_t = typename MapInfo::template factory_t<node_type>;
		using factory_storage_type = factory_storage<factory_t>;

		hash_map_base() noexcept(MapInfo::nothrow_constructible)
			: m_hasher(),
			  m_keyComparer(),
			  m_memoryPool()
		{
		}

		hash_map_base(const hasher_type& h, const key_comparer_type& equal)
			noexcept(MapInfo::nothrow_copy_constructible)
			: m_hasher(h),
			  m_keyComparer(equal),
			  m_memoryPool()
		{
		}

		explicit hash_map_base(const hasher_type& h) noexcept(MapInfo::nothrow_hasher_copy_constructible)
			: m_hasher(h),
			  m_keyComparer(),
			  m_memoryPool()
		{
		}

		explicit hash_map_base(const key_comparer_type& equal) noexcept(MapInfo::nothrow_comparer_copy_constructible)
			: m_hasher(),
			  m_keyComparer(equal),
			  m_memoryPool()
		{
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

	private:
		using data_pool = conditional_storage<!is_flat, memory_pool<value_type, allocator_t>>;
        using value_container = dynamic_array<node_type, allocator_t, factory_t>;
		using bucket_container = dynamic_array<bucket_type, allocator_t, typename MapInfo::template factory_t<bucket_type>>;

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

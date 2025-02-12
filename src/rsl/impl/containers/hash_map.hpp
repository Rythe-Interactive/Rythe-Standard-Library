#pragma once

#include "../math/basic/constraint.hpp"
#include "../memory/memory_pool.hpp"
#include "../util/hash.hpp"
#include "../util/utilities.hpp"
#include "iterators.hpp"
#include "pair.hpp"

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
				: m_data(::rsl::move(other.m_data))
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
			explicit flat_hash_map_node(map_type&, Args&&... args)
				noexcept(is_nothrow_constructible_v<value_type, Args...>)
				: m_data(forward<Args>(args)...)
			{
			}

			flat_hash_map_node(map_type&, flat_hash_map_node&& other)
				noexcept(is_nothrow_move_constructible_v<value_type>)
				: m_data(::rsl::move(other.m_data))
			{
			}

			void destroy(map_type&) noexcept {}

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
			value_type m_data;
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
	} // namespace internal

	struct fast_forward_tag_type
	{
	};

	[[maybe_unused]] constexpr fast_forward_tag_type fast_forward_tag{};

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

		hash_map_iterator(node_ptr node, const uint8* info, fast_forward_tag_type) noexcept
			: m_node(node),
			  m_info(info)
		{
			fast_forward();
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
			fast_forward();
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
		void fast_forward() noexcept
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

	template <typename KeyType, typename Hasher>
	struct hasher_wrapper
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

	template <typename KeyType, typename T>
	struct hasher_wrapper<KeyType, hash<T>>
	{
		hash<T> hasher{};

		[[nodiscard]] [[rythe_always_inline]] constexpr id_type hash(const KeyType& val) const noexcept
		{
			return hasher(val);
		}
	};

	template <typename KeyType, typename T>
	struct hasher_wrapper<KeyType, fast_hash<T>>
	{
		fast_hash<T> hasher{};

		[[nodiscard]] [[rythe_always_inline]] constexpr id_type hash(const KeyType& val) const noexcept
		{
			return hasher(val);
		}
	};

	template <typename KeyType, typename T>
	struct hasher_wrapper<KeyType, protected_hash<T>>
	{
		protected_hash<T> hasher{};

		[[nodiscard]] [[rythe_always_inline]] constexpr id_type hash(const KeyType& val) const noexcept
		{
			return hasher(val);
		}
	};

	// MaxLoadFactor100 could be a ratio instead
	//
	// info is PSL/wealth mixed with some specific hash???
	//
	// for us i don't think there's a use to a node map, if you can create a value stable map by using map<key,
	// unique_ptr<value>> I don't think we ever need a key stable map, and if/when we do, we can create a node map
	// later.

	template <typename Hash, typename KeyEqual, size_type MaxLoadFactor = 80>
	struct map_info
	{
		static_assert(MaxLoadFactor > 10 && MaxLoadFactor < 100, "MaxLoadFactor needs to be > 10 && < 100");

		constexpr static bool is_flat = true; // currently ignored, always flat.
		constexpr static size_type max_load_factor = MaxLoadFactor;
		using hasher_type = Hash;
		using key_comparer_type = KeyEqual;

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
	template <typename Key, typename Value, typename MapInfo>
	class hash_map_base
	{
	public:
		static constexpr bool is_flat = MapInfo::is_flat;
		using key_type = Key;
		using mapped_type = Value;
		using hasher_type = typename MapInfo::hasher_type;
		using key_comparer_type = typename MapInfo::key_comparer_type;

		static constexpr bool is_map = !is_void<Value>::value;
		static constexpr bool is_set = !is_map;
		static constexpr bool is_transparent =
			has_is_transparent<hasher_type>::value && has_is_transparent<key_comparer_type>::value;

		static constexpr size_type max_load_factor = MapInfo::max_load_factor;

		using value_type =
			typename conditional<is_set, Key, pair<typename conditional<is_flat, Key, Key const>::type, T>>::type;
		using node_type = internal::map_node<hash_map_base>;

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

	private:
		using data_pool = conditional_storage<is_flat, memory_pool<value_type>>;

		hasher_type m_hasher;
		key_comparer_type m_keyComparer;
		data_pool m_memoryPool;
	};
} // namespace rsl

#include "hash_map.inl"

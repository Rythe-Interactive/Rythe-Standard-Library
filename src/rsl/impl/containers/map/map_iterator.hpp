#pragma once

namespace rsl
{
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
			++m_info;
			++m_node;
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
			size_type n;
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
} // namespace rsl

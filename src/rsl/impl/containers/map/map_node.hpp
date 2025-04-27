#pragma once

namespace rsl::internal
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
		explicit hash_map_node(map_type& map, const key_type& key, Args&&... args) // NOLINT(cppcoreguidelines*)
			: m_data(map.get_memory_pool().allocate())
		{
			if constexpr (map_type::is_map)
			{
				m_data->first = key;
				map.get_factory().construct(&m_data->second, 1, rsl::forward<Args>(args)...);
			}
			else
			{
				*m_data = key;
			}
		}

		hash_map_node(hash_map_node&& other) noexcept // NOLINT(cppcoreguidelines*)
			: m_data(other.m_data) {}

		void destroy(map_type& map) noexcept
		{
			if constexpr (map_type::is_map)
			{
				map.get_factory().destroy(&m_data->second, 1);
				m_data->first.~key_type();
			}
			else
			{
				m_data->~value_type();
			}

			map.get_memory_pool().deallocate(m_data);
			m_data = nullptr;
		}

		~hash_map_node()
		{
			rsl_assert_invalid_operation(m_data == nullptr);
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

	#if defined(RYHTE_VALIDATE) && RYTHE_VALIDATION_LEVEL >= RYTHE_HIGH_IMPACT_VALIDATION_LEVEL
	#if !defined(RYTHE_VALIDATE_FLAT_MAP_NODE)
		#define RYTHE_VALIDATE_FLAT_MAP_NODE
	#endif
	#endif

	template <typename MapType>
	class flat_hash_map_node
	{
	public:
		using map_type = MapType;
		using key_type = typename MapType::key_type;
		using mapped_type = typename MapType::mapped_type;
		using value_type = typename MapType::value_type;

		template <typename... Args>
		explicit flat_hash_map_node(map_type& map, const key_type& key, Args&&... args) // NOLINT(cppcoreguidelines*)
			noexcept(is_nothrow_constructible_v<value_type, Args...>)
		{
			if constexpr (map_type::is_map)
			{
				m_data.first = key;
				map.get_factory().construct(&m_data.second, 1, rsl::forward<Args>(args)...);
			}
			else
			{
				m_data = key;
			}

			#if defined(RYTHE_VALIDATE_FLAT_MAP_NODE)
			m_holdsValue = true
			#endif
		}

		flat_hash_map_node(flat_hash_map_node&& other) // NOLINT(cppcoreguidelines*)
			noexcept(is_nothrow_move_constructible_v<value_type>)
			: m_data(rsl::move(other.m_data))
		{
			#if defined(RYTHE_VALIDATE_FLAT_MAP_NODE)
			m_holdsValue = other.m_holdsData;
			other.m_holdsData = false;
			#endif
		}

		~flat_hash_map_node()
		{
			#if defined(RYTHE_VALIDATE_FLAT_MAP_NODE)
			rsl_assert_invalid_operation(m_holdsValue);
			#endif
		}

		void destroy(map_type& map) noexcept
		{
			if constexpr (map_type::is_map)
			{
				map.get_factory().destroy(&m_data.second, 1);
				m_data.first.~key_type();
			}
			else
			{
				m_data.~value_type();
			}

			#if defined(RYTHE_VALIDATE_FLAT_MAP_NODE)
			m_holdsData = false;
			#endif
		}

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

		#if defined(RYTHE_VALIDATE_FLAT_MAP_NODE)
		bool m_holdsValue = false;
		#endif
	};

	template <typename MapType, bool IsFlat = false>
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


} // namespace rsl::internal

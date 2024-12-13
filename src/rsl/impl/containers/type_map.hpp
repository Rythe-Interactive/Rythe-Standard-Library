#pragma once
#include "../memory/allocator.hpp"
#include "../util/assert.hpp"
#include "../util/common.hpp"
#include "../util/type_traits.hpp"
#include "util/map_traits.hpp"

namespace rsl
{
	template <
		universal_allocator_type Alloc = default_allocator,
		template <typename> typename Constructor = default_constructor,
		template <typename...> typename MapType = std::unordered_map,
		typename Comparer = typename map_traits<MapType>::template default_comparer<id_type>>
	class basic_type_map
	{
	private:
		struct entry_item;

		using underlying_map_value_type = typename map_traits<MapType>::template value_type<id_type, entry_item>;
		using underlying_map_allocator = typename map_traits<MapType>::template allocator_type<
			id_type, entry_item, Alloc, Constructor<underlying_map_value_type>>;

	public:
		using universal_alloc = Alloc;
		template <typename T>
		using alloc_type = allocator<T, universal_alloc, Constructor<T>>;
		using underlying_map = typename map_traits<MapType>::template customized_type<
			id_type, entry_item, Comparer, Alloc, Constructor<underlying_map_value_type>>;

		constexpr basic_type_map() = default;

		explicit constexpr basic_type_map(const universal_alloc& alloc)
			noexcept(is_nothrow_copy_constructible_v<universal_alloc>)
			requires nothrow_copy_constructible<universal_alloc>
			: m_allocator(alloc),
			  m_storage(underlying_map_allocator(m_allocator))
		{
		}

		[[nodiscard]] [[rythe_always_inline]] constexpr size_type size() const noexcept { return m_storage.size(); }
		[[nodiscard]] [[rythe_always_inline]] constexpr bool empty() const noexcept { return m_storage.empty(); }

		template <typename T>
		[[nodiscard]] [[rythe_always_inline]] constexpr bool has() const noexcept
		{
			constexpr id_type typeHash = type_id<T>();
			return m_storage.contains(typeHash);
		}

		template <typename T>
		[[nodiscard]] [[rythe_always_inline]] constexpr const T* try_get() const noexcept
		{
			constexpr id_type typeHash = type_id<T>();
			if (auto iter = m_storage.find(typeHash); iter != m_storage.end())
			{
				return iter->second.template cast<const T>();
			}

			return nullptr;
		}

		template <typename T>
		[[nodiscard]] [[rythe_always_inline]] constexpr T* try_get() noexcept
		{
			return const_cast<T*>(rsl::as_const(*this).template try_get<T>());
		}

		template <typename T>
		[[nodiscard]] [[rythe_always_inline]] constexpr const T& get() const
		{
			auto* ptr = try_get<T>();
			rsl_assert_invalid_object(ptr);
			return *ptr;
		}

		template <typename T>
		[[nodiscard]] [[rythe_always_inline]] constexpr T& get()
		{
			return const_cast<T&>(rsl::as_const(*this).template get<T>());
		}

		template <typename T, typename... Args>
		[[rythe_always_inline]] constexpr std::pair<T&, bool> try_emplace(Args&&... args) noexcept
		{
			constexpr id_type typeHash = type_id<T>();
			auto result = m_storage.try_emplace(typeHash);
			if (result.second)
			{
				result.first->second.template construct<T>(this, forward<Args>(args)...);
			}

			return std::make_pair(std::ref(*(result.first->second.template cast<T>())), result.second);
		}

		template <typename T, typename... Args>
		[[rythe_always_inline]] constexpr T& emplace(Args&&... args)
		{
			return try_emplace<T>(std::forward<Args>(args)...).first;
		}

		template <typename T, typename... Args>
		[[nodiscard]] [[rythe_always_inline]] constexpr T& get_or_emplace(Args&&... args) noexcept
		{
			if (T* ptr = try_get<T>(); ptr != nullptr)
			{
				return *ptr;
			}

			return emplace<T>(forward<Args>(args)...);
		}

		template <typename T, typename... Args>
		[[rythe_always_inline]] constexpr T& emplace_or_replace(Args&&... args) noexcept
		{
			erase<T>();
			return emplace<T>(forward<Args>(args)...);
		}

		template <typename T>
		[[rythe_always_inline]] constexpr void erase() noexcept
		{
			constexpr id_type typeHash = type_id<T>();
			m_storage.erase(typeHash);
		}

		[[rythe_always_inline]] constexpr void clear() noexcept { m_storage.clear(); }

		[[rythe_always_inline]] constexpr universal_alloc& get_allocator() noexcept { return m_allocator; }
		[[rythe_always_inline]] constexpr const universal_alloc& get_allocator() const noexcept { return m_allocator; }

	private:
		template <typename T>
		static void deallocate_entry(basic_type_map& map, entry_item& entry)
		{
			alloc_type<T>(map.m_allocator).deallocate(entry.template cast<T>());
			entry.data = nullptr;
			entry.map = nullptr;
			entry.deallocate_entry = nullptr;
		}

		struct entry_item
		{
			using deallocate_func = void (*)(basic_type_map&, entry_item&);
			void* data = nullptr;
			basic_type_map* map = nullptr;
			deallocate_func deallocate_entry = nullptr;

			[[rythe_always_inline]] constexpr entry_item() noexcept = default;
			[[rythe_always_inline]] constexpr entry_item(entry_item&& other) noexcept
				: data(other.data),
				  map(other.map),
				  deallocate_entry(other.deallocate_entry)
			{
				other.data = nullptr;
				other.map = nullptr;
				other.deallocate_entry = nullptr;
			}

			[[rythe_always_inline]] constexpr entry_item& operator=(entry_item&& other) noexcept
			{
				data = other.data;
				map = other.map;
				deallocate_entry = other.deallocate_entry;
				other.data = nullptr;
				other.map = nullptr;
				other.deallocate_entry = nullptr;
				return *this;
			}

			template <typename T, typename... Args>
			void construct(basic_type_map* mapPtr, Args&&... args)
			{
				data = alloc_type<T>(mapPtr->m_allocator).allocate(1, forward<Args>(args)...);
				map = mapPtr;
				deallocate_entry = &basic_type_map::deallocate_entry<T>;
			}

			template <typename T>
			T* cast() const
			{
				return static_cast<T*>(data);
			}

			~entry_item()
			{
				if (data)
				{
					rsl_assert_rarely(map);
					(*deallocate_entry)(*map, *this);
				}
			}
		};

		universal_alloc m_allocator;
		underlying_map m_storage;
	};

	using type_map = basic_type_map<>;

	using pmu_alloc_type_map = basic_type_map<pmu_alloc_ptr_wrapper>;
} // namespace rsl

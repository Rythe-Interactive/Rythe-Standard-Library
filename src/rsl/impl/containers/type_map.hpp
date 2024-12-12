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
		using univeral_alloc = Alloc;
		template <typename T>
		using alloc_type = allocator<T, univeral_alloc, Constructor<T>>;
		using underlying_map = typename map_traits<MapType>::template customized_type<
			id_type, entry_item, Comparer, Alloc, Constructor<underlying_map_value_type>>;

		constexpr basic_type_map() = default;

		explicit constexpr basic_type_map(const univeral_alloc& alloc)
			noexcept(is_nothrow_copy_constructible_v<univeral_alloc>)
			requires nothrow_copy_constructible<univeral_alloc>
			: m_allocator(alloc),
			  m_storage(underlying_map_allocator(m_allocator))
		{
		}

		template <typename T>
		bool has() const
		{
			constexpr static id_type typeHash = type_id<T>();
			return m_storage.contains(typeHash);
		}

		template <typename T>
		const T* try_get() const
		{
			constexpr static id_type typeHash = type_id<T>();
			if (auto iter = m_storage.find(typeHash); iter != m_storage.end())
			{
				return iter->second.template cast<const T>();
			}

			return nullptr;
		}

		template <typename T>
		T* try_get()
		{
			return const_cast<T*>(rsl::as_const(*this).template try_get<T>());
		}

		template <typename T>
		const T& get() const
		{
			auto* ptr = try_get<T>();
			rsl_assert_invalid_object(ptr);
			return *ptr;
		}

		template <typename T>
		T& get()
		{
			return const_cast<T&>(rsl::as_const(*this).template get<T>());
		}

		template <typename T, typename... Args>
		std::pair<T&, bool> try_emplace(Args&&... args)
		{
			constexpr static id_type typeHash = type_id<T>();
			auto result = m_storage.try_emplace(typeHash);
			if (result.second)
			{
				entry_item& entry = result.first->second;
				entry.data = alloc_type<T>(m_allocator).allocate(1, forward<Args>(args)...);
			}

			return std::make_pair(std::ref(*(result.first->second.template cast<T>())), result.second);
		}

		template <typename T, typename... Args>
		T& emplace(Args&&... args)
		{
			return try_emplace<T>(std::forward<Args>(args)...).first;
		}

		template <typename T, typename... Args>
		T& get_or_emplace(Args&&... args)
		{
			if (T* ptr = try_get<T>(); ptr != nullptr)
			{
				return *ptr;
			}

			return emplace<T>(forward<Args>(args)...);
		}

		template <typename T, typename... Args>
		T& emplace_or_replace(Args&&... args)
		{
			erase<T>();
			return emplace<T>(forward<Args>(args)...);
		}

		template <typename T>
		void erase()
		{
			constexpr u64 typeHash = type_hash<T>();
			if (auto iter = m_storage.find(typeHash); iter != m_storage.end())
			{
				alloc_type<T>(m_allocator).deallocate(1, iter->second.data.template cast<T>());
				m_storage.erase(typeHash);
			}
		}

	private:
		struct entry_item
		{
			void* data;

			template <typename T>
			T* cast() const
			{
				return static_cast<T*>(data);
			}
		};

		univeral_alloc m_allocator;
		underlying_map m_storage;
	};

	using type_map = basic_type_map<>;

    using pmu_alloc_type_map = basic_type_map<pmu_alloc_ptr_wrapper>;
} // namespace rsl

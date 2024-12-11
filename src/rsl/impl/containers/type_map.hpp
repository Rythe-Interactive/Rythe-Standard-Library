#pragma once
#include "../memory/allocator.hpp"
#include "../util/assert.hpp"
#include "../util/common.hpp"
#include "../util/type_traits.hpp"
#include "util/map_traits.hpp"

namespace rsl
{
	template <typename Alloc = allocator<id_type>,
		template <typename...> typename MapType = std::unordered_map, typename Comparer = map_traits<MapType>::default_comparer<id_type>>
	class basic_type_map
	{
	private:
		struct entry_item;

	public:
		using univeral_alloc = Alloc::universal_type;
        template<typename T>
        using alloc_type = typename Alloc::template retarget<T>;
		using underlying_map = typename map_traits<MapType>::template customized_type<id_type, entry_item, Comparer, Alloc>;

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
			return const_cast<T*>(as_const(*this).template try_get<T>());
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
			auto* ptr = try_get<T>();
			rsl_assert_invalid_object(ptr);
			return *ptr;
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
} // namespace rsl

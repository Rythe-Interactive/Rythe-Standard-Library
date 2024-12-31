#pragma once

#include "dynamic_array.hpp"

namespace rsl
{
	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	inline constexpr dynamic_array<T, Alloc, Factory>::dynamic_array(const dynamic_array& src)
		noexcept(container_base::copy_construct_noexcept)
		: container_base(src)
	{
		mem_rsc::allocate(src.m_size);
		container_base::m_size = src.m_size;
		container_base::m_capacity = src.m_size;
		container_base::copy_construct_from_unsafe_impl(0, container_base::m_size, src.get_ptr());
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	inline constexpr dynamic_array<T, Alloc, Factory>::dynamic_array(dynamic_array&& src) noexcept
		: container_base(move(src))
	{
		src.set_ptr(nullptr);
		src.m_size = 0;
		src.m_capacity = 0;
	}

    template <typename T, allocator_type Alloc, typed_factory_type Factory>
	inline constexpr dynamic_array<T, Alloc, Factory>::dynamic_array(const allocator_storage_type& allocStorage)
		noexcept(is_nothrow_constructible_v<container_base, const allocator_storage_type&>)
		: container_base(allocStorage)
	{
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	inline constexpr dynamic_array<T, Alloc, Factory>::dynamic_array(const factory_storage_type& factoryStorage)
		noexcept(is_nothrow_constructible_v<container_base, const factory_storage_type&>)
		: container_base(factoryStorage)
	{
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	inline constexpr dynamic_array<T, Alloc, Factory>::dynamic_array(
		const allocator_storage_type& allocStorage, const factory_storage_type& factoryStorage
	) noexcept(is_nothrow_constructible_v<container_base, const allocator_storage_type&, const factory_storage_type&>)
		: container_base(allocStorage, factoryStorage)
	{
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	template <size_type N>
	inline constexpr dynamic_array<T, Alloc, Factory>::dynamic_array(const value_type (&arr)[N])
		noexcept(container_base::copy_construct_noexcept)
	{
		mem_rsc::allocate(N);
		container_base::m_size = N;
		container_base::m_capacity = N;
		container_base::copy_construct_from_unsafe_impl(0, container_base::m_size, arr);
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	template <size_type N>
	inline constexpr dynamic_array<T, Alloc, Factory>::dynamic_array(value_type (&&arr)[N])
		noexcept(container_base::move_construct_noexcept)
	{
		mem_rsc::allocate(N);
		container_base::m_size = N;
		container_base::m_capacity = N;
		container_base::move_construct_from_unsafe_impl(0, container_base::m_size, arr);
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	inline constexpr dynamic_array<T, Alloc, Factory>::dynamic_array(view_type src)
		noexcept(container_base::copy_construct_noexcept)
	{
		mem_rsc::allocate(src.size());
		container_base::m_size = src.size();
		container_base::m_capacity = src.size();
		container_base::copy_construct_from_unsafe_impl(0, container_base::m_size, src.data());
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	inline constexpr dynamic_array<T, Alloc, Factory>::dynamic_array(size_type capacity) noexcept
	{
		mem_rsc::allocate(capacity);
		container_base::m_size = 0;
		container_base::m_capacity = capacity;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	template <typename... Args>
	inline constexpr dynamic_array<T, Alloc, Factory>::dynamic_array(
		size_type count, in_place_signal_type, Args&&... args
	) noexcept(container_base::template construct_noexcept<Args...>)
	{
		mem_rsc::allocate(count);
		container_base::m_size = count;
		container_base::m_capacity = count;
		mem_rsc::construct(count, 0, forward<Args>(args)...);
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	inline constexpr dynamic_array<T, Alloc, Factory>&
	dynamic_array<T, Alloc, Factory>::operator=(const dynamic_array& src)
		noexcept(container_base::copy_assign_noexcept && container_base::copy_construct_noexcept)
	{
		copy_assign_impl(src.get_ptr(), src.size(), mem_rsc::m_alloc == src.m_alloc ? nullptr : &src.m_alloc);
		return *this;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	inline constexpr dynamic_array<T, Alloc, Factory>& dynamic_array<T, Alloc, Factory>::operator=(dynamic_array&& src
	) noexcept
	{
		container_base::operator=(move(src));
		src.set_ptr(nullptr);
		src.m_size = 0;
		src.m_capacity = 0;
		return *this;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	template <size_type N>
	inline constexpr dynamic_array<T, Alloc, Factory>&
	dynamic_array<T, Alloc, Factory>::operator=(const value_type (&arr)[N])
		noexcept(container_base::copy_assign_noexcept && container_base::copy_construct_noexcept)
	{
		copy_assign_impl(arr, N);
		return *this;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	template <size_type N>
	inline constexpr dynamic_array<T, Alloc, Factory>& dynamic_array<T, Alloc, Factory>::operator=(value_type (&&arr)[N]
	) noexcept(container_base::move_assign_noexcept && container_base::move_construct_noexcept)
	{
		move_data_assign_impl(arr, N);
		return *this;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	inline constexpr dynamic_array<T, Alloc, Factory>& dynamic_array<T, Alloc, Factory>::operator=(view_type src)
		noexcept(container_base::copy_assign_noexcept && container_base::copy_construct_noexcept)
	{
		copy_assign_impl(src.data(), src.size());
		return *this;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	template <typename... Args>
	inline constexpr void dynamic_array<T, Alloc, Factory>::resize(size_type newSize, Args&&... args)
		noexcept(container_base::template construct_noexcept<Args...> && container_base::move_construct_noexcept)
	{
		if (newSize > container_base::m_capacity)
		{
			reserve(newSize);
		}

		if (newSize > container_base::m_size)
		{
			container_base::emplace_unsafe_impl(container_base::m_size, newSize, forward<Args>(args)...);
		}
		else if (newSize < container_base::m_size)
		{
			container_base::reset_unsafe_impl(newSize);
		}

		container_base::m_size = newSize;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	inline constexpr void dynamic_array<T, Alloc, Factory>::reserve(size_type newCapacity)
		noexcept(container_base::move_construct_noexcept)
	{
		if constexpr (is_trivially_copyable_v<T>)
		{
			mem_rsc::reallocate(container_base::m_capacity, newCapacity);
		}
		else
		{
			T* newMem = mem_rsc::m_alloc.allocate(newCapacity);
			if (newMem)
			{
				mem_rsc::m_alloc.move(newMem, mem_rsc::m_ptr, container_base::m_size);
			}

			mem_rsc::deallocate(container_base::m_capacity);
			mem_rsc::m_ptr = newMem;
		}

		container_base::m_capacity = newCapacity;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	inline constexpr void dynamic_array<T, Alloc, Factory>::push_back(const value_type& value)
		noexcept(container_base::move_construct_noexcept)
	{
		maybe_grow();
		container_base::emplace_unsafe_impl(container_base::m_size, container_base::m_size + 1, value);
		container_base::m_size++;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	inline constexpr void dynamic_array<T, Alloc, Factory>::push_back(value_type&& value)
		noexcept(container_base::move_construct_noexcept)
	{
		maybe_grow();
		container_base::emplace_unsafe_impl(container_base::m_size, container_base::m_size + 1, move(value));
		container_base::m_size++;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	template <typename... Args>
	inline constexpr T& dynamic_array<T, Alloc, Factory>::emplace_back(Args&&... args)
		noexcept(container_base::template construct_noexcept<Args...> && container_base::move_construct_noexcept)
	{
		maybe_grow();
		container_base::emplace_unsafe_impl(container_base::m_size, container_base::m_size + 1, forward<Args>(args)...);
		container_base::m_size++;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	template <typename Comp>
	inline constexpr dynamic_array<T, Alloc, Factory>::iterator
	dynamic_array<T, Alloc, Factory>::erase(Comp&& comparable)
		noexcept(container_base::move_construct_noexcept)
	{
		return erase([&](const_iterator iter) { return (*iter) == comparable });
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	template <invocable<bool(dynamic_array<T, Alloc, Factory>::const_iterator)> Func>
	inline constexpr dynamic_array<T, Alloc, Factory>::iterator dynamic_array<T, Alloc, Factory>::erase(Func&& comparer)
		noexcept(container_base::move_construct_noexcept)
	{
        for (const_iterator iter = cbegin(); iter != cend(); ++iter)
        {
            if (comparer(iter))
            {
				return erase(iter);
            }
        }

        return end();
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	inline constexpr void dynamic_array<T, Alloc, Factory>::pop_back(size_type count) noexcept
	{
		if (count > container_base::m_size)
		{
			count = container_base::m_size;
		}

		container_base::reset_unsafe_impl(container_base::m_size - count);
		container_base::m_size -= count;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	inline constexpr void dynamic_array<T, Alloc, Factory>::clear() noexcept
	{
		container_base::reset_unsafe_impl();
		container_base::m_size = 0;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	inline constexpr void dynamic_array<T, Alloc, Factory>::maybe_grow()
		noexcept(container_base::move_construct_noexcept)
	{
		if (container_base::m_size == container_base::m_capacity)
		{
			reserve(container_base::m_capacity * 2);
		}
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	inline constexpr void dynamic_array<T, Alloc, Factory>::copy_assign_impl(
		const value_type* src, size_type srcSize, mem_rsc::typed_alloc_type* alloc
	) noexcept(container_base::copy_assign_noexcept && container_base::copy_construct_noexcept)
	{
		if (srcSize > container_base::m_capacity || alloc != nullptr)
		{
			if (mem_rsc::m_ptr)
			{
				container_base::reset_unsafe_impl();
				mem_rsc::deallocate(container_base::m_capacity);
				container_base::m_capacity = 0;
				container_base::m_size = 0;
			}

			rsl_assert_frequent(container_base::m_size == 0);
			rsl_assert_frequent(container_base::m_capacity == 0);

			if (alloc)
			{
				mem_rsc::m_alloc = *alloc;
			}

			mem_rsc::allocate(srcSize);
			container_base::m_capacity = srcSize;
		}

		if (container_base::m_size > srcSize)
		{
			container_base::reset_unsafe_impl(srcSize);
			container_base::m_size = srcSize;
		}

		if (container_base::m_size > 0)
		{
			container_base::copy_assign_from_unsafe_impl(0, container_base::m_size, src);
		}

		if (container_base::m_size < srcSize)
		{
			container_base::copy_construct_from_unsafe_impl(
				container_base::m_size, srcSize, src + container_base::m_size
			);
			container_base::m_size = srcSize;
		}
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	inline constexpr void
	dynamic_array<T, Alloc, Factory>::move_data_assign_impl(const value_type* src, size_type srcSize)
		noexcept(container_base::move_assign_noexcept && container_base::move_construct_noexcept)
	{
		if (srcSize > container_base::m_capacity)
		{
			if (mem_rsc::m_ptr)
			{
				container_base::reset_unsafe_impl();
				mem_rsc::deallocate(container_base::m_capacity);
				container_base::m_capacity = 0;
				container_base::m_size = 0;
			}

			rsl_assert_frequent(container_base::m_size == 0);
			rsl_assert_frequent(container_base::m_capacity == 0);

			mem_rsc::allocate(srcSize);
			container_base::m_capacity = srcSize;
		}

		if (container_base::m_size > srcSize)
		{
			container_base::reset_unsafe_impl(srcSize);
			container_base::m_size = srcSize;
		}

		if (container_base::m_size > 0)
		{
			container_base::move_assign_from_unsafe_impl(0, container_base::m_size, src);
		}

		if (container_base::m_size < srcSize)
		{
			container_base::move_construct_from_unsafe_impl(
				container_base::m_size, srcSize, src + container_base::m_size
			);
			container_base::m_size = srcSize;
		}
	}
} // namespace rsl

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
		if (newCapacity < container_base::m_capacity)
		{
			return;
		}

		if constexpr (is_trivially_copyable_v<T>)
		{
			mem_rsc::reallocate(container_base::m_capacity, newCapacity);
		}
		else
		{
			T* newMem = mem_rsc::m_alloc.allocate(newCapacity);
			if (newMem)
			{
				mem_rsc::m_alloc.move(newMem, mem_rsc::get_ptr(), container_base::m_size);
			}

			mem_rsc::deallocate(container_base::m_capacity);
			mem_rsc::set_ptr(newMem);
		}

		container_base::m_capacity = newCapacity;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	inline constexpr void dynamic_array<T, Alloc, Factory>::push_back(const value_type& value)
		noexcept(container_base::copy_construct_noexcept)
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
	inline constexpr void dynamic_array<T, Alloc, Factory>::assign(size_type count, const value_type& value)
	{
		clear();
		insert(0, count, value);
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	template <input_iterator InputIt>
	inline constexpr void dynamic_array<T, Alloc, Factory>::assign(InputIt first, InputIt last)
	{
		clear();
		insert(0, first, last);
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	inline constexpr dynamic_array<T, Alloc, Factory>::iterator_type
	dynamic_array<T, Alloc, Factory>::iterator_at(size_type i) noexcept
	{
		return container_base::begin() + i;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	inline constexpr dynamic_array<T, Alloc, Factory>::const_iterator_type
	dynamic_array<T, Alloc, Factory>::iterator_at(size_type i) const noexcept
	{
		return container_base::begin() + i;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	inline constexpr size_type dynamic_array<T, Alloc, Factory>::insert(size_type pos, const value_type& value)
		noexcept(container_base::move_construct_noexcept && container_base::copy_construct_noexcept)
	{
		split_reserve(
			pos, 1,
			container_base::m_size == container_base::m_capacity ? container_base::m_capacity * 2
																 : container_base::m_size + 1
		);

		mem_rsc::construct(1, pos, value);

		return pos;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	inline constexpr size_type dynamic_array<T, Alloc, Factory>::insert(size_type pos, value_type&& value)
		noexcept(container_base::move_construct_noexcept)
	{
		split_reserve(
			pos, 1,
			container_base::m_size == container_base::m_capacity ? container_base::m_capacity * 2
																 : container_base::m_size + 1);

		mem_rsc::construct(1, pos, move(value));

		return pos;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	inline constexpr size_type
	dynamic_array<T, Alloc, Factory>::insert(size_type pos, size_type count, const value_type& value)
		noexcept(container_base::move_construct_noexcept && container_base::copy_construct_noexcept)
	{
		split_reserve(pos, count, container_base::m_size + count);

        mem_rsc::construct(count, pos, value);

		return pos;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	template <input_iterator InputIt>
	inline constexpr size_type dynamic_array<T, Alloc, Factory>::insert(
		size_type pos, InputIt first, InputIt last
	) noexcept(container_base::move_construct_noexcept && container_base::template construct_noexcept<iter_value_t<InputIt>>)
	{
		size_type count = iterator_diff(first, last);
		split_reserve(pos, count, container_base::m_size + count);

		size_type i = pos;
		for (InputIt iter = first; iter != last; iter++, i++)
		{
			mem_rsc::construct(1, i, *iter);
		}

		return pos;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	inline constexpr size_type dynamic_array<T, Alloc, Factory>::erase(size_type pos)
		noexcept(container_base::move_construct_noexcept)
	{
		mem_rsc::destroy(1, pos);
		container_base::move_shift_elements_unsafe(pos + 1, npos, -1ll);
		container_base::m_size--;
		return pos;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	inline constexpr size_type dynamic_array<T, Alloc, Factory>::erase(size_type first, size_type last)
		noexcept(container_base::move_construct_noexcept)
	{
		if (last > container_base::m_size)
		{
			last = container_base::m_size;
		}

		size_type count = last - first;
		mem_rsc::destroy(count, first);
		container_base::move_shift_elements_unsafe(last, npos, -static_cast<diff_type>(count));
		container_base::m_size -= count;
		return last - count;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	template <typename Comp>
	inline constexpr size_type dynamic_array<T, Alloc, Factory>::erase(Comp&& comparable)
		noexcept(container_base::move_construct_noexcept)
	{
		return erase([&](const_iterator_type iter) { return (*iter) == comparable; });
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	template <typename Func>
	inline constexpr size_type dynamic_array<T, Alloc, Factory>::erase(Func&& comparer)
		noexcept(container_base::move_construct_noexcept)
		requires invocable<Func, bool(const_iterator_type)>
	{
		size_type eraseLocation = npos;
		diff_type shift = -1ll;

		const size_type originalSize = container_base::m_size;

		for (size_type i = 0; i < originalSize; i++)
		{
			if (comparer(iterator_at(iter)))
			{
				if (eraseLocation != npos)
				{
					mem_rsc::destroy(1, eraseLocation);
					container_base::move_shift_elements_unsafe(pos + 1, i, shift);
					container_base::m_size--;
					shift -= 1ll;
				}

				eraseLocation = i;
			}
		}

		if (eraseLocation != npos)
		{
			mem_rsc::destroy(1, eraseLocation);
			container_base::move_shift_elements_unsafe(pos + 1, npos, shift);
			container_base::m_size--;
			return eraseLocation;
		}

		return npos;
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
			if (mem_rsc::get_ptr())
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
			if (mem_rsc::get_ptr())
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

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	inline constexpr void
	dynamic_array<T, Alloc, Factory>::split_reserve(size_type pos, size_type count, size_type newSize)
		noexcept(container_base::move_construct_noexcept)
	{
		if (newSize > container_base::m_capacity)
		{
			if constexpr (is_trivially_copyable_v<T>)
			{
				mem_rsc::reallocate(container_base::m_capacity, newSize);
				container_base::move_shift_elements_unsafe(pos, npos, count);
			}
			else
			{
				T* newMem = mem_rsc::m_alloc.allocate(newSize);
				if (newMem)
				{
					mem_rsc::m_alloc.move(newMem, mem_rsc::get_ptr(), pos);
					mem_rsc::m_alloc.move(newMem + pos, container_base::get_ptr_at(pos), pos + count);
				}

				mem_rsc::deallocate(container_base::m_capacity);
				mem_rsc::set_ptr(newMem);
			}

			container_base::m_capacity = newSize;
		}
		else
		{
			container_base::move_shift_elements_unsafe(pos, npos, count);
		}

        container_base::m_size = newSize;
	}
} // namespace rsl

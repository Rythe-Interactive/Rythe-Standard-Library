#pragma once

namespace rsl
{
	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	constexpr dynamic_array<T, Alloc, Factory, UsePostFix>::dynamic_array(const dynamic_array& src)
		noexcept(container_base::copy_construct_noexcept)
		: container_base(src)
	{
		size_type newMemorySize;
		if constexpr (UsePostFix)
		{
			newMemorySize = src.m_size + 1ull;
		}
		else
		{
			newMemorySize = src.m_size;
		}

		mem_rsc::allocate(newMemorySize);
		container_base::m_size = src.m_size;
		container_base::m_capacity = src.m_size;
		container_base::copy_construct_from_unsafe_impl(0ull, newMemorySize, src.get_ptr());
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	constexpr dynamic_array<T, Alloc, Factory, UsePostFix>::dynamic_array(dynamic_array&& src) noexcept
		: container_base(rsl::move(src))
	{
		src.set_ptr(nullptr);
		src.m_size = 0ull;
		src.m_capacity = 0ull;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	constexpr dynamic_array<T, Alloc, Factory, UsePostFix>::dynamic_array(const allocator_storage_type& allocStorage)
		noexcept(is_nothrow_constructible_v<container_base, const allocator_storage_type&>)
		: container_base(allocStorage) {}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	constexpr dynamic_array<T, Alloc, Factory, UsePostFix>::dynamic_array(const factory_storage_type& factoryStorage)
		noexcept(is_nothrow_constructible_v<container_base, const factory_storage_type&>)
		: container_base(factoryStorage) {}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	constexpr dynamic_array<T, Alloc, Factory, UsePostFix>::dynamic_array(
		const allocator_storage_type& allocStorage, const factory_storage_type& factoryStorage
	) noexcept(is_nothrow_constructible_v<container_base, const allocator_storage_type&, const factory_storage_type&>)
		: container_base(allocStorage, factoryStorage) {}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	template <size_type N>
	constexpr dynamic_array<T, Alloc, Factory, UsePostFix> dynamic_array<T, Alloc, Factory, UsePostFix>::from_array(
		const value_type (&arr)[N])
		noexcept(container_base::copy_construct_noexcept)
	{
		dynamic_array result;
		result.reserve(N);
		result.m_size = N;
		result.m_capacity = result.m_size;
		result.copy_construct_from_unsafe_impl(0ull, result.m_size, &arr[0]);

		if constexpr (UsePostFix)
		{
			result.construct(1ull, result.m_size);
		}

		return result;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	template <size_type N>
	constexpr dynamic_array<T, Alloc, Factory, UsePostFix> dynamic_array<T, Alloc, Factory, UsePostFix>::from_array(
		value_type (&&arr)[N])
		noexcept(container_base::move_construct_noexcept)
	{
		dynamic_array result;
		result.reserve(N);
		result.m_size = N;
		result.m_capacity = result.m_size;
		result.move_construct_from_unsafe_impl(0ull, result.m_size, &arr[0]);

		if constexpr (UsePostFix)
		{
			result.construct(1ull, result.m_size);
		}

		return result;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	constexpr dynamic_array<T, Alloc, Factory, UsePostFix> dynamic_array<T, Alloc, Factory, UsePostFix>::from_buffer(
		const value_type* ptr,
		size_type count) noexcept(container_base::copy_construct_noexcept)
	{
		dynamic_array result;
		result.reserve(count);
		result.m_size = count;
		result.m_capacity = result.m_size;
		result.copy_construct_from_unsafe_impl(0ull, result.m_size, ptr);

		if constexpr (UsePostFix)
		{
			result.construct(1ull, result.m_size);
		}

		return result;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	constexpr dynamic_array<T, Alloc, Factory, UsePostFix> dynamic_array<T, Alloc, Factory, UsePostFix>::from_view(
		const_view_type src) noexcept(container_base::copy_construct_noexcept)
	{
		dynamic_array result;
		result.reserve(src.size());
		result.m_size = src.size();
		result.m_capacity = result.m_size;
		result.copy_construct_from_unsafe_impl(0ull, result.m_size, src.data());

		if constexpr (UsePostFix)
		{
			result.construct(1ull, result.m_size);
		}

		return result;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	constexpr dynamic_array<T, Alloc, Factory, UsePostFix> dynamic_array<T, Alloc, Factory, UsePostFix>::create_reserved(
		const size_type capacity)
		noexcept
	{
		dynamic_array result;
		result.reserve(capacity);

		return result;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	template <typename... Args>
	constexpr dynamic_array<T, Alloc, Factory, UsePostFix> dynamic_array<T, Alloc, Factory, UsePostFix>::create_in_place(
		size_type count, Args&&... args)
		noexcept(container_base::template construct_noexcept<Args...>)
	{
		dynamic_array result;
		result.reserve(count);
		result.m_size = count;
		result.m_capacity = count;
		result.construct(count, 0ull, rsl::forward<Args>(args)...);

		if constexpr (UsePostFix)
		{
			result.construct(1ull, result.m_size);
		}

		return result;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	constexpr dynamic_array<T, Alloc, Factory, UsePostFix>&
	dynamic_array<T, Alloc, Factory, UsePostFix>::operator=(const dynamic_array& src)
		noexcept(container_base::copy_assign_noexcept && container_base::copy_construct_noexcept)
	{
		copy_assign_impl(src.get_ptr(), src.m_size, mem_rsc::m_alloc == src.m_alloc ? nullptr : &src.m_alloc);
		return *this;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	constexpr dynamic_array<T, Alloc, Factory, UsePostFix>& dynamic_array<T, Alloc, Factory, UsePostFix>::operator=(
		dynamic_array&& src
	) noexcept
	{
		container_base::operator=(move(src));
		src.set_ptr(nullptr);
		src.m_size = 0ull;
		src.m_capacity = 0ull;
		return *this;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	template <size_type N>
	constexpr dynamic_array<T, Alloc, Factory, UsePostFix>&
	dynamic_array<T, Alloc, Factory, UsePostFix>::operator=(const value_type (&arr)[N])
		noexcept(container_base::copy_assign_noexcept && container_base::copy_construct_noexcept)
	{
		copy_assign_impl(arr, N);
		return *this;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	template <size_type N>
	constexpr dynamic_array<T, Alloc, Factory, UsePostFix>& dynamic_array<T, Alloc, Factory, UsePostFix>::operator=(
		value_type (&&arr)[N]
	) noexcept(container_base::move_assign_noexcept && container_base::move_construct_noexcept)
	{
		move_data_assign_impl(arr, N);
		return *this;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	constexpr dynamic_array<T, Alloc, Factory, UsePostFix>& dynamic_array<T, Alloc, Factory, UsePostFix>::operator=(
		view_type src)
		noexcept(container_base::copy_assign_noexcept && container_base::copy_construct_noexcept)
	{
		copy_assign_impl(src.data(), src.size());
		return *this;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	inline constexpr bool dynamic_array<T, Alloc, Factory, UsePostFix>::operator==(
		const dynamic_array<T, Alloc, Factory, UsePostFix>& rhs)
	{
		bool result = true;
		for (size_type i = 0; i < this->size(); ++i)
		{
			result &= this->at(i) == rhs.at(i);
		}

		return result;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	inline constexpr bool dynamic_array<T, Alloc, Factory, UsePostFix>::operator!=(
		const dynamic_array<T, Alloc, Factory, UsePostFix>& rhs)
	{
		return !(*this == rhs);
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	template <typename... Args>
	constexpr void dynamic_array<T, Alloc, Factory, UsePostFix>::resize(size_type newSize, Args&&... args)
		noexcept(container_base::template construct_noexcept<Args...> && container_base::move_construct_noexcept)
	{
		if constexpr (UsePostFix)
		{
			if (container_base::m_capacity > 0ull)
			{
				container_base::destroy(1ull, container_base::m_size);
			}
		}

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

		if constexpr (UsePostFix)
		{
			container_base::construct(1ull, container_base::m_size);
		}
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	constexpr void dynamic_array<T, Alloc, Factory, UsePostFix>::reserve(size_type newCapacity)
		noexcept(container_base::move_construct_noexcept)
	{
		if (newCapacity < container_base::m_capacity)
		{
			return;
		}

		size_type newMemorySize;
		size_type oldMemorySize;
		if constexpr (UsePostFix)
		{
			newMemorySize = newCapacity + 1ull;
			oldMemorySize = container_base::m_capacity + 1ull;
		}
		else
		{
			newMemorySize = newCapacity;
			oldMemorySize = container_base::m_capacity;
		}

		if (container_base::m_capacity == 0)
		{
			rsl_assert_invalid_object(!mem_rsc::get_ptr());

			mem_rsc::allocate(newMemorySize);

			container_base::m_capacity = newCapacity;

			if constexpr (UsePostFix)
			{
				container_base::construct(1ull, 0);
			}

			return;
		}

		if constexpr (is_trivially_copyable_v<T>)
		{
			mem_rsc::reallocate(oldMemorySize, newMemorySize);
		}
		else
		{
			T* newMem = mem_rsc::m_alloc.allocate(newMemorySize);
			if (newMem)
			{
				mem_rsc::m_alloc.move(newMem, mem_rsc::get_ptr(), container_base::m_size);
			}

			mem_rsc::deallocate(oldMemorySize);
			mem_rsc::set_ptr(newMem);

			if constexpr (UsePostFix)
			{
				container_base::construct(1ull, container_base::m_size);
			}
		}

		container_base::m_capacity = newCapacity;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	constexpr void dynamic_array<T, Alloc, Factory, UsePostFix>::push_back(const value_type& value)
		noexcept(container_base::copy_construct_noexcept)
	{
		if constexpr (UsePostFix)
		{
			if (container_base::m_capacity > 0ull)
			{
				container_base::destroy(1ull, container_base::m_size);
			}
		}

		maybe_grow();
		container_base::emplace_unsafe_impl(container_base::m_size, container_base::m_size + 1, value);
		++container_base::m_size;

		if constexpr (UsePostFix)
		{
			container_base::construct(1ull, container_base::m_size);
		}
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	constexpr void dynamic_array<T, Alloc, Factory, UsePostFix>::push_back(value_type&& value)
		noexcept(container_base::move_construct_noexcept)
	{
		if constexpr (UsePostFix)
		{
			if (container_base::m_capacity > 0ull)
			{
				container_base::destroy(1ull, container_base::m_size);
			}
		}

		maybe_grow();
		container_base::emplace_unsafe_impl(container_base::m_size, container_base::m_size + 1, move(value));
		++container_base::m_size;

		if constexpr (UsePostFix)
		{
			container_base::construct(1ull, container_base::m_size);
		}
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	template <typename... Args>
	constexpr T& dynamic_array<T, Alloc, Factory, UsePostFix>::emplace_back(Args&&... args)
		noexcept(container_base::template construct_noexcept<Args...> && container_base::move_construct_noexcept)
	{
		if constexpr (UsePostFix)
		{
			if (container_base::m_capacity > 0ull)
			{
				container_base::destroy(1ull, container_base::m_size);
			}
		}

		maybe_grow();
		container_base::emplace_unsafe_impl(container_base::m_size, container_base::m_size + 1, rsl::forward<Args>(args)...);
		++container_base::m_size;

		if constexpr (UsePostFix)
		{
			container_base::construct(1ull, container_base::m_size);
		}

		return container_base::back();
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	constexpr void dynamic_array<T, Alloc, Factory, UsePostFix>::pop_back() noexcept
	{
		reduce(1);
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	constexpr void dynamic_array<T, Alloc, Factory, UsePostFix>::reduce(size_type count) noexcept
	{
		if (count > container_base::m_size)
		{
			count = container_base::m_size;
		}

		if constexpr (UsePostFix)
		{
			if (container_base::m_capacity > 0ull)
			{
				container_base::destroy(1ull, container_base::m_size);
			}
		}

		container_base::reset_unsafe_impl(container_base::m_size - count);
		container_base::m_size -= count;

		if constexpr (UsePostFix)
		{
			container_base::construct(1ull, container_base::m_size);
		}
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	constexpr void dynamic_array<T, Alloc, Factory, UsePostFix>::clear() noexcept
	{
		if constexpr (UsePostFix)
		{
			if (container_base::m_capacity > 0ull)
			{
				container_base::destroy(1ull, container_base::m_size);
			}
		}

		container_base::reset_unsafe_impl();
		container_base::m_size = 0;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	constexpr void dynamic_array<T, Alloc, Factory, UsePostFix>::assign(const size_type count, const value_type& value)
	{
		clear();
		insert(0, count, value);
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	template <input_iterator InputIt>
	constexpr void dynamic_array<T, Alloc, Factory, UsePostFix>::assign(InputIt first, InputIt last)
	{
		clear();
		insert(0, first, last);
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	template <input_iterator InputIt>
	constexpr void dynamic_array<T, Alloc, Factory, UsePostFix>::assign(const value_type* ptr, const size_type count)
	{
		clear();
		insert(0, ptr, count);
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	template <size_type N>
	constexpr void dynamic_array<T, Alloc, Factory, UsePostFix>::assign(const value_type (&src)[N])
	{
		clear();
		insert<N>(0, src);
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	template <size_type N>
	constexpr void dynamic_array<T, Alloc, Factory, UsePostFix>::assign(value_type (&&src)[N])
	{
		clear();
		insert<N>(0, rsl::move(src));
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	constexpr typename dynamic_array<T, Alloc, Factory, UsePostFix>::iterator_type
	dynamic_array<T, Alloc, Factory, UsePostFix>::iterator_at(size_type i) noexcept
	{
		return container_base::begin() + i;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	constexpr typename dynamic_array<T, Alloc, Factory, UsePostFix>::const_iterator_type
	dynamic_array<T, Alloc, Factory, UsePostFix>::iterator_at(size_type i) const noexcept
	{
		return container_base::begin() + i;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	constexpr size_type dynamic_array<T, Alloc, Factory, UsePostFix>::append(const value_type& value)
		noexcept(container_base::move_construct_noexcept && container_base::copy_construct_noexcept)
	{
		return insert(container_base::m_size, value);
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	constexpr size_type dynamic_array<T, Alloc, Factory, UsePostFix>::append(value_type&& value)
		noexcept(container_base::move_construct_noexcept)
	{
		return insert(container_base::m_size, rsl::move(value));
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	constexpr size_type
	dynamic_array<T, Alloc, Factory, UsePostFix>::append(const size_type count, const value_type& value)
		noexcept(container_base::move_construct_noexcept && container_base::copy_construct_noexcept)
	{
		return insert(container_base::m_size, count, value);
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	template <input_iterator InputIt>
	constexpr size_type dynamic_array<T, Alloc, Factory, UsePostFix>::append(InputIt first, InputIt last)
	noexcept(container_base::move_construct_noexcept && container_base::template construct_noexcept<iter_value_t<InputIt>>)
	{
		return insert(container_base::m_size, first, last);
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	constexpr size_type dynamic_array<T, Alloc, Factory, UsePostFix>::append(const value_type* ptr, const size_type count)
		noexcept(container_base::move_construct_noexcept && container_base::copy_construct_noexcept)
	{
		return insert(container_base::m_size, ptr, count);
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	template <size_type N>
	constexpr size_type dynamic_array<T, Alloc, Factory, UsePostFix>::append(const value_type (&src)[N])
		noexcept(container_base::move_construct_noexcept && container_base::copy_construct_noexcept)
	{
		return insert(container_base::m_size, src);
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	template <size_type N>
	constexpr size_type dynamic_array<T, Alloc, Factory, UsePostFix>::append(value_type (&&src)[N])
		noexcept(container_base::move_construct_noexcept)
	{
		return insert(container_base::m_size, rsl::move(src));
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	constexpr size_type dynamic_array<T, Alloc, Factory, UsePostFix>::insert(const size_type pos, const value_type& value)
		noexcept(container_base::move_construct_noexcept && container_base::copy_construct_noexcept)
	{
		split_reserve(
			pos, 1,
			container_base::m_size == container_base::m_capacity
				? container_base::m_capacity * 2
				: container_base::m_size + 1
		);

		mem_rsc::construct(1, pos, value);

		return pos;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	constexpr size_type dynamic_array<T, Alloc, Factory, UsePostFix>::insert(const size_type pos, value_type&& value)
		noexcept(container_base::move_construct_noexcept)
	{
		split_reserve(
			pos, 1,
			container_base::m_size == container_base::m_capacity
				? container_base::m_capacity * 2
				: container_base::m_size + 1
		);

		mem_rsc::construct(1, pos, move(value));

		return pos;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	constexpr size_type
	dynamic_array<T, Alloc, Factory, UsePostFix>::insert(const size_type pos, const size_type count, const value_type& value)
		noexcept(container_base::move_construct_noexcept && container_base::copy_construct_noexcept)
	{
		split_reserve(pos, count, container_base::m_size + count);

		mem_rsc::construct(count, pos, value);

		return pos;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	template <input_iterator InputIt>
	constexpr size_type dynamic_array<T, Alloc, Factory, UsePostFix>::insert(
		const size_type pos, InputIt first, InputIt last
	) noexcept(container_base::move_construct_noexcept && container_base::template construct_noexcept<iter_value_t<InputIt>>)
	{
		size_type count = iterator_diff(first, last);
		split_reserve(pos, count, container_base::m_size + count);

		container_base::copy_construct_from_unsafe_impl(pos, pos + count, first);

		return pos;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	constexpr size_type dynamic_array<T, Alloc, Factory, UsePostFix>::insert(
		const size_type pos, const value_type* ptr, const size_type count)
		noexcept(container_base::move_construct_noexcept && container_base::copy_construct_noexcept)
	{
		split_reserve(pos, count, container_base::m_size + count);

		container_base::copy_construct_from_unsafe_impl(pos, pos + count, ptr);

		return pos;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	template <size_type N>
	constexpr size_type dynamic_array<T, Alloc, Factory, UsePostFix>::insert(const size_type pos, const value_type (&src)[N])
		noexcept(container_base::move_construct_noexcept && container_base::copy_construct_noexcept)
	{
		split_reserve(pos, N, container_base::m_size + N);

		container_base::copy_construct_from_unsafe_impl(pos, pos + N, src);

		return pos;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	template <size_type N>
	constexpr size_type dynamic_array<T, Alloc, Factory, UsePostFix>::insert(const size_type pos, value_type (&&src)[N])
		noexcept(container_base::move_construct_noexcept)
	{
		split_reserve(pos, N, container_base::m_size + N);

		container_base::move_construct_from_unsafe_impl(pos, pos + N, src);

		return pos;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	constexpr size_type dynamic_array<T, Alloc, Factory, UsePostFix>::erase_swap(const size_type pos)
		noexcept(container_base::move_construct_noexcept)
	{
		rsl_assert_out_of_range(pos < container_base::m_size);

		if constexpr (UsePostFix)
		{
			mem_rsc::destroy(1, container_base::m_size);
		}

		erase_swap_impl(pos);

		if constexpr (UsePostFix)
		{
			mem_rsc::construct(1, container_base::m_size);
		}

		return pos;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	constexpr size_type dynamic_array<T, Alloc, Factory, UsePostFix>::erase_swap(const_view_type view)
		noexcept(container_base::move_construct_noexcept)
	{
		rsl_assert_ptr_out_of_range(view.data(), mem_rsc::get_ptr(), mem_rsc::get_ptr() + container_base::m_size);
		rsl_assert_ptr_out_of_range(view.data() + view.size(), mem_rsc::get_ptr(),
		                            mem_rsc::get_ptr() + container_base::m_size);

		size_type beginIndex = view.data() - mem_rsc::get_ptr();
		return erase_swap(beginIndex, beginIndex + view.size());
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	constexpr size_type dynamic_array<T, Alloc, Factory, UsePostFix>::erase_swap(const size_type first, size_type last)
		noexcept(container_base::move_construct_noexcept)
	{
		rsl_assert_out_of_range(first < container_base::m_size);
		rsl_assert_invalid_parameters(first < last);
		if (last > container_base::m_size)
		{
			last = container_base::m_size;
		}

		if constexpr (UsePostFix)
		{
			mem_rsc::destroy(1, container_base::m_size);
		}

		size_type count = last - first;
		mem_rsc::destroy(count, first);

		for (size_type i = 0; i < count; ++i)
		{
			mem_rsc::construct(1, i + first, move(*container_base::get_ptr_at(container_base::m_size - (i + 1))));
		}

		container_base::m_size -= count;

		if constexpr (UsePostFix)
		{
			mem_rsc::construct(1, container_base::m_size);
		}

		return first;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	template <typename Comp>
	constexpr size_type dynamic_array<T, Alloc, Factory, UsePostFix>::erase_swap(const Comp& comparable)
		noexcept(container_base::move_construct_noexcept) requires equality_comparable_with<Comp, value_type>
	{
		return erase_swap([&](const_iterator_type iter) { return (*iter) == comparable; });
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	template <typename Func>
	constexpr size_type dynamic_array<T, Alloc, Factory, UsePostFix>::erase_swap(Func&& comparer)
		noexcept(container_base::move_construct_noexcept) requires invocable<Func, bool(const_iterator_type)>
	{
		const size_type originalSize = container_base::m_size;

		size_type erasureCount = 0ull;
		for (size_type i = originalSize; i != 0; --i)
		{
			if (comparer(iterator_at(i - 1)))
			{
				erase_swap_impl(i - 1);
				erasureCount++;
			}
		}

		if constexpr (UsePostFix)
		{
			if (erasureCount != 0ull)
			{
				mem_rsc::destroy(1, originalSize);
				mem_rsc::construct(1, container_base::m_size);
			}
		}

		return erasureCount;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	constexpr size_type dynamic_array<T, Alloc, Factory, UsePostFix>::erase_shift(const size_type pos)
		noexcept(container_base::move_construct_noexcept)
	{
		rsl_assert_out_of_range(pos < container_base::m_size);

		mem_rsc::destroy(1, pos);
		container_base::move_shift_elements_unsafe(pos + 1, container_base::m_size, -1ll);
		--container_base::m_size;

		if constexpr (UsePostFix)
		{
			container_base::destroy(1ull, container_base::m_size + 1ull);
			container_base::construct(1ull, container_base::m_size);
		}

		return pos;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	constexpr size_type dynamic_array<T, Alloc, Factory, UsePostFix>::erase_shift(const const_view_type view)
		noexcept(container_base::move_construct_noexcept)
	{
		rsl_assert_ptr_out_of_range(view.data(), mem_rsc::get_ptr(), mem_rsc::get_ptr() + container_base::m_size);
		rsl_assert_ptr_out_of_range(view.data() + view.size(), mem_rsc::get_ptr(),
		                            mem_rsc::get_ptr() + container_base::m_size);

		size_type beginIndex = view.data() - mem_rsc::get_ptr();
		return erase_shift(beginIndex, beginIndex + view.size());
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	constexpr size_type dynamic_array<T, Alloc, Factory, UsePostFix>::erase_shift(const size_type first, size_type last)
		noexcept(container_base::move_construct_noexcept)
	{
		rsl_assert_out_of_range(first < container_base::m_size);
		rsl_assert_invalid_parameters(first < last);
		if (last > container_base::m_size)
		{
			last = container_base::m_size;
		}

		const size_type count = last - first;
		mem_rsc::destroy(count, first);
		container_base::move_shift_elements_unsafe(last, container_base::m_size, -static_cast<diff_type>(count));
		container_base::m_size -= count;

		if constexpr (UsePostFix)
		{
			if (count != 0ull)
			{
				container_base::destroy(1ull, container_base::m_size + count);
				container_base::construct(1ull, container_base::m_size);
			}
		}

		return first;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	template <typename Comp>
	constexpr size_type dynamic_array<T, Alloc, Factory, UsePostFix>::erase_shift(const Comp& comparable)
		noexcept(container_base::move_construct_noexcept) requires equality_comparable_with<Comp, value_type>
	{
		return erase_shift([&](const_iterator_type iter) { return (*iter) == comparable; });
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	template <typename Func>
	constexpr size_type dynamic_array<T, Alloc, Factory, UsePostFix>::erase_shift(Func&& comparer)
		noexcept(container_base::move_construct_noexcept)
		requires invocable<Func, bool(const_iterator_type)>
	{
		size_type eraseLocation = npos;
		diff_type shift = -1ll;

		const size_type originalSize = container_base::m_size;

		size_type erasureCount = 0ull;
		for (size_type i = 0; i < originalSize; i++)
		{
			if (comparer(iterator_at(i)))
			{
				if (eraseLocation != npos)
				{
					mem_rsc::destroy(1, eraseLocation);
					container_base::move_shift_elements_unsafe(eraseLocation + 1, i, shift);
					--container_base::m_size;
					shift -= 1ll;
				}

				erasureCount++;
				eraseLocation = i;
			}
		}

		if (eraseLocation != npos)
		{
			mem_rsc::destroy(1, eraseLocation);
			container_base::move_shift_elements_unsafe(eraseLocation + 1, container_base::m_size, shift);
			--container_base::m_size;
		}

		if constexpr (UsePostFix)
		{
			if (erasureCount != 0ull)
			{
				container_base::destroy(1ull, originalSize);
				container_base::construct(1ull, container_base::m_size);
			}
		}

		return erasureCount;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	constexpr void dynamic_array<T, Alloc, Factory, UsePostFix>::maybe_grow()
		noexcept(container_base::move_construct_noexcept)
	{
		if (container_base::m_capacity == 0)
		{
			reserve(1);
			return;
		}

		if (container_base::m_size == container_base::m_capacity)
		{
			reserve(container_base::m_capacity * 2);
		}
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	constexpr void dynamic_array<T, Alloc, Factory, UsePostFix>::copy_assign_impl(
		const value_type* src, const size_type srcSize, typename mem_rsc::typed_alloc_type* alloc
	) noexcept(container_base::copy_assign_noexcept && container_base::copy_construct_noexcept)
	{
		if constexpr (UsePostFix)
		{
			if (container_base::m_capacity > 0ull)
			{
				container_base::destroy(1ull, container_base::m_size);
			}
		}

		if (srcSize > container_base::m_capacity || alloc != nullptr)
		{
			if (mem_rsc::get_ptr())
			{
				container_base::reset_unsafe_impl();

				if constexpr (UsePostFix)
				{
					mem_rsc::deallocate(container_base::m_capacity + 1ull);
				}
				else
				{
					mem_rsc::deallocate(container_base::m_capacity);
				}

				container_base::m_capacity = 0;
				container_base::m_size = 0;
			}

			rsl_assert_frequent(container_base::m_size == 0);
			rsl_assert_frequent(container_base::m_capacity == 0);

			if (alloc)
			{
				mem_rsc::m_alloc = *alloc;
			}

			reserve(srcSize);
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

		if constexpr (UsePostFix)
		{
			container_base::construct(1ull, container_base::m_size);
		}
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	constexpr void
	dynamic_array<T, Alloc, Factory, UsePostFix>::move_data_assign_impl(const value_type* src, const size_type srcSize)
		noexcept(container_base::move_assign_noexcept && container_base::move_construct_noexcept)
	{
		if constexpr (UsePostFix)
		{
			if (container_base::m_capacity > 0ull)
			{
				container_base::destroy(1ull, container_base::m_size);
			}
		}

		if (srcSize > container_base::m_capacity)
		{
			if (mem_rsc::get_ptr())
			{
				container_base::reset_unsafe_impl();

				if constexpr (UsePostFix)
				{
					mem_rsc::deallocate(container_base::m_capacity + 1ull);
				}
				else
				{
					mem_rsc::deallocate(container_base::m_capacity);
				}

				container_base::m_capacity = 0;
				container_base::m_size = 0;
			}

			rsl_assert_frequent(container_base::m_size == 0);
			rsl_assert_frequent(container_base::m_capacity == 0);

			reserve(srcSize);
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

		if constexpr (UsePostFix)
		{
			container_base::construct(1ull, container_base::m_size);
		}
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	constexpr void
	dynamic_array<T, Alloc, Factory, UsePostFix>::split_reserve(const size_type pos, const size_type count, const size_type newSize)
		noexcept(container_base::move_construct_noexcept)
	{
		if (container_base::m_capacity == 0)
		{
			reserve(newSize);
		}
		else
		{
			if constexpr (UsePostFix)
			{
				container_base::destroy(1ull, container_base::m_size);
			}

			if (newSize > container_base::m_capacity)
			{
				if constexpr (is_trivially_copyable_v<T>)
				{
					mem_rsc::reallocate(container_base::m_capacity, newSize);
					container_base::move_shift_elements_unsafe(pos, container_base::m_size, count);
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
				container_base::move_shift_elements_unsafe(pos, container_base::m_size, count);
			}
		}

		container_base::m_size = newSize;

		if constexpr (UsePostFix)
		{
			container_base::construct(1ull, container_base::m_size);
		}
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix>
	constexpr void dynamic_array<T, Alloc, Factory, UsePostFix>::erase_swap_impl(const size_type pos)
		noexcept(container_base::move_construct_noexcept)
	{
		--container_base::m_size;
		mem_rsc::destroy(1, pos);

		if (pos != container_base::m_size)
		{
			mem_rsc::construct(1, pos, move(*container_base::get_ptr_at(container_base::m_size)));
			mem_rsc::destroy(1, container_base::m_size);
		}
	}
} // namespace rsl

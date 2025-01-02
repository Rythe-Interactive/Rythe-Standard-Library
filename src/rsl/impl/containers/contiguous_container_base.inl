#pragma once
#include "contiguous_container_base.hpp"

namespace rsl
{
	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	inline contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::contiguous_container_base(
		const allocator_storage_type& allocStorage
	) noexcept(is_nothrow_constructible_v<mem_rsc, const allocator_storage_type&>)
		: mem_rsc(allocStorage)
	{
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	inline contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::contiguous_container_base(
		const factory_storage_type& factoryStorage
	) noexcept(is_nothrow_constructible_v<mem_rsc, const factory_storage_type&>)
		: mem_rsc(factoryStorage)
	{
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	inline contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::contiguous_container_base(
		const allocator_storage_type& allocStorage, const factory_storage_type& factoryStorage
	) noexcept(is_nothrow_constructible_v<mem_rsc, const allocator_storage_type&, const factory_storage_type&>)
		: mem_rsc(allocStorage, factoryStorage)
	{
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr size_type contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::size() const noexcept
	{
		return m_size;
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr bool contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::empty() const noexcept
	{
		return m_size == 0;
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	inline constexpr size_type contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::capacity() const noexcept
	{
		return m_capacity;
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::value_type&
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::at(size_type i) noexcept
	{
		return *(begin() + i);
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr const typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::value_type&
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::at(size_type i) const noexcept
	{
		return *(cbegin() + i);
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::value_type&
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::operator[](size_type i) noexcept
	{
		return at(i);
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr const typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::value_type&
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::operator[](size_type i) const noexcept
	{
		return at(i);
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	inline constexpr T* contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::data() noexcept
	{
		return mem_rsc::get_ptr();
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	inline constexpr const T* contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::data() const noexcept
	{
		return mem_rsc::get_ptr();
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	inline constexpr contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::view_type
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::view() noexcept
	{
		return view_type(mem_rsc::get_ptr(), m_size);
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	inline constexpr contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::const_view_type
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::view() const noexcept
	{
		return const_view_type(mem_rsc::get_ptr(), m_size);
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	inline constexpr contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::operator view_type() noexcept
	{
		return view();
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	inline constexpr contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::operator const_view_type(
	) const noexcept
	{
		return view();
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::value_type&
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::front() noexcept
	{
		return at(0);
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr const typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::value_type&
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::front() const noexcept
	{
		return at(0);
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::value_type&
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::back() noexcept
	{
		return at(m_size - 1);
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr const typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::value_type&
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::back() const noexcept
	{
		return at(m_size - 1);
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::iterator_type
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::begin() noexcept
	{
		return typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::iterator_type(mem_rsc::m_ptr);
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::const_iterator_type
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::begin() const noexcept
	{
		return cbegin();
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>

	constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::const_iterator_type
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::cbegin() const noexcept
	{
		return const_iterator_type(mem_rsc::get_ptr());
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>

	constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::iterator_type
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::end() noexcept
	{
		return iterator_type(mem_rsc::get_ptr() + m_size);
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>

	constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::const_iterator_type
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::end() const noexcept
	{
		return cend();
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>

	constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::const_iterator_type
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::cend() const noexcept
	{
		return const_iterator_type(mem_rsc::get_ptr() + m_size);
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>

	constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::reverse_iterator_type
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::rbegin() noexcept
	{
		return reverse_iterator_type(mem_rsc::get_ptr() + m_size - 1);
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>

	constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::const_reverse_iterator_type
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::rbegin() const noexcept
	{
		return crbegin();
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>

	constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::const_reverse_iterator_type
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::crbegin() const noexcept
	{
		return const_reverse_iterator_type(mem_rsc::get_ptr() + m_size - 1);
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>

	constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::reverse_iterator_type
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::rend() noexcept
	{
		return reverse_iterator_type(mem_rsc::get_ptr() - 1);
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>

	constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::const_reverse_iterator_type
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::rend() const noexcept
	{
		return crend();
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>

	constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::const_reverse_iterator_type
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::crend() const noexcept
	{
		return const_reverse_iterator_type(mem_rsc::get_ptr() - 1);
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	inline constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::copy_assign_from_unsafe_impl(
		size_type offset, size_type end, const value_type* src
	) noexcept(copy_assign_noexcept)
	{
		for (auto to = mem_rsc::get_ptr() + offset; to != mem_rsc::get_ptr() + end; to++, src++)
		{
			*to = *src;
		}
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	inline constexpr void
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::copy_construct_from_unsafe_impl(
		size_type offset, size_type end, const value_type* src
	) noexcept(copy_construct_noexcept)
	{
		for (size_type i = offset; i != end; i++, src++)
		{
			mem_rsc::construct(1, i, *src);
		}
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	inline constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::move_assign_from_unsafe_impl(
		size_type offset, size_type end, const value_type* src
	) noexcept(move_assign_noexcept)
	{
		for (auto to = mem_rsc::get_ptr() + offset; to != mem_rsc::get_ptr() + end; to++, src++)
		{
			*to = move(*src);
		}
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	template <typename... Args>
	inline constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::emplace_unsafe_impl(
		size_type offset, size_type end, Args&&... args
	) noexcept(construct_noexcept<Args...>)
	{
		mem_rsc::construct(end - offset, offset, forward<Args>(args)...);
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	inline constexpr void
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::move_construct_from_unsafe_impl(
		size_type offset, size_type end, const value_type* src
	) noexcept(move_construct_noexcept)
	{
		for (size_type i = offset; i != end; i++, src++)
		{
			mem_rsc::construct(1, i, move(*src));
		}
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	inline constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::reset_unsafe_impl(
		size_type offset, size_type end
	) noexcept
	{
		if (end > m_size)
		{
			end = m_size;
		}

		mem_rsc::destroy(end - offset, offset);
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	inline constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::move_shift_elements_unsafe(
		size_type offset, size_type end, diff_type shift
	) noexcept(move_construct_noexcept)
	{
		if (end > m_size)
		{
			end = m_size;
		}

		for (size_type i = offset; i != end; i++)
		{
			mem_rsc::construct(1, static_cast<size_type>(i + shift), move(at(i)));
		}
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	inline constexpr T* contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::get_ptr_at(size_type i) noexcept
	{
		return mem_rsc::get_ptr() + i;
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	inline constexpr const T* contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::get_ptr_at(size_type i
	) const noexcept
	{
		return mem_rsc::get_ptr() + i;
	}
} // namespace rsl

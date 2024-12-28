#pragma once
#include "contiguous_container_base.hpp"

namespace rsl
{
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
		return typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::iterator_type(m_ptr);
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
		for (auto to = begin() + offset; to != begin() + end; to++, src++)
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
		for (auto to = begin() + offset; to != begin() + end; to++, src++)
		{
			new (to_address(to)) value_type(*src);
		}
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	inline constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::move_assign_from_unsafe_impl(
		size_type offset, size_type end, const value_type* src
	) noexcept(move_assign_noexcept)
	{
		for (auto to = begin() + offset; to != begin() + end; to++, src++)
		{
			*to_address(to) = move(*src);
		}
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	template <typename... Args>
	inline constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::emplace_unsafe_impl(
		size_type offset, size_type end, Args&&... args
	) noexcept(construct_noexcept<Args...>)
	{
		for (auto to = begin() + offset; to != begin() + end; to++)
		{
			new (to_address(to)) value_type(forward<Args>(args)...);
		}
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	inline constexpr void
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::move_construct_from_unsafe_impl(
		size_type offset, size_type end, const value_type* src
	) noexcept(move_construct_noexcept)
	{
		for (auto to = begin() + offset; to != begin() + end; to++, src++)
		{
			new (to_address(to)) value_type(move(*src));
		}
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	inline constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::reset_unsafe_impl(
		size_type offset, size_type end
	) noexcept
	{
		for (size_type i = offset; i < end; i++)
		{
			at(i).~value_type();
		}
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	inline constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::copy_assign_impl(
		const value_type* src, size_type srcSize
	) noexcept(copy_assign_noexcept && copy_construct_noexcept)
	{
		if (m_size >= srcSize)
		{
			reset_unsafe_impl(srcSize, m_size);
			m_size = srcSize;
			copy_assign_from_unsafe_impl(0, m_size, src);
		}
		else
		{
			copy_assign_from_unsafe_impl(0, m_size, src);
			copy_construct_from_unsafe_impl(m_size, srcSize, src);
			m_size = srcSize;
		}
	}
} // namespace rsl

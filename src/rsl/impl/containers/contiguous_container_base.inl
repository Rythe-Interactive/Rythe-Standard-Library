#pragma once
#include "contiguous_container_base.hpp"

namespace rsl
{
	template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::contiguous_container_base(
		const allocator_storage_type& allocStorage
	) noexcept(is_nothrow_constructible_v<mem_rsc, const allocator_storage_type&>)
		: mem_rsc(allocStorage)
	{
	}

	template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::contiguous_container_base(
		const factory_storage_type& factoryStorage
	) noexcept(is_nothrow_constructible_v<mem_rsc, const factory_storage_type&>)
		: mem_rsc(factoryStorage)
	{
	}

	template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::contiguous_container_base(
		const allocator_storage_type& allocStorage, const factory_storage_type& factoryStorage
	) noexcept(is_nothrow_constructible_v<mem_rsc, const allocator_storage_type&, const factory_storage_type&>)
		: mem_rsc(allocStorage, factoryStorage)
	{
	}

	template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr size_type contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::size() const noexcept
	{
		return m_size;
	}

	template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr bool contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::empty() const noexcept
	{
		return m_size == 0;
	}

	template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr size_type contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::capacity() const noexcept
	{
		return m_capacity;
	}

	template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::value_type&
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::at(const size_type i) noexcept
	{
		rsl_assert_out_of_range(i < m_size);
		return *get_ptr_at(i);
	}

	template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr const typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::value_type&
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::at(const size_type i) const noexcept
	{
		rsl_assert_out_of_range(i < m_size);
		return *get_ptr_at(i);
	}

	template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::value_type&
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::operator[](const size_type i) noexcept
	{
		return at(i);
	}

	template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr const typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::value_type&
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::operator[](const size_type i) const noexcept
	{
		return at(i);
	}

	template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr T* contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::data() noexcept
	{
		return mem_rsc::get_ptr();
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr const T* contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::data() const noexcept
	{
		return mem_rsc::get_ptr();
	}

	template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::view_type
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::view() noexcept
	{
		return view_type(mem_rsc::get_ptr(), m_size);
	}

	template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::const_view_type
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::view() const noexcept
	{
		return const_view_type(mem_rsc::get_ptr(), m_size);
	}

	template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::operator view_type() noexcept
	{
		return view();
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::operator const_view_type(
	) const noexcept
	{
		return view();
	}

	template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::value_type&
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::front() noexcept
	{
		return at(0);
	}

	template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr const typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::value_type&
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::front() const noexcept
	{
		return at(0);
	}

	template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::value_type&
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::back() noexcept
	{
		return at(m_size - 1);
	}

	template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr const typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::value_type&
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::back() const noexcept
	{
		return at(m_size - 1);
	}

	template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::iterator_type
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::begin() noexcept
	{
		return contiguous_container_base::iterator_type(mem_rsc::m_ptr);
	}

	template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::const_iterator_type
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::begin() const noexcept
	{
		return cbegin();
	}

	template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::const_iterator_type
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::cbegin() const noexcept
	{
		return const_iterator_type(mem_rsc::get_ptr());
	}

	template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::iterator_type
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::end() noexcept
	{
		return iterator_type(mem_rsc::get_ptr() + m_size);
	}

	template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::const_iterator_type
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::end() const noexcept
	{
		return cend();
	}

	template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::const_iterator_type
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::cend() const noexcept
	{
		return const_iterator_type(mem_rsc::get_ptr() + m_size);
	}

	template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::reverse_iterator_type
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::rbegin() noexcept
	{
		return reverse_iterator_type(end());
	}

	template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::const_reverse_iterator_type
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::rbegin() const noexcept
	{
		return crbegin();
	}

	template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::const_reverse_iterator_type
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::crbegin() const noexcept
	{
		return const_reverse_iterator_type(cend());
	}

	template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::reverse_iterator_type
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::rend() noexcept
	{
		return reverse_iterator_type(begin());
	}

	template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::const_reverse_iterator_type
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::rend() const noexcept
	{
		return crend();
	}

	template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::const_reverse_iterator_type
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::crend() const noexcept
	{
		return const_reverse_iterator_type(cbegin());
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	template <input_iterator InputIt>
	constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::copy_assign_from_unsafe_impl(
		const size_type offset, const size_type end, InputIt srcIter
	) noexcept(copy_assign_noexcept)
	{
		for (auto to = mem_rsc::get_ptr() + offset; to != mem_rsc::get_ptr() + end; ++to, ++srcIter)
		{
			*to = *srcIter;
		}
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	template <input_iterator InputIt>
	constexpr void
	contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::copy_construct_from_unsafe_impl(
		const size_type offset, const size_type end, InputIt srcIter
	) noexcept(copy_construct_noexcept)
	{
		if constexpr (is_pointer_v<InputIt>)
		{
			mem_rsc::copy(end - offset, offset, srcIter);
		}
		else
		{
			for (size_type i = offset; i != end; i++, ++srcIter)
			{
				mem_rsc::construct(1, i, *srcIter);
			}
		}
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	template <input_iterator InputIt>
	constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::move_assign_from_unsafe_impl(
		const size_type offset, const size_type end, InputIt srcIter
	) noexcept(move_assign_noexcept)
	{
		for (auto to = mem_rsc::get_ptr() + offset; to != mem_rsc::get_ptr() + end; ++to, ++srcIter)
		{
			*to = move(*srcIter);
		}
	}

	template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	template <input_iterator InputIt>
	constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::move_construct_from_unsafe_impl(
		const size_type offset, const size_type end, InputIt srcIter
	) noexcept(move_construct_noexcept)
	{
		if constexpr (is_pointer_v<InputIt>)
		{
			mem_rsc::move(end - offset, offset, srcIter);
		}
		else
		{
			for (size_type i = offset; i != end; i++, ++srcIter)
			{
				mem_rsc::construct(1, i, move(*srcIter));
			}
		}
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	template <typename... Args>
	constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::emplace_unsafe_impl(
		const size_type offset, const size_type end, Args&&... args
	) noexcept(construct_noexcept<Args...>)
	{
		mem_rsc::construct(end - offset, offset, rsl::forward<Args>(args)...);
	}

	template <
		typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::reset_unsafe_impl(
		const size_type offset, size_type end
	) noexcept
	{
		if (end > m_size)
		{
			end = m_size;
		}

		mem_rsc::destroy(end - offset, offset);
	}

	template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::move_shift_elements_unsafe(
		const size_type offset, const size_type end, const diff_type shift
	) noexcept(move_construct_noexcept)
	{
		for (size_type i = offset; i != end; i++)
		{
			mem_rsc::construct(1, static_cast<size_type>(i + shift), move(*get_ptr_at(i)));
		}
	}

	template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr T* contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::get_ptr_at(const size_type i) noexcept
	{
		return mem_rsc::get_ptr() + i;
	}

	template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr const T* contiguous_container_base<T, Alloc, Factory, Iter, ConstIter>::get_ptr_at(const size_type i) const noexcept
	{
		return mem_rsc::get_ptr() + i;
	}
} // namespace rsl

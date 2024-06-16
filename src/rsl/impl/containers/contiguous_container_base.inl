#include "contiguous_container_base.hpp"
#pragma once

namespace rsl
{
#define CCB_TEMPLATE_ARGS typename T, typename Alloc, typename Iter, typename ConstIter

#define CCB_VALUE_TYPE typename contiguous_container_base<T, Alloc, Iter, ConstIter>::value_type

#define CCB_ITER_TYPE typename contiguous_container_base<T, Alloc, Iter, ConstIter>::iterator_type

#define CCB_CONST_ITER_TYPE typename contiguous_container_base<T, Alloc, Iter, ConstIter>::const_iterator_type

#define CCB_REVERSE_ITER_TYPE typename contiguous_container_base<T, Alloc, Iter, ConstIter>::reverse_iterator_type

#define CCB_CONST_REVERSE_ITER_TYPE                                                                                    \
	typename contiguous_container_base<T, Alloc, Iter, ConstIter>::const_reverse_iterator_type

#define CCB_TYPE contiguous_container_base<T, Alloc, Iter, ConstIter>

	template <CCB_TEMPLATE_ARGS>
	[[nodiscard]]
	constexpr size_type CCB_TYPE::size() const noexcept
	{
		return m_size;
	}

	template <CCB_TEMPLATE_ARGS>
	[[nodiscard]]
	constexpr bool CCB_TYPE::empty() const noexcept
	{
		return m_size == 0;
	}

	template <CCB_TEMPLATE_ARGS>
	[[nodiscard]]
	constexpr CCB_VALUE_TYPE& CCB_TYPE::at(size_type i) noexcept
	{
		return *(begin() + i);
	}

	template <CCB_TEMPLATE_ARGS>
	[[nodiscard]]
	constexpr const CCB_VALUE_TYPE& CCB_TYPE::at(size_type i) const noexcept
	{
		return *(cbegin() + i);
	}

	template <CCB_TEMPLATE_ARGS>
	[[nodiscard]]
	constexpr CCB_VALUE_TYPE& CCB_TYPE::operator[](size_type i) noexcept
	{
		return at(i);
	}

	template <CCB_TEMPLATE_ARGS>
	[[nodiscard]]
	constexpr const CCB_VALUE_TYPE& CCB_TYPE::operator[](size_type i) const noexcept
	{
		return at(i);
	}

	template <CCB_TEMPLATE_ARGS>
	[[nodiscard]]
	constexpr CCB_VALUE_TYPE& CCB_TYPE::front() noexcept
	{
		return at(0);
	}

	template <CCB_TEMPLATE_ARGS>
	[[nodiscard]]
	constexpr const CCB_VALUE_TYPE& CCB_TYPE::front() const noexcept
	{
		return at(0);
	}

	template <CCB_TEMPLATE_ARGS>
	[[nodiscard]]
	constexpr CCB_VALUE_TYPE& CCB_TYPE::back() noexcept
	{
		return at(m_size - 1);
	}

	template <CCB_TEMPLATE_ARGS>
	[[nodiscard]]
	constexpr const CCB_VALUE_TYPE& CCB_TYPE::back() const noexcept
	{
		return at(m_size - 1);
	}

	template <CCB_TEMPLATE_ARGS>
	[[nodiscard]]
	constexpr CCB_ITER_TYPE CCB_TYPE::begin() noexcept
	{
		return CCB_ITER_TYPE(m_ptr);
	}

	template <CCB_TEMPLATE_ARGS>
	[[nodiscard]]
	constexpr CCB_CONST_ITER_TYPE CCB_TYPE::begin() const noexcept
	{
		return cbegin();
	}

	template <CCB_TEMPLATE_ARGS>
	[[nodiscard]]
	constexpr CCB_CONST_ITER_TYPE CCB_TYPE::cbegin() const noexcept
	{
		return CCB_CONST_ITER_TYPE(m_ptr);
	}

	template <CCB_TEMPLATE_ARGS>
	[[nodiscard]]
	constexpr CCB_ITER_TYPE CCB_TYPE::end() noexcept
	{
		return CCB_ITER_TYPE(m_ptr + m_size);
	}

	template <CCB_TEMPLATE_ARGS>
	[[nodiscard]]
	constexpr CCB_CONST_ITER_TYPE CCB_TYPE::end() const noexcept
	{
		return cend();
	}

	template <CCB_TEMPLATE_ARGS>
	[[nodiscard]]
	constexpr CCB_CONST_ITER_TYPE CCB_TYPE::cend() const noexcept
	{
		return CCB_CONST_ITER_TYPE(m_ptr + m_size);
	}

	template <CCB_TEMPLATE_ARGS>
	[[nodiscard]]
	constexpr CCB_REVERSE_ITER_TYPE CCB_TYPE::rbegin() noexcept
	{
		return CCB_REVERSE_ITER_TYPE(m_ptr + m_size - 1);
	}

	template <CCB_TEMPLATE_ARGS>
	[[nodiscard]]
	constexpr CCB_CONST_REVERSE_ITER_TYPE CCB_TYPE::rbegin() const noexcept
	{
		return crbegin();
	}

	template <CCB_TEMPLATE_ARGS>
	[[nodiscard]]
	constexpr CCB_CONST_REVERSE_ITER_TYPE CCB_TYPE::crbegin() const noexcept
	{
		return CCB_CONST_REVERSE_ITER_TYPE(m_ptr + m_size - 1);
	}

	template <CCB_TEMPLATE_ARGS>
	[[nodiscard]]
	constexpr CCB_REVERSE_ITER_TYPE CCB_TYPE::rend() noexcept
	{
		return CCB_REVERSE_ITER_TYPE(m_ptr - 1);
	}

	template <CCB_TEMPLATE_ARGS>
	[[nodiscard]]
	constexpr CCB_CONST_REVERSE_ITER_TYPE CCB_TYPE::rend() const noexcept
	{
		return crend();
	}

	template <CCB_TEMPLATE_ARGS>
	[[nodiscard]]
	constexpr CCB_CONST_REVERSE_ITER_TYPE CCB_TYPE::crend() const noexcept
	{
		return CCB_CONST_REVERSE_ITER_TYPE(m_ptr - 1);
	}


#undef CCB_TEMPLATE_ARGS
#undef CCB_VALUE_TYPE
#undef CCB_ITER_TYPE
#undef CCB_CONST_ITER_TYPE
#undef CCB_REVERSE_ITER_TYPE
#undef CCB_CONST_REVERSE_ITER_TYPE
#undef CCB_TYPE
} // namespace rsl

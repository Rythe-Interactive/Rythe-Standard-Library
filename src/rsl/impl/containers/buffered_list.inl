#pragma once

#include "buffered_list.hpp"

namespace rsl
{
	template <typename ValueType, size_type maxSize>
	inline constexpr buffered_list<ValueType, maxSize>::buffered_list(const buffered_list& src)
		noexcept(copy_construct_noexcept)
		: m_size(validate_size(src.m_size))
	{
		copy_construct_from_unsafe_impl(0, m_size, src.m_buffer);
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr buffered_list<ValueType, maxSize>::buffered_list(buffered_list&& src)
		noexcept(move_construct_noexcept)
		: m_size(validate_size(src.m_size))
	{
		move_construct_from_unsafe_impl(0, m_size, src.m_buffer);
	}

	template <typename ValueType, size_type maxSize>
	template <size_type N>
	inline constexpr buffered_list<ValueType, maxSize>::buffered_list(const value_type (&arr)[N])
		noexcept(copy_construct_noexcept)
		: m_size(validate_size(N))
	{
		copy_construct_from_unsafe_impl(0, m_size, arr);
	}

	template <typename ValueType, size_type maxSize>
	template <size_type N>
	inline constexpr buffered_list<ValueType, maxSize>::buffered_list(value_type (&&arr)[N])
		noexcept(move_construct_noexcept)
		: m_size(validate_size(N))
	{
		move_construct_from_unsafe_impl(0, m_size, arr);
	}

	template <typename ValueType, size_type maxSize>
	template <typename... Args>
	inline constexpr buffered_list<ValueType, maxSize>::buffered_list(
		size_type count, in_place_signal_type, Args&&... args
	) noexcept(construct_noexcept<Args...>)
		: m_size(validate_size(count))
	{
		emplace_unsafe_impl(0, m_size, forward<Args>(args)...);
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr buffered_list<ValueType, maxSize>::buffered_list(view_type src) noexcept(copy_construct_noexcept)
		: m_size(validate_size(src.size()))
	{
		copy_construct_from_unsafe_impl(0, m_size, src.data());
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr buffered_list<ValueType, maxSize>&
	buffered_list<ValueType, maxSize>::operator=(const buffered_list& src)
		noexcept(copy_assign_noexcept && copy_construct_noexcept)
	{
		copy_assign_impl(src.m_buffer, src.m_size);
		return *this;
	}

	template <typename ValueType, size_type maxSize>
	template <size_type N>
	inline constexpr buffered_list<ValueType, maxSize>&
	buffered_list<ValueType, maxSize>::operator=(const value_type (&arr)[N])
		noexcept(copy_assign_noexcept && copy_construct_noexcept)
	{
		copy_assign_impl(arr, N);
		return *this;
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr buffered_list<ValueType, maxSize>& buffered_list<ValueType, maxSize>::operator=(view_type src)
		noexcept(copy_assign_noexcept && copy_construct_noexcept)
	{
		copy_assign_impl(src.data(), src.size());
		return *this;
	}

	template <typename ValueType, size_type maxSize>
	template <typename... Args>
	inline constexpr void buffered_list<ValueType, maxSize>::resize(size_type newSize, Args&&... args)
		noexcept(construct_noexcept<Args...>)
	{
		size_type oldSize = m_size;
		m_size = validate_size(newSize);

		if (m_size >= oldSize)
		{
			emplace_unsafe_impl(oldSize, m_size, rsl::forward<Args>(args)...);
		}
		else
		{
			reset_unsafe_impl(m_size, oldSize);
		}
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr void buffered_list<ValueType, maxSize>::push_back(const value_type& value)
		noexcept(move_construct_noexcept)
	{
		size_type oldSize = m_size++;
		rsl_assert_out_of_range(m_size <= maxSize);
		copy_construct_from_unsafe_impl(oldSize, m_size, &value);
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr void buffered_list<ValueType, maxSize>::push_back(value_type&& value)
		noexcept(move_construct_noexcept)
	{
		size_type oldSize = m_size++;
		rsl_assert_out_of_range(m_size <= maxSize);
		move_construct_from_unsafe_impl(oldSize, m_size, &value);
	}

	template <typename ValueType, size_type maxSize>
	template <typename... Args>
	inline constexpr ValueType& buffered_list<ValueType, maxSize>::emplace_back(Args&&... args)
		noexcept(construct_noexcept<Args...>)
	{
		size_type oldSize = m_size++;
		rsl_assert_out_of_range(m_size <= maxSize);
		emplace_unsafe_impl(oldSize, m_size, rsl::forward<Args>(args)...);
		return m_buffer[m_size - 1].value;
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr void buffered_list<ValueType, maxSize>::pop_back(size_type count) noexcept
	{
		size_type oldSize = m_size;
		m_size = count > m_size ? 0 : (m_size - count);
		reset_unsafe_impl(m_size, oldSize);
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr void buffered_list<ValueType, maxSize>::clear() noexcept
	{
		pop_back(maxSize);
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr ValueType& buffered_list<ValueType, maxSize>::at(size_type i) noexcept
	{
		rsl_assert_out_of_range(i < m_size);
		return m_buffer[i].value;
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr const ValueType& buffered_list<ValueType, maxSize>::at(size_type i) const noexcept
	{
		rsl_assert_out_of_range(i < m_size);
		return m_buffer[i].value;
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr ValueType& buffered_list<ValueType, maxSize>::raw_at(size_type i) noexcept
	{
		return m_buffer[i].value;
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr const ValueType& buffered_list<ValueType, maxSize>::raw_at(size_type i) const noexcept
	{
		return m_buffer[i].value;
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr ValueType& buffered_list<ValueType, maxSize>::operator[](size_type i)
	{
		return at(i);
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr const ValueType& buffered_list<ValueType, maxSize>::operator[](size_type i) const
	{
		return at(i);
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr bool buffered_list<ValueType, maxSize>::empty() const noexcept
	{
		return m_size == 0;
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr size_type buffered_list<ValueType, maxSize>::size() const noexcept
	{
		return m_size;
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr size_type buffered_list<ValueType, maxSize>::capacity() const noexcept
	{
		return maxSize;
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr buffered_list<ValueType, maxSize>::view_type buffered_list<ValueType, maxSize>::view() noexcept
	{
		return view_type(&m_buffer->value, m_size);
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr buffered_list<ValueType, maxSize>::const_view_type
	buffered_list<ValueType, maxSize>::view() const noexcept
	{
		return const_view_type(&m_buffer->value, m_size);
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr buffered_list<ValueType, maxSize>::view_type buffered_list<ValueType, maxSize>::raw_view() noexcept
	{
		return view_type(&m_buffer->value, maxSize);
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr buffered_list<ValueType, maxSize>::const_view_type
	buffered_list<ValueType, maxSize>::raw_view() const noexcept
	{
		return const_view_type(&m_buffer->value, maxSize);
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr buffered_list<ValueType, maxSize>::operator const value_type*() const noexcept
	{
		return &m_buffer->value;
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr buffered_list<ValueType, maxSize>::operator view_type() noexcept
	{
		return view();
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr buffered_list<ValueType, maxSize>::operator const_view_type() const noexcept
	{
		return view();
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr buffered_list<ValueType, maxSize>::iterator buffered_list<ValueType, maxSize>::begin() noexcept
	{
		return &m_buffer->value;
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr buffered_list<ValueType, maxSize>::const_iterator
	buffered_list<ValueType, maxSize>::begin() const noexcept
	{
		return &m_buffer->value;
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr buffered_list<ValueType, maxSize>::const_iterator
	buffered_list<ValueType, maxSize>::cbegin() const noexcept
	{
		return &m_buffer->value;
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr buffered_list<ValueType, maxSize>::iterator buffered_list<ValueType, maxSize>::end() noexcept
	{
		return &m_buffer[m_size].value;
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr buffered_list<ValueType, maxSize>::const_iterator
	buffered_list<ValueType, maxSize>::end() const noexcept
	{
		return &m_buffer[m_size].value;
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr buffered_list<ValueType, maxSize>::const_iterator
	buffered_list<ValueType, maxSize>::cend() const noexcept
	{
		return &m_buffer[m_size].value;
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr buffered_list<ValueType, maxSize>::reverse_iterator
	buffered_list<ValueType, maxSize>::rend() noexcept
	{
		return reverse_iterator(&m_buffer[m_size].value);
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr buffered_list<ValueType, maxSize>::const_reverse_iterator
	buffered_list<ValueType, maxSize>::rend() const noexcept
	{
		return const_reverse_iterator(&m_buffer[m_size].value);
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr buffered_list<ValueType, maxSize>::const_reverse_iterator
	buffered_list<ValueType, maxSize>::crend() const noexcept
	{
		return const_reverse_iterator(&m_buffer[m_size].value);
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr buffered_list<ValueType, maxSize>::reverse_iterator
	buffered_list<ValueType, maxSize>::rbegin() noexcept
	{
		return reverse_iterator(&m_buffer->value);
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr buffered_list<ValueType, maxSize>::const_reverse_iterator
	buffered_list<ValueType, maxSize>::rbegin() const noexcept
	{
		return const_reverse_iterator(&m_buffer->value);
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr buffered_list<ValueType, maxSize>::const_reverse_iterator
	buffered_list<ValueType, maxSize>::crbegin() const noexcept
	{
		return const_reverse_iterator(&m_buffer->value);
	}

	template <typename ValueType, size_type maxSize>
	constexpr size_type buffered_list<ValueType, maxSize>::validate_size(size_type newSize) noexcept
	{
		return newSize < maxSize ? newSize : maxSize;
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr void buffered_list<ValueType, maxSize>::copy_assign_from_unsafe_impl(
		size_type begin, size_type end, const value_type* src
	) noexcept(copy_assign_noexcept)
	{
		for (auto* to = m_buffer + begin; to != m_buffer + end; to++, src++)
		{
			to->value = *src;
		}
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr void buffered_list<ValueType, maxSize>::copy_construct_from_unsafe_impl(
		size_type begin, size_type end, const value_type* src
	) noexcept(copy_construct_noexcept)
	{
		for (auto* to = m_buffer + begin; to != m_buffer + end; to++, src++)
		{
			new (&to->value) value_type(*src);
		}
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr void buffered_list<ValueType, maxSize>::move_assign_from_unsafe_impl(
		size_type begin, size_type end, const value_type* src
	) noexcept(move_assign_noexcept)
	{
		for (auto* to = m_buffer + begin; to != m_buffer + end; to++, src++)
		{
			to->value = rsl::move(*src);
		}
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr void buffered_list<ValueType, maxSize>::move_construct_from_unsafe_impl(
		size_type begin, size_type end, const value_type* src
	) noexcept(move_construct_noexcept)
	{
		for (auto* to = m_buffer + begin; to != m_buffer + end; to++, src++)
		{
			new (&to->value) value_type(rsl::move(*src));
		}
	}

	template <typename ValueType, size_type maxSize>
	template <typename... Args>
	inline constexpr void
	buffered_list<ValueType, maxSize>::emplace_unsafe_impl(size_type begin, size_type end, Args&&... args)
		noexcept(construct_noexcept<Args...>)
	{
		for (auto* to = m_buffer + begin; to != m_buffer + end; to++)
		{
			new (&to->value) value_type(rsl::forward<Args>(args)...);
		}
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr void buffered_list<ValueType, maxSize>::reset_unsafe_impl(size_type begin, size_type end) noexcept
	{
		for (size_type i = begin; i < end; i++)
		{
			m_buffer[i].value.~value_type();
		}
	}

	template <typename ValueType, size_type maxSize>
	inline constexpr void buffered_list<ValueType, maxSize>::copy_assign_impl(const value_type* src, size_type srcSize)
		noexcept(copy_assign_noexcept && copy_construct_noexcept)
	{
		if (m_size >= srcSize)
		{
			reset_unsafe_impl(srcSize, m_size);
			m_size = srcSize;
			copy_assign_from_unsafe_impl(0, m_size, src);
		}
		else
		{
			size_type effectiveSize = validate_size(srcSize);
			copy_assign_from_unsafe_impl(0, m_size, src);
			copy_construct_from_unsafe_impl(m_size, effectiveSize, src);
			m_size = effectiveSize;
		}
	}
} // namespace rsl

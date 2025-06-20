#pragma once
#include "string.hpp"

namespace rsl
{
	template <typename CharType, allocator_type Alloc>
	template <size_type N>
	constexpr basic_dynamic_string<CharType, Alloc> basic_dynamic_string<CharType, Alloc>::from_array(const value_type(& arr)[N])
		noexcept(container_base::copy_construct_noexcept)
	{
		basic_dynamic_string result;
		result.allocate(N);
		result.m_size = N;
		result.m_capacity = N;
		result.copy_construct_from_unsafe_impl(0, result.m_size, &arr[0]);

		return result;
	}

	template <typename CharType, allocator_type Alloc>
	template <size_type N>
	constexpr basic_dynamic_string<CharType, Alloc> basic_dynamic_string<CharType, Alloc>::from_array(value_type(&& arr)[N])
		noexcept(container_base::move_construct_noexcept)
	{
		basic_dynamic_string result;
		result.allocate(N);
		result.m_size = N;
		result.m_capacity = N;
		result.move_construct_from_unsafe_impl(0, result.m_size, &arr[0]);

		return result;
	}

	template <typename CharType, allocator_type Alloc>
	constexpr basic_dynamic_string<CharType, Alloc> basic_dynamic_string<CharType, Alloc>::from_buffer(const value_type* ptr, size_type count)
		noexcept(container_base::copy_construct_noexcept)
	{
		basic_dynamic_string result;
		result.allocate(count);
		result.m_size = count;
		result.m_capacity = count;
		result.copy_construct_from_unsafe_impl(0, result.m_size, ptr);

		return result;
	}

	template <typename CharType, allocator_type Alloc>
	constexpr basic_dynamic_string<CharType, Alloc> basic_dynamic_string<CharType, Alloc>::from_view(view_type src)
		noexcept(container_base::copy_construct_noexcept)
	{
		basic_dynamic_string result;
		result.allocate(src.size());
		result.m_size = src.size();
		result.m_capacity = src.size();
		result.copy_construct_from_unsafe_impl(0, result.m_size, src.data());

		return result;
	}

	template <typename CharType, allocator_type Alloc>
	constexpr basic_dynamic_string<CharType, Alloc> basic_dynamic_string<CharType, Alloc>::create_reserved(size_type capacity)
		noexcept
	{
		basic_dynamic_string result;
		result.reserve(capacity);

		return result;
	}

	template <typename CharType, allocator_type Alloc>
	template <typename ... Args>
	constexpr basic_dynamic_string<CharType, Alloc> basic_dynamic_string<CharType, Alloc>::create_in_place(size_type count, CharType val)
		noexcept(container_base::copy_construct_noexcept)
	{
		basic_dynamic_string result;
		result.allocate(count);
		result.m_size = count;
		result.m_capacity = count;
		result.construct(count, 0, val);

		return result;
	}

	//TODO(Rowan): Woops forgot to implement these
	template <typename CharType, allocator_type Alloc>
	inline basic_dynamic_string<CharType, Alloc>& basic_dynamic_string<CharType, Alloc>::append(view_type view)
	{
		return *this;
	}

	template <typename CharType, allocator_type Alloc>
	inline basic_dynamic_string<CharType, Alloc>& basic_dynamic_string<CharType, Alloc>::operator+=(view_type rhs)
	{
		return *this;
	}

	template <typename CharType, allocator_type Alloc>
	inline basic_dynamic_string<CharType, Alloc>& basic_dynamic_string<CharType, Alloc>::replace(size_type pos, size_type count, view_type view)
	{
		return *this;
	}
}

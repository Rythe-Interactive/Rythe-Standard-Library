#pragma once
#include "../logging/fmt_include.hpp"

namespace rsl
{
	template <typename CharType, allocator_type Alloc>
	constexpr basic_dynamic_string<CharType, Alloc>::basic_dynamic_string(
		const base& src) noexcept(container_base::copy_construct_noexcept)
		: base(src) {}

	template <typename CharType, allocator_type Alloc>
	constexpr basic_dynamic_string<CharType, Alloc>::basic_dynamic_string(base&& src) noexcept
		: base(rsl::move(src)) {}

	template <typename CharType, allocator_type Alloc>
	constexpr basic_dynamic_string<CharType, Alloc>& basic_dynamic_string<CharType, Alloc>::operator+=(const_view_type rhs)
	{
		this->append(rhs);
		return *this;
	}

	template <typename T>
	dynamic_string to_string(const T& value)
	{
		return rsl::format("{}", value);
	}
}

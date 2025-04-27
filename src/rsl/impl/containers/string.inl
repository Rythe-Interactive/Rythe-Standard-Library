#pragma once
#include "string.hpp"

namespace rsl
{
	template <allocator_type Alloc>
	inline string<Alloc>::string() noexcept(is_nothrow_constructible_v<base>) : base() {}

	template <allocator_type Alloc>
	inline string<Alloc>::string(const string& src) noexcept(base::container_base::copy_construct_noexcept) : base(src) {}

	template <allocator_type Alloc>
	inline string<Alloc>::string(string&& src) noexcept : base(src) {}

	template <allocator_type Alloc>
	template <size_type N>
	inline string<Alloc>::string(const value_type (&arr)[N]) noexcept(base::container_base::move_construct_noexcept) : base(arr) {}

	template <allocator_type Alloc>
	template <size_type N>
	inline string<Alloc>::string(value_type (&&arr)[N]) noexcept(base::container_base::copy_construct_noexcept) : base(rsl::move(arr)) {}

	template <allocator_type Alloc>
	inline string<Alloc>::string(view_type src) noexcept(base::container_base::copy_construct_noexcept): base(src) {}

	template <allocator_type Alloc>
	inline string<Alloc>::string(size_type capacity) noexcept : base(capacity) {}

	template <allocator_type Alloc>
	template <size_type N>
	void string<Alloc>::assign(const value_type (&src)[N]) noexcept(base::container_base::copy_assign_noexcept)
	{
		this->reserve(N);
		this->resize(N);
		this->set_ptr(const_cast<char*const>(&src[0]));
	}

	template <allocator_type Alloc>
	template <size_type N>
	void string<Alloc>::assign(value_type (&&src)[N]) noexcept(base::container_base::move_assign_noexcept)
	{
		this->reserve(N);
		this->resize(N);
		this->set_ptr(std::move(const_cast<char*const>(&src[0])));
	}

	template <allocator_type Alloc>
	inline string<Alloc>& string<Alloc>::append(view_type view)
	{
		return *this;
	}

	template <allocator_type Alloc>
	inline string<Alloc>& string<Alloc>::operator+=(view_type rhs)
	{
		return *this;
	}

	template <allocator_type Alloc>
	inline string<Alloc>& string<Alloc>::replace(size_type pos, size_type count, view_type view)
	{
		return *this;
	}
}

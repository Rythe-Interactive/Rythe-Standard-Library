#pragma once
#include "string.hpp"

namespace rsl
{
	template <allocator_type Alloc>
	template <size_type N>
	void string<Alloc>::assign(const value_type (&src)[N]) noexcept(base::container_base::copy_assign_noexcept)
	{
		this->reserve(N);
		this->resize(N);
		// TODO(Rowan): this doesn't work, need to memcpy, not just swap out the pointer.
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

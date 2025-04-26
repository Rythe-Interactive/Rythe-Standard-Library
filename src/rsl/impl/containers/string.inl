#include "string.hpp"

namespace rsl
{
	template <allocator_type Alloc>
	string<Alloc>& string<Alloc>::append(view_type view)
	{
		return *this;
	}

	template <allocator_type Alloc>
	string<Alloc>& string<Alloc>::operator+=(view_type rhs)
	{
		return *this;
	}

	template <allocator_type Alloc>
	string<Alloc>& string<Alloc>::replace(size_type pos, size_type count, view_type view)
	{
		return *this;
	}
}

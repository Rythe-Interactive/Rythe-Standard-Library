#pragma once
#include "allocator_storage.hpp"

namespace rsl
{
	template <allocator_type Alloc>
	inline constexpr allocator_storage<Alloc>::allocator_storage(const Alloc& alloc)
		noexcept(is_nothrow_copy_constructible_v<Alloc>)
		: value(alloc)
	{
	}

	template <allocator_type Alloc>
	inline constexpr allocator_storage<Alloc>::allocator_storage(Alloc&& alloc)
		noexcept(is_nothrow_move_constructible_v<Alloc>)
		: value(std::move(alloc))
	{
	}

	inline constexpr allocator_storage<polymorphic_allocator>::allocator_storage(polymorphic_allocator& alloc) noexcept
		: value(&alloc)
	{
	}
} // namespace rsl

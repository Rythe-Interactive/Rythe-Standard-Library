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

	inline constexpr allocator_storage<polymorphic_allocator>::allocator_storage(const allocator_storage& other
	) noexcept
		: value(other.value)
	{
	}

	inline constexpr allocator_storage<polymorphic_allocator>&
	allocator_storage<polymorphic_allocator>::operator=(const allocator_storage& other) noexcept
	{
		value = other.value;
		return *this;
	}

	inline constexpr allocator_storage<polymorphic_allocator>::allocator_storage(polymorphic_allocator& alloc) noexcept
		: value(&alloc)
	{
	}
} // namespace rsl

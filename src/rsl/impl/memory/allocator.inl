#pragma once
#include "allocator.hpp"

namespace rsl
{
	inline void* polymorphic_allocator_ptr_wrapper::allocate(size_type size) noexcept
	{
		return allocatorPtr->allocate(size);
	}

	inline void* polymorphic_allocator_ptr_wrapper::allocate(size_type size, size_type alignment) noexcept
	{
		return allocatorPtr->allocate(size, alignment);
	}

	inline void* polymorphic_allocator_ptr_wrapper::reallocate(void* ptr, size_type oldSize, size_type newSize) noexcept
	{
		return allocatorPtr->reallocate(ptr, oldSize, newSize);
	}

	inline void* polymorphic_allocator_ptr_wrapper::reallocate(
		void* ptr, size_type oldSize, size_type newSize, size_type alignment
	) noexcept
	{
		return allocatorPtr->reallocate(ptr, oldSize, newSize, alignment);
	}

	inline void polymorphic_allocator_ptr_wrapper::deallocate(void* ptr, size_type size) noexcept
	{
		allocatorPtr->deallocate(ptr, size);
	}

	inline void polymorphic_allocator_ptr_wrapper::deallocate(void* ptr, size_type size, size_type alignment) noexcept
	{
		allocatorPtr->deallocate(ptr, size, alignment);
	}
} // namespace rsl

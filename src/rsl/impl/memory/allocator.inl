#pragma once
#include "allocator.hpp"

namespace rsl
{
	template <allocator_type Alloc>
	inline void* polymorphic_allocator_mixin<Alloc>::allocate(size_type size) noexcept
	{
		return impl.allocate(size);
	}

	template <allocator_type Alloc>
	inline void* polymorphic_allocator_mixin<Alloc>::allocate(size_type size, size_type alignment) noexcept
	{
		return impl.allocate(size, alignment);
	}

	template <allocator_type Alloc>
	inline void*
	polymorphic_allocator_mixin<Alloc>::reallocate(void* ptr, size_type oldSize, size_type newSize) noexcept
	{
		return impl.reallocate(ptr, oldSize, newSize);
	}

	template <allocator_type Alloc>
	inline void* polymorphic_allocator_mixin<Alloc>::reallocate(
		void* ptr, size_type oldSize, size_type newSize, size_type alignment
	) noexcept
	{
		return impl.reallocate(ptr, oldSize, newSize, alignment);
	}

	template <allocator_type Alloc>
	inline void polymorphic_allocator_mixin<Alloc>::deallocate(void* ptr, size_type size) noexcept
	{
		impl.deallocate(ptr, size);
	}

	template <allocator_type Alloc>
	inline void polymorphic_allocator_mixin<Alloc>::deallocate(void* ptr, size_type size, size_type alignment) noexcept
	{
		impl.deallocate(ptr, size, alignment);
	}

	inline constexpr void* polymorphic_allocator_ptr_wrapper::allocate(size_type size) noexcept
	{
		return allocatorPtr->allocate(size);
	}

	inline constexpr void* polymorphic_allocator_ptr_wrapper::allocate(size_type size, size_type alignment) noexcept
	{
		return allocatorPtr->allocate(size, alignment);
	}

	inline constexpr void*
	polymorphic_allocator_ptr_wrapper::reallocate(void* ptr, size_type oldSize, size_type newSize) noexcept
	{
		return allocatorPtr->reallocate(ptr, oldSize, newSize);
	}

	inline constexpr void* polymorphic_allocator_ptr_wrapper::reallocate(
		void* ptr, size_type oldSize, size_type newSize, size_type alignment
	) noexcept
	{
		return allocatorPtr->reallocate(ptr, oldSize, newSize, alignment);
	}

	inline constexpr void polymorphic_allocator_ptr_wrapper::deallocate(void* ptr, size_type size) noexcept
	{
		allocatorPtr->deallocate(ptr, size);
	}

	inline constexpr void
	polymorphic_allocator_ptr_wrapper::deallocate(void* ptr, size_type size, size_type alignment) noexcept
	{
		allocatorPtr->deallocate(ptr, size, alignment);
	}
} // namespace rsl

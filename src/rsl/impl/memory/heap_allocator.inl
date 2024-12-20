#pragma once
#include "heap_allocator.hpp"

namespace rsl
{
	inline constexpr void* heap_allocator::allocate(size_type size) noexcept
	{
		return ::operator new(size, std::nothrow);
	}

	inline constexpr void* heap_allocator::allocate(size_type size, size_type alignment) noexcept
	{
		return ::operator new(size, std::align_val_t{alignment}, std::nothrow);
	}

	inline constexpr void* heap_allocator::reallocate(void* ptr, size_type oldSize, size_type newSize) noexcept
	{
		void* mem = nullptr;

		if (newSize != 0)
		{
			mem = ::operator new(newSize, std::nothrow);
			if (mem)
			{
				std::memcpy(mem, ptr, std::min(oldSize, newSize));
			}
		}

		::operator delete(ptr, oldSize);

		return mem;
	}

	inline constexpr void*
	heap_allocator::reallocate(void* ptr, size_type oldSize, size_type newSize, size_type alignment) noexcept
	{
		void* mem = nullptr;

		if (newSize != 0)
		{
			mem = ::operator new(newSize, std::align_val_t{alignment}, std::nothrow);
			if (mem)
			{
				std::memcpy(mem, ptr, std::min(oldSize, newSize));
			}
		}

		::operator delete(ptr, oldSize, std::align_val_t{alignment});

		return mem;
	}

	inline constexpr void heap_allocator::deallocate(void* ptr, size_type size) noexcept
	{
		::operator delete(ptr, size);
	}

	inline constexpr void heap_allocator::deallocate(void* ptr, size_type size, size_type alignment) noexcept
	{
		::operator delete(ptr, size, std::align_val_t{alignment});
	}
} // namespace rsl

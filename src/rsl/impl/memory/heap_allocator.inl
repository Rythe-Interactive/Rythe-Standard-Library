#pragma once
#include "heap_allocator.hpp"

namespace rsl
{
	inline constexpr void* heap_allocator::allocate(size_type size) noexcept
	{
		if constexpr (is_constant_evaluated())
		{
			return new byte[size];
		}
		else
		{
			return ::operator new(size, std::nothrow);
		}
	}

	inline constexpr void* heap_allocator::allocate(size_type size, size_type alignment) noexcept
	{
		if constexpr (is_constant_evaluated())
		{
			return new byte[size];
		}
		else
		{
			return ::operator new(size, std::align_val_t{alignment}, std::nothrow);
		}
	}

	inline constexpr void* heap_allocator::reallocate(void* ptr, size_type oldSize, size_type newSize) noexcept
	{
		if constexpr (is_constant_evaluated())
		{
			void* mem = nullptr;

			if (newSize != 0)
			{
				mem = new byte[newSize];
				constexpr_memcpy(mem, ptr, oldSize < newSize ? oldSize : newSize);
			}

			delete[] bit_cast<byte*>(ptr);

			return mem;
		}
		else
		{
			void* mem = nullptr;

			if (newSize != 0)
			{
				mem = ::operator new(newSize, std::nothrow);
				if (mem)
				{
					memcpy(mem, ptr, std::min(oldSize, newSize));
				}
			}

			::operator delete(ptr, oldSize);

			return mem;
		}
	}

	inline constexpr void*
	heap_allocator::reallocate(void* ptr, size_type oldSize, size_type newSize, size_type alignment) noexcept
	{
		if constexpr (is_constant_evaluated())
		{
			void* mem = nullptr;

			if (newSize != 0)
			{
				mem = new byte[newSize];
				constexpr_memcpy(mem, ptr, oldSize < newSize ? oldSize : newSize);
			}

			delete[] bit_cast<byte*>(ptr);

			return mem;
		}
		else
		{
			void* mem = nullptr;

			if (newSize != 0)
			{
				mem = ::operator new(newSize, std::align_val_t{alignment}, std::nothrow);
				if (mem)
				{
					memcpy(mem, ptr, std::min(oldSize, newSize));
				}
			}

			::operator delete(ptr, oldSize, std::align_val_t{alignment});

			return mem;
		}
	}

	inline constexpr void heap_allocator::deallocate(void* ptr, size_type size) noexcept
	{
		if constexpr (is_constant_evaluated())
		{
			delete[] bit_cast<byte*>(ptr);
		}
		else
		{
			::operator delete(ptr, size);
		}
	}

	inline constexpr void heap_allocator::deallocate(void* ptr, size_type size, size_type alignment) noexcept
	{
		if constexpr (is_constant_evaluated())
		{
			delete[] bit_cast<byte*>(ptr);
		}
		else
		{
			::operator delete(ptr, size, std::align_val_t{alignment});
		}
	}
} // namespace rsl

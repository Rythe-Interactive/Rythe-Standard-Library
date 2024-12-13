#pragma once

#include "../util/primitives.hpp"

namespace rsl
{
	class heap_allocator
	{
	public:
		using value_type = void;

		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] void* allocate(size_type size) noexcept;
		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] void*
		allocate(size_type size, size_type alignment) noexcept;

		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] void*
		reallocate(void* ptr, size_type oldSize, size_type newSize) noexcept;
		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] void*
		reallocate(void* ptr, size_type oldSize, size_type newSize, size_type alignment) noexcept;

		[[rythe_always_inline]] void deallocate(void* ptr, size_type size) noexcept;
		[[rythe_always_inline]] void deallocate(void* ptr, size_type size, size_type alignment) noexcept;
	};
} // namespace rsl

#include "heap_allocator.inl"

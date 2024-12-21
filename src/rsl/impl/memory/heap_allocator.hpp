#pragma once

#include "../util/common.hpp"
#include "../util/primitives.hpp"

namespace rsl
{
	class heap_allocator
	{
	public:
		using value_type = void;

		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr void* allocate(size_type size) noexcept;
		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr void*
		allocate(size_type size, size_type alignment) noexcept;

		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr void*
		reallocate(void* ptr, size_type oldSize, size_type newSize) noexcept;
		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr void*
		reallocate(void* ptr, size_type oldSize, size_type newSize, size_type alignment) noexcept;

		[[rythe_always_inline]] constexpr void deallocate(void* ptr, size_type size) noexcept;
		[[rythe_always_inline]] constexpr void deallocate(void* ptr, size_type size, size_type alignment) noexcept;
	};
} // namespace rsl

#include "heap_allocator.inl"

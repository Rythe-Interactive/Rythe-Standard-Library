#pragma once

#include "../util/primitives.hpp"

namespace rsl
{
	class mock_allocator
	{
	public:
		using value_type = void;

		[[rythe_always_inline]] constexpr bool is_valid() const noexcept { return false; } //NOLINT

		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] static constexpr void*
		allocate([[maybe_unused]] const size_type size) noexcept { return nullptr; }

		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] static constexpr void*
		allocate([[maybe_unused]] const size_type size, [[maybe_unused]] const size_type alignment) noexcept
		{
			return nullptr;
		}

		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] static constexpr void*
		reallocate([[maybe_unused]] void* const ptr, [[maybe_unused]] const size_type oldSize,
		           [[maybe_unused]] const size_type newSize) noexcept
		{
			return nullptr;
		}

		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] static constexpr void*
		reallocate([[maybe_unused]] void* const ptr, [[maybe_unused]] const size_type oldSize,
		           [[maybe_unused]] const size_type newSize, [[maybe_unused]] const size_type alignment) noexcept
		{
			return nullptr;
		}

		[[rythe_always_inline]] static constexpr void deallocate([[maybe_unused]] void* const ptr,
		                                                  [[maybe_unused]] const size_type size) noexcept {}

		[[rythe_always_inline]] static constexpr void deallocate([[maybe_unused]] void* const ptr,
		                                                  [[maybe_unused]] const size_type size,
		                                                  [[maybe_unused]] const size_type alignment) noexcept {}
	};
} // namespace rsl

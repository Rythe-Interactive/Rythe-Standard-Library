#pragma once

#include "../util/assert.hpp"
#include "../util/common.hpp"
#include "../util/concepts.hpp"
#include "../util/type_util.hpp"

#include "heap_allocator.hpp"

namespace rsl
{
	template <typename T>
	concept noexcept_allocator_type = requires(T alloc, void* ptr, size_type n) {
		{ alloc.allocate(n) } noexcept -> convertible_to<void*>;
		{ alloc.allocate(n, n) } noexcept -> convertible_to<void*>;
		{ alloc.reallocate(ptr, n, n) } noexcept -> convertible_to<void*>;
		{ alloc.reallocate(ptr, n, n, n) } noexcept -> convertible_to<void*>;
		{ alloc.deallocate(ptr, n) } noexcept;
		{ alloc.deallocate(ptr, n, n) } noexcept;
	};

	template <typename T>
	concept allocator_type = requires(T alloc, void* ptr, size_type n) {
		{ alloc.allocate(n) } -> convertible_to<void*>;
		{ alloc.allocate(n, n) } -> convertible_to<void*>;
		{ alloc.reallocate(ptr, n, n) } -> convertible_to<void*>;
		{ alloc.reallocate(ptr, n, n, n) } -> convertible_to<void*>;
		{ alloc.deallocate(ptr, n) };
		{ alloc.deallocate(ptr, n, n) };
	};

#if !defined(RSL_DEFAULT_ALLOCATOR_OVERRIDE)
	using default_allocator = heap_allocator;
#else
	using default_allocator = RSL_DEFAULT_ALLOCATOR_OVERRIDE;
#endif

	class polymorphic_allocator
	{
	public:
		[[nodiscard]] [[rythe_allocating]] virtual void* allocate(size_type size) noexcept = 0;
		[[nodiscard]] [[rythe_allocating]] virtual void* allocate(size_type size, size_type alignment) noexcept = 0;

		[[nodiscard]] [[rythe_allocating]] virtual void*
		reallocate(void* ptr, size_type oldSize, size_type newSize) noexcept = 0;
		[[nodiscard]] [[rythe_allocating]] virtual void*
		reallocate(void* ptr, size_type oldSize, size_type newSize, size_type alignment) noexcept = 0;

		virtual void deallocate(void* ptr, size_type size) noexcept = 0;
		virtual void deallocate(void* ptr, size_type size, size_type alignment) noexcept = 0;
	};

	using pmu_allocator = polymorphic_allocator;

	template <allocator_type Alloc = default_allocator>
	class polymorphic_allocator_mixin final : public polymorphic_allocator
	{
	public:
		using value_type = void;

		Alloc impl;

		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] void* allocate(size_type size) noexcept override
		{
			return impl.allocate(size);
		}

		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] void*
		allocate(size_type size, size_type alignment) noexcept override
		{
			return impl.allocate(size, alignment);
		}

		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] void*
		reallocate(void* ptr, size_type oldSize, size_type newSize) noexcept override
		{
			return impl.reallocate(ptr, oldSize, newSize);
		}

		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] void*
		reallocate(void* ptr, size_type oldSize, size_type newSize, size_type alignment) noexcept override
		{
			return impl.reallocate(ptr, oldSize, newSize, alignment);
		}

		[[rythe_always_inline]] void deallocate(void* ptr, size_type size) noexcept override
		{
			impl.deallocate(ptr, size);
		}

		[[rythe_always_inline]] void deallocate(void* ptr, size_type size, size_type alignment) noexcept override
		{
			impl.deallocate(ptr, size, alignment);
		}
	};

	using default_polymorphic_allocator = polymorphic_allocator_mixin<default_allocator>;
	using default_pmu_allocator = default_polymorphic_allocator;

	class polymorphic_allocator_ptr_wrapper
	{
	public:
		using value_type = void;

		pmu_allocator* allocatorPtr;

		constexpr polymorphic_allocator_ptr_wrapper() noexcept = default;
		constexpr polymorphic_allocator_ptr_wrapper(pmu_allocator* ptr) noexcept
			: allocatorPtr(ptr)
		{
		}


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

	using pmu_alloc_ptr_wrapper = polymorphic_allocator_ptr_wrapper;
} // namespace rsl

#include "allocator.inl"

#pragma once

#include "../util/assert.hpp"
#include "../util/common.hpp"
#include "../util/type_util.hpp"

namespace rsl
{
	template <typename T>
	concept allocator_type = requires(T alloc, typename T::value_type* ptr, size_type n) {
		{ alloc.allocate(n) } -> convertible_to<typename T::value_type*>;
		{ alloc.allocate(n, n) } -> convertible_to<typename T::value_type*>;
		{ alloc.reallocate(ptr, n, n) } -> convertible_to<typename T::value_type*>;
		{ alloc.reallocate(ptr, n, n, n) } -> convertible_to<typename T::value_type*>;
		{ alloc.deallocate(ptr, n) };
		{ alloc.deallocate(ptr, n, n) };
	};

	template <typename T>
	concept noexcept_allocator_type = requires(T alloc, typename T::value_type* ptr, size_type n) {
		{ alloc.allocate(n) } noexcept -> convertible_to<typename T::value_type*>;
		{ alloc.allocate(n, n) } noexcept -> convertible_to<typename T::value_type*>;
		{ alloc.reallocate(ptr, n, n) } noexcept -> convertible_to<typename T::value_type*>;
		{ alloc.reallocate(ptr, n, n, n) } noexcept -> convertible_to<typename T::value_type*>;
		{ alloc.deallocate(ptr, n) } noexcept;
		{ alloc.deallocate(ptr, n, n) } noexcept;
	};

    template<typename T>
	concept universal_allocator_type = allocator_type<T> && same_as<typename T::value_type, void>;

	class heap_allocator final
	{
	public:
        using value_type = void;

		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] void* allocate(size_type size) noexcept;
		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] void*
		allocate(size_type size, size_type alignment) noexcept;

		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] virtual void*
		reallocate(void* ptr, size_type oldSize, size_type newSize) noexcept;
		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] virtual void*
		reallocate(void* ptr, size_type oldSize, size_type newSize, size_type alignment) noexcept;

		[[rythe_always_inline]] void deallocate(void* ptr, size_type size) noexcept;
		[[rythe_always_inline]] void deallocate(void* ptr, size_type size, size_type alignment) noexcept;
	};

#if !defined(RSL_DEFAULT_ALLOCATOR_OVERRIDE)
	using default_allocator = heap_allocator;
#else
	using default_allocator = RSL_DEFAULT_ALLOCATOR_OVERRIDE;
#endif

    template<typename T>
    class default_constructor final
    {
	public:
        using value_type = T;

        template<typename Other>
        using retarget = default_constructor<Other>;

        template<typename... Args>
		T* construct(void* ptr, size_type count, Args&&... args) noexcept(is_nothrow_constructible_v<T, Args...>);

        T* move(void* dst, T* src, size_type count) noexcept(is_nothrow_move_constructible_v<T>);

		void destroy(T* ptr, size_type count) noexcept;
    };

	template <typename T>
	concept constructor_type = requires(T ctor, void* mem, typename T::value_type* ptr, size_type n) {
		{ ctor.construct(mem, n) } -> convertible_to<typename T::value_type*>;
		{ ctor.move(mem, ptr, n) } -> convertible_to<typename T::value_type*>;
		{ ctor.destroy(ptr, n) };
	};

	template <typename T>
	concept noexcept_constructor_type = requires(T ctor, void* mem, typename T::value_type* ptr, size_type n) {
		{ ctor.construct(mem, n) } noexcept -> convertible_to<typename T::value_type*>;
		{ ctor.move(mem, ptr, n) } noexcept -> convertible_to<typename T::value_type*>;
		{ ctor.destroy(ptr, n) } noexcept;
	};

	template <typename T, universal_allocator_type UniversalAlloc = default_allocator, constructor_type Constructor = default_constructor<T>>
	class allocator final : private UniversalAlloc, private Constructor
	{
	public:
        using value_type = T;

        template<typename... Args>
		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] T*
		allocate(size_type count = 1, Args&&... args) noexcept(is_nothrow_invocable_v<Constructor::construct, void*, size_type, Args...>);
		template <typename... Args>
		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] T* allocate(
			size_type count, size_type alignment, Args&&... args
		) noexcept(is_nothrow_invocable_v<Constructor::construct, void*, size_type, Args...>);

		template <typename... Args>
		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] T* reallocate(
			T* ptr, size_type oldCount, size_type newCount, Args&&... args
		) noexcept(is_nothrow_invocable_v<Constructor::construct, void*, size_type, Args...> && is_nothrow_invocable_v<Constructor::move, void*, T*, size_type>);

		template <typename... Args>
		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] T* reallocate(
			T* ptr, size_type oldCount, size_type newCount, size_type alignment, Args&&... args
			) noexcept(is_nothrow_invocable_v<Constructor::construct, void*, size_type, Args...> && is_nothrow_invocable_v<Constructor::move, void*, T*, size_type>);

		[[rythe_always_inline]] void deallocate(T* ptr, size_type count = 1) noexcept;
		[[rythe_always_inline]] void deallocate(T* ptr, size_type count, size_type alignment) noexcept;
        
		template <typename Other>
		using retarget = allocator<Other, UniversalAlloc, typename Constructor::template retarget<Other>>;
	};
} // namespace rsl

#include "allocator.inl"

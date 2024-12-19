#pragma once

#include "allocator.hpp"
#include "factory.hpp"

namespace rsl
{
	template <typename T, allocator_type Alloc = default_allocator, factory_type Factory = default_factory<T>>
	class typed_allocator final
	{
	public:
		using value_type = T;
		using universal_type = Alloc;

		typed_allocator() = default;
		typed_allocator(Alloc* uAlloc, Factory* construct = {})
			: Alloc(uAlloc),
			  Factory(construct)
		{
		}

		universal_type& as_universal() noexcept;
		const universal_type& as_universal() const noexcept;

		template <typename... Args>
		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] T* allocate(size_type count = 1, Args&&... args)
			noexcept(factory_traits<Factory>::template noexcept_constructable<Args...>);

		template <typename... Args>
		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] T*
		allocate(size_type count, size_type alignment, Args&&... args)
			noexcept(factory_traits<Factory>::template noexcept_constructable<Args...>);

		template <typename... Args>
		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] T*
		reallocate(T* ptr, size_type oldCount, size_type newCount, Args&&... args) noexcept(
			factory_traits<Factory>::template noexcept_constructable<Args...> &&
			factory_traits<Factory>::noexcept_moveable
		);

		template <typename... Args>
		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] T*
		reallocate(T* ptr, size_type oldCount, size_type newCount, size_type alignment, Args&&... args) noexcept(
			factory_traits<Factory>::template noexcept_constructable<Args...> &&
			factory_traits<Factory>::noexcept_moveable
		);

		[[rythe_always_inline]] void deallocate(T* ptr, size_type count = 1) noexcept;
		[[rythe_always_inline]] void deallocate(T* ptr, size_type count, size_type alignment) noexcept;

		template <typename Other>
		using retarget = typed_allocator<Other, Alloc, Factory>;
	};
} // namespace rsl

#include "typed_allocator.inl"

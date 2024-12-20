#pragma once
#include "allocator.hpp"

namespace rsl
{
	template <allocator_type Alloc>
	struct allocator_storage final
	{
		[[rythe_always_inline]] constexpr allocator_storage() noexcept(is_nothrow_constructible_v<Alloc>) = default;

		[[rythe_always_inline]] constexpr allocator_storage(const Alloc& alloc)
			noexcept(is_nothrow_copy_constructible_v<Alloc>);
		[[rythe_always_inline]] constexpr allocator_storage(Alloc&& alloc)
			noexcept(is_nothrow_move_constructible_v<Alloc>);

		[[rythe_always_inline]] constexpr Alloc& operator*() noexcept { return value; }
		[[rythe_always_inline]] constexpr const Alloc& operator*() const noexcept { return value; }
		[[rythe_always_inline]] constexpr Alloc* operator->() noexcept { return &value; }
		[[rythe_always_inline]] constexpr const Alloc* operator->() const noexcept { return &value; }

		Alloc value;
	};

	template <>
	struct allocator_storage<polymorphic_allocator> final
	{
		allocator_storage() noexcept;
		[[rythe_always_inline]] constexpr allocator_storage(const allocator_storage& other) noexcept;
		[[rythe_always_inline]] constexpr allocator_storage& operator=(const allocator_storage& other) noexcept;

		[[rythe_always_inline]] constexpr allocator_storage(polymorphic_allocator& alloc) noexcept;

		[[rythe_always_inline]] constexpr polymorphic_allocator& operator*() noexcept { return *value; }
		[[rythe_always_inline]] constexpr const polymorphic_allocator& operator*() const noexcept { return *value; }
		[[rythe_always_inline]] constexpr polymorphic_allocator* operator->() noexcept { return value; }
		[[rythe_always_inline]] constexpr const polymorphic_allocator* operator->() const noexcept { return value; }

		mutable polymorphic_allocator* value;
	};
} // namespace rsl

#include "allocator_storage.inl"

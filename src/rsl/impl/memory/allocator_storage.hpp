#pragma once
#include "allocator.hpp"

namespace rsl
{
    template <allocator_type Alloc>
    struct allocator_storage;

    template <allocator_type Alloc>
        requires (!is_empty_v<Alloc>)
    struct allocator_storage<Alloc> final
    {
        [[rythe_always_inline]] constexpr allocator_storage() noexcept(is_nothrow_constructible_v<Alloc>) = default;

        [[rythe_always_inline]] constexpr allocator_storage(const Alloc& alloc)
            noexcept(is_nothrow_copy_constructible_v<Alloc>) : value(alloc) {}

        [[rythe_always_inline]] constexpr allocator_storage(Alloc&& alloc)
            noexcept(is_nothrow_move_constructible_v<Alloc>) : value(rsl::move(alloc)) {}

        [[rythe_always_inline]] constexpr Alloc& operator*() noexcept { return value; }
        [[rythe_always_inline]] constexpr const Alloc& operator*() const noexcept { return value; }
        [[rythe_always_inline]] constexpr Alloc* operator->() noexcept { return &value; }
        [[rythe_always_inline]] constexpr const Alloc* operator->() const noexcept { return &value; }

        [[rythe_always_inline]] constexpr operator bool() const noexcept { return value.is_valid(); }

        Alloc value;
    };

    template <allocator_type Alloc>
        requires (is_empty_v<Alloc>)
    struct allocator_storage<Alloc> final
    {
        [[rythe_always_inline]] constexpr allocator_storage() noexcept = default;

        [[rythe_always_inline]] constexpr allocator_storage(const Alloc&) noexcept {}
        [[rythe_always_inline]] constexpr allocator_storage(Alloc&&) noexcept {}

        [[rythe_always_inline]] constexpr Alloc& operator*() noexcept { return *address_of_empty<Alloc>(); }
        [[rythe_always_inline]] constexpr const Alloc& operator*() const noexcept { return *address_of_empty<Alloc>(); }
        [[rythe_always_inline]] constexpr Alloc* operator->() noexcept { return address_of_empty<Alloc>(); }
        [[rythe_always_inline]] constexpr const Alloc* operator->() const noexcept { return address_of_empty<Alloc>(); }

        [[rythe_always_inline]] constexpr operator bool() const noexcept { return Alloc{}.is_valid(); }
    };

    template <>
    struct allocator_storage<polymorphic_allocator> final
    {
        allocator_storage() noexcept;
        [[rythe_always_inline]] constexpr allocator_storage(const allocator_storage& other) noexcept = default;
        [[rythe_always_inline]] constexpr allocator_storage& operator=(const allocator_storage& other) noexcept = default;

        [[rythe_always_inline]] constexpr allocator_storage(polymorphic_allocator* alloc) noexcept : value(alloc) {}

        [[rythe_always_inline]] constexpr polymorphic_allocator& operator*() noexcept { return *value; }
        [[rythe_always_inline]] constexpr const polymorphic_allocator& operator*() const noexcept { return *value; }
        [[rythe_always_inline]] constexpr polymorphic_allocator* operator->() noexcept { return value; }
        [[rythe_always_inline]] constexpr const polymorphic_allocator* operator->() const noexcept { return value; }

        [[rythe_always_inline]] constexpr operator bool() const noexcept { return value && value->is_valid(); }

        mutable polymorphic_allocator* value;
    };
} // namespace rsl

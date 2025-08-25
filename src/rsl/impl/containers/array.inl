#pragma once

namespace rsl
{
    template <typename T, allocator_type Alloc, typed_factory_type Factory, size_type StaticCapacity, bool CanResize>
    constexpr basic_dynamic_array<T, Alloc, Factory, StaticCapacity, CanResize>::basic_dynamic_array(
            const container_base& src
            ) noexcept(container_base::copy_construct_container_noexcept)
        : container_base(src) {}

    template <typename T, allocator_type Alloc, typed_factory_type Factory, size_type StaticCapacity, bool CanResize>
    constexpr basic_dynamic_array<T, Alloc, Factory, StaticCapacity, CanResize>::basic_dynamic_array(
            container_base&& src
            ) noexcept(container_base::move_construct_container_noexcept)
        : container_base(rsl::move(src)) {}
} // namespace rsl

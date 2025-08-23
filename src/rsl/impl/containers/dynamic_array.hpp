#pragma once

#include "../util/primitives.hpp"

#include "contiguous_container_base.hpp"

namespace rsl
{
    template <typename T, allocator_type Alloc = default_allocator, typed_factory_type Factory = default_factory<T>, size_type
              StaticCapacity = 0ull, bool CanResize = true>
    class basic_dynamic_array
            : public contiguous_container_base<T, Alloc, Factory, T*, const T*, contiguous_container_info<
                                                   false, StaticCapacity, !is_same_v<Alloc, mock_allocator>, CanResize>>
    {
    public:
        using container_base = contiguous_container_base<
            T, Alloc, Factory, T*, const T*, contiguous_container_info<
                false, StaticCapacity, !is_same_v<Alloc, mock_allocator>, CanResize>>;
        using mem_rsc = typename container_base::mem_rsc;
        using value_type = T;
        using iterator_type = typename container_base::iterator_type;
        using const_iterator_type = typename container_base::const_iterator_type;
        using reverse_iterator_type = typename container_base::reverse_iterator_type;
        using const_reverse_iterator_type = typename container_base::const_reverse_iterator_type;
        using view_type = typename container_base::view_type;
        using const_view_type = typename container_base::const_view_type;
        using allocator_storage_type = allocator_storage<Alloc>;
        using allocator_t = Alloc;
        using factory_storage_type = factory_storage<Factory>;
        using factory_t = Factory;

        using container_base::contiguous_container_base;

        using container_base::operator view_type;
        using container_base::operator const_view_type;

        using container_base::operator[];
        using container_base::operator=;
        using container_base::operator==;
        using container_base::operator!=;
    };

    template <typename T, allocator_type Alloc = default_allocator, typed_factory_type Factory = default_factory<T>>
    using dynamic_array = basic_dynamic_array<T, Alloc, Factory>;

    template <typename T, size_type StaticCapacity, allocator_type Alloc = default_allocator, typed_factory_type Factory =
              default_factory<T>>
    using hybrid_array = basic_dynamic_array<T, Alloc, Factory, StaticCapacity>;

    template <typename T, size_type StaticCapacity, typed_factory_type Factory = default_factory<T>>
    using static_array = basic_dynamic_array<T, mock_allocator, Factory, StaticCapacity>;

    template <typename T, size_type StaticCapacity, typed_factory_type Factory = default_factory<T>>
    using array = basic_dynamic_array<T, mock_allocator, Factory, StaticCapacity, false>;
} // namespace rsl

#include "dynamic_array.inl"

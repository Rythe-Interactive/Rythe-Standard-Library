#pragma once

namespace rsl
{
    template <typename T, allocator_type Alloc, typed_factory_type Factory, size_type StaticCapacity, bool CanResize>
    template <size_type N>
    constexpr basic_dynamic_array<T, Alloc, Factory, StaticCapacity, CanResize> basic_dynamic_array<
        T, Alloc, Factory, StaticCapacity, CanResize>::from_array(
            const value_type (& arr)[N]
            )
        noexcept
        (container_base::copy_construct_noexcept)
    {
        basic_dynamic_array result;
        result.reserve(N);
        result.m_size = N;
        result.copy_construct_from_unsafe_impl(0ull, result.m_size, &arr[0]);
        return result;
    }

    template <typename T, allocator_type Alloc, typed_factory_type Factory, size_type StaticCapacity, bool CanResize>
    template <size_type N>
    constexpr basic_dynamic_array<T, Alloc, Factory, StaticCapacity, CanResize> basic_dynamic_array<
        T, Alloc, Factory, StaticCapacity, CanResize>::from_array(
            value_type (&& arr)[N]
            )
        noexcept
        (container_base::move_construct_noexcept)
    {
        basic_dynamic_array result;
        result.reserve(N);
        result.m_size = N;
        result.move_construct_from_unsafe_impl(0ull, result.m_size, &arr[0]);
        return result;
    }

    template <typename T, allocator_type Alloc, typed_factory_type Factory, size_type StaticCapacity, bool CanResize>
    constexpr basic_dynamic_array<T, Alloc, Factory, StaticCapacity, CanResize> basic_dynamic_array<
        T, Alloc, Factory, StaticCapacity, CanResize>::from_buffer(
            const value_type* ptr,
            size_type count
            ) noexcept(container_base::copy_construct_noexcept)
    {
        basic_dynamic_array result;
        result.reserve(count);
        result.m_size = count;
        result.copy_construct_from_unsafe_impl(0ull, result.m_size, ptr);
        return result;
    }

    template <typename T, allocator_type Alloc, typed_factory_type Factory, size_type StaticCapacity, bool CanResize>
    constexpr basic_dynamic_array<T, Alloc, Factory, StaticCapacity, CanResize> basic_dynamic_array<
        T, Alloc, Factory, StaticCapacity, CanResize>::from_view(
            const_view_type src
            ) noexcept(container_base::copy_construct_noexcept)
    {
        basic_dynamic_array result;
        result.reserve(src.size());
        result.m_size = src.size();
        result.copy_construct_from_unsafe_impl(0ull, result.m_size, src.data());
        return result;
    }

    template <typename T, allocator_type Alloc, typed_factory_type Factory, size_type StaticCapacity, bool CanResize>
    constexpr basic_dynamic_array<T, Alloc, Factory, StaticCapacity, CanResize>
        basic_dynamic_array<T, Alloc, Factory, StaticCapacity, CanResize>::create_reserved(const size_type capacity) noexcept
            requires(container_base::can_allocate)
    {
        basic_dynamic_array result;
        result.reserve(capacity);
        return result;
    }

    template <typename T, allocator_type Alloc, typed_factory_type Factory, size_type StaticCapacity, bool CanResize>
    template <typename... Args>
    constexpr basic_dynamic_array<T, Alloc, Factory, StaticCapacity, CanResize> basic_dynamic_array<
        T, Alloc, Factory, StaticCapacity, CanResize>::create_in_place(
            size_type count,
            Args&&... args
            )
        noexcept(container_base::template construct_noexcept<Args...>)
    {
        basic_dynamic_array result;
        result.reserve(count);
        result.m_size = count;
        result.construct(count, 0ull, rsl::forward<Args>(args)...);
        return result;
    }
} // namespace rsl

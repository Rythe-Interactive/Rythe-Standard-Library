#pragma once

#include "../util/primitives.hpp"
#include "../util/type_util.hpp"
#include "optional.hpp"

namespace rsl
{
    template <
        typename T, typename Alloc, bool AllowRawDataAccess = false>
    struct memory_resource_base
    {
        using value_type = T;
        using ref_type = add_lval_ref_t<T>;
        using const_ref_type = decorate_type_t<T, const_signal, lval_ref_signal>;
        using ptr_type = add_pointer_t<T>;
        using const_ptr_type = decorate_type_t<T, const_signal, pointer_signal>;
        using allocator_type = Alloc;

        virtual ~memory_resource_base() = default;

        [[nodiscard]] constexpr ptr_type data() noexcept
            requires AllowRawDataAccess;

        [[nodiscard]] constexpr const_ptr_type data() const noexcept
            requires AllowRawDataAccess;

    protected:
        constexpr void allocate(size_type n = 1);
        constexpr size_type allocate_at_least(size_type n);
        template<typename... Args>
        constexpr void construct(size_type n = 1, diff_type offset = 0, Args&&... args);
        constexpr void deallocate(size_type n = 1);
        constexpr void destroy();

        allocator_type m_alloc;
        ptr_type m_ptr;
    };
} // namespace rsl

#include "memory_resource_base.inl"

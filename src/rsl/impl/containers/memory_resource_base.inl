#include "memory_resource_base.hpp"
#pragma once

namespace rsl
{
#define MRB_TEMPLATE_ARGS typename T, typename Alloc, bool AllowRawDataAccess

#define MRB_PTR_TYPE typename memory_resource_base<T, Alloc, AllowRawDataAccess>::ptr_type

#define MRB_CONST_PTR_TYPE typename memory_resource_base<T, Alloc, AllowRawDataAccess>::const_ptr_type

#define MRB_TYPE memory_resource_base<T, Alloc, AllowRawDataAccess>

    template <MRB_TEMPLATE_ARGS>
    inline constexpr MRB_PTR_TYPE MRB_TYPE::data() noexcept
        requires AllowRawDataAccess
    {
        return m_data;
    }

    template <MRB_TEMPLATE_ARGS>
    inline constexpr MRB_CONST_PTR_TYPE MRB_TYPE::data() const noexcept
        requires AllowRawDataAccess
    {
        return m_data;
    }

    template <MRB_TEMPLATE_ARGS>
    inline constexpr void MRB_TYPE::allocate(size_type n)
    {
        m_ptr = m_alloc.allocate(n);
    }
    
    template <MRB_TEMPLATE_ARGS>
    inline constexpr size_type MRB_TYPE::allocate_at_least(size_type n)
    {
        auto [ptr, count] = m_alloc.allocate_at_least(n);
        m_ptr = ptr;
        return count;
    }

    template <MRB_TEMPLATE_ARGS>
    template<typename... Args>
    inline constexpr void MRB_TYPE::construct(size_type n, diff_type offset, Args&&... args)
    {
        m_alloc.construct(m_ptr + offset, n, std::forward<Args>(args...));
    }

    template <MRB_TEMPLATE_ARGS>
    inline constexpr void MRB_TYPE::deallocate(size_type n)
    {
        m_alloc.deallocate(m_ptr, n);
    }

    template <MRB_TEMPLATE_ARGS>
    inline constexpr void MRB_TYPE::destroy()
    {
        m_alloc.destroy(m_ptr);
    }

#undef MRB_TEMPLATE_ARGS
#undef MRB_VALUE_TYPE
#undef MRB_TYPE
} // namespace rsl

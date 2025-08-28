#pragma once
#include "../logging/fmt_include.hpp"

namespace rsl
{
    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    constexpr basic_dynamic_string<CharType, Alloc, StaticCapacity>::basic_dynamic_string(
            const container_base& src
            ) noexcept(container_base::copy_construct_container_noexcept)
        : container_base(src) {}

    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    constexpr basic_dynamic_string<CharType, Alloc, StaticCapacity>::basic_dynamic_string(
            container_base&& src
            ) noexcept(container_base::move_construct_container_noexcept)
        : container_base(rsl::move(src)) {}

    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    constexpr basic_dynamic_string<CharType, Alloc, StaticCapacity>& basic_dynamic_string<CharType, Alloc, StaticCapacity>::operator+=(
            const_view_type rhs
            )
    {
        this->append(rhs);
        return *this;
    }

    template <typename T>
    dynamic_string to_string(const T& value)
    {
        return rsl::format("{}", value);
    }
}

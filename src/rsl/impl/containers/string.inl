#pragma once
#include "../logging/fmt_include.hpp"

namespace rsl
{
    template <typename CharType = char, allocator_type Alloc = default_allocator, size_type StaticCapacity = 0ull>
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

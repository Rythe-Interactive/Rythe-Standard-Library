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

    inline char to_upper(const char ch)
    {
        return static_cast<char>(toupper(static_cast<unsigned char>(ch)));
    }

    inline char to_lower(const char ch)
    {
        return static_cast<char>(tolower(static_cast<unsigned char>(ch)));
    }

    inline dynamic_string to_upper(const dynamic_string::const_view_type str)
    {
        dynamic_string result;
        result.resize(str.size());

        for (size_type i = 0; i < str.size(); i++)
        {
            result[i] = to_upper(str[i]);
        }

        return result;
    }

    inline dynamic_string to_lower(const dynamic_string::const_view_type str)
    {
        dynamic_string result;
        result.resize(str.size());

        for (size_type i = 0; i < str.size(); i++)
        {
            result[i] = to_lower(str[i]);
        }

        return result;
    }

    template <typename T>
    dynamic_string to_string(const T& value)
    {
        return rsl::format("{}", value);
    }
}

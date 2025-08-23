#pragma once
#include "dynamic_array.hpp"

namespace rsl
{
    // TODO(Rowan): constexpr strings are still useful if the string never leaks from constant evaluation time to runtime. constexpr_string is needed if you want to be able to read the results of a string at runtime.
    template <typename CharType = char, allocator_type Alloc = default_allocator, size_type StaticCapacity = 0ull>
    class basic_dynamic_string final
            : public contiguous_container_base<CharType, Alloc, default_factory<CharType>, CharType*, const CharType*,
                                               contiguous_container_info<
                                                   true, StaticCapacity, !is_same_v<Alloc, mock_allocator>, true>>
    {
    public:
        using container_base = contiguous_container_base<
            CharType, Alloc, default_factory<CharType>, CharType*, const CharType*, contiguous_container_info<
                true, StaticCapacity, !is_same_v<Alloc, mock_allocator>, true>>;
        using mem_rsc = typename container_base::mem_rsc;
        using value_type = CharType;
        using iterator_type = typename container_base::iterator_type;
        using const_iterator_type = typename container_base::const_iterator_type;
        using reverse_iterator_type = typename container_base::reverse_iterator_type;
        using const_reverse_iterator_type = typename container_base::const_reverse_iterator_type;
        using view_type = typename container_base::view_type;
        using const_view_type = typename container_base::const_view_type;
        using allocator_storage_type = allocator_storage<Alloc>;
        using allocator_t = Alloc;
        using factory_storage_type = factory_storage<default_factory<CharType>>;
        using factory_t = default_factory<CharType>;

        using container_base::contiguous_container_base;

        using container_base::operator view_type;
        using container_base::operator const_view_type;

        using container_base::operator[];
        using container_base::operator=;
        using container_base::operator==;
        using container_base::operator!=;

        // operator+=
        [[rythe_always_inline]] constexpr basic_dynamic_string& operator+=(const_view_type rhs);
    };

    using dynamic_string = basic_dynamic_string<>;

    template <size_type StaticCapacity>
    using hybrid_string = basic_dynamic_string<char, default_allocator, StaticCapacity>;

    template <size_type StaticCapacity>
    using static_string = basic_dynamic_string<char, mock_allocator, StaticCapacity>;

    using dynamic_wstring = basic_dynamic_string<utf16>;

    dynamic_wstring to_utf16(dynamic_string::const_view_type str);

    template <typename T>
    [[nodiscard]] [[rythe_always_inline]] dynamic_string to_string(const T& value);
} // namespace rsl

#include "string.inl"

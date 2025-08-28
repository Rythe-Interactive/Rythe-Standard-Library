#pragma once

namespace rsl
{
    template <char_type CharType>
    constexpr basic_hashed_string_view<CharType> basic_hashed_string_view<CharType>::from_value(CharType& src) noexcept
    {
        basic_hashed_string_view result;
        result.string = string_view_type::from_value(src);
        result.hash = hash_string(result.string);

        return result;
    }

    template <char_type CharType>
    template <size_type N>
    constexpr basic_hashed_string_view<CharType> basic_hashed_string_view<CharType>::from_array(CharType (& arr)[N]) noexcept
    {
        basic_hashed_string_view result;
        result.string = string_view_type::from_array(arr);
        result.hash = hash_string(result.string);

        return result;
    }

    template <char_type CharType>
    constexpr basic_hashed_string_view<CharType> basic_hashed_string_view<CharType>::from_buffer(
            const value_type* ptr,
            size_type count
            ) noexcept
    {
        basic_hashed_string_view result;
        result.string = string_view_type::from_buffer(ptr, count);
        result.hash = hash_string(result.string);

        return result;
    }

    template <char_type CharType>
    template <contiguous_iterator It>
    constexpr basic_hashed_string_view<CharType> basic_hashed_string_view<CharType>::from_iterator_pair(
            It first,
            It last
            ) noexcept(iter_noexcept_deref<It> && iter_noexcept_diff<It>)
        requires same_as<iter_pointer_t<It>, CharType*>
    {
        basic_hashed_string_view result;
        result.string = string_view_type::from_iterator_pair(first, last);
        result.hash = hash_string(result.string);

        return result;
    }

    template <char_type CharType>
    constexpr basic_hashed_string_view<CharType> basic_hashed_string_view<CharType>::from_view(string_view_type src) noexcept
    {
        basic_hashed_string_view result;
        result.string = src;
        result.hash = hash_string(result.string);

        return result;
    }

    template <char_type CharType>
    constexpr basic_hashed_string_view<CharType> basic_hashed_string_view<CharType>::from_string_length(
            CharType* str,
            CharType terminator
            ) noexcept
    {
        basic_hashed_string_view result;
        result.string = string_view_type::from_string_length(str, terminator);
        result.hash = hash_string(result.string);

        return result;
    }

    template <char_type CharType>
    constexpr typename basic_hashed_string_view<CharType>::iterator_type basic_hashed_string_view<CharType>::begin() noexcept
    {
        return string.begin();
    }

    template <char_type CharType>
    constexpr typename basic_hashed_string_view<CharType>::const_iterator_type basic_hashed_string_view<CharType>::
    begin() const noexcept
    {
        return string.begin();
    }

    template <char_type CharType>
    constexpr typename basic_hashed_string_view<CharType>::const_iterator_type basic_hashed_string_view<CharType>::
    cbegin() const noexcept
    {
        return string.cbegin();
    }

    template <char_type CharType>
    constexpr typename basic_hashed_string_view<CharType>::iterator_type basic_hashed_string_view<CharType>::end() noexcept
    {
        return string.end();
    }

    template <char_type CharType>
    constexpr typename basic_hashed_string_view<CharType>::const_iterator_type basic_hashed_string_view<CharType>::
    end() const noexcept
    {
        return string.end();
    }

    template <char_type CharType>
    constexpr typename basic_hashed_string_view<CharType>::const_iterator_type basic_hashed_string_view<CharType>::
    cend() const noexcept
    {
        return string.cend();
    }

    template <char_type CharType>
    constexpr typename basic_hashed_string_view<CharType>::reverse_iterator_type basic_hashed_string_view<CharType>::
    rbegin() noexcept
    {
        return string.rbegin();
    }

    template <char_type CharType>
    constexpr typename basic_hashed_string_view<CharType>::const_reverse_iterator_type basic_hashed_string_view<CharType>::
    rbegin() const noexcept
    {
        return string.rbegin();
    }

    template <char_type CharType>
    constexpr typename basic_hashed_string_view<CharType>::const_reverse_iterator_type basic_hashed_string_view<CharType>::
    crbegin() const noexcept
    {
        return string.crbegin();
    }

    template <char_type CharType>
    constexpr typename basic_hashed_string_view<CharType>::reverse_iterator_type basic_hashed_string_view<CharType>::rend() noexcept
    {
        return string.rend();
    }

    template <char_type CharType>
    constexpr typename basic_hashed_string_view<CharType>::const_reverse_iterator_type basic_hashed_string_view<CharType>::
    rend() const noexcept
    {
        return string.rend();
    }

    template <char_type CharType>
    constexpr typename basic_hashed_string_view<CharType>::const_reverse_iterator_type basic_hashed_string_view<CharType>::
    crend() const noexcept
    {
        return string.crend();
    }

    template <char_type CharType>
    constexpr typename basic_hashed_string_view<CharType>::reference basic_hashed_string_view<CharType>::front()
    {
        return string.front();
    }

    template <char_type CharType>
    constexpr typename basic_hashed_string_view<CharType>::reference basic_hashed_string_view<CharType>::front() const
    {
        return string.front();
    }

    template <char_type CharType>
    constexpr typename basic_hashed_string_view<CharType>::reference basic_hashed_string_view<CharType>::back()
    {
        return string.back();
    }

    template <char_type CharType>
    constexpr typename basic_hashed_string_view<CharType>::reference basic_hashed_string_view<CharType>::back() const
    {
        return string.back();
    }

    template <char_type CharType>
    constexpr typename basic_hashed_string_view<CharType>::reference basic_hashed_string_view<CharType>::at(const size_type index)
    {
        return string.at(index);
    }

    template <char_type CharType>
    constexpr typename basic_hashed_string_view<CharType>::reference basic_hashed_string_view<CharType>::at(size_type index) const
    {
        return string.at(index);
    }

    template <char_type CharType>
    constexpr typename basic_hashed_string_view<CharType>::reference basic_hashed_string_view<CharType>::operator[](size_type index)
    {

        return string[index];
    }

    template <char_type CharType>
    constexpr typename basic_hashed_string_view<CharType>::reference basic_hashed_string_view<CharType>::operator[](
            size_type index
            ) const
    {
        return string[index];
    }

    template <char_type CharType>
    constexpr typename basic_hashed_string_view<CharType>::pointer basic_hashed_string_view<CharType>::data() noexcept
    {
        return string.data();
    }

    template <char_type CharType>
    constexpr typename basic_hashed_string_view<CharType>::pointer basic_hashed_string_view<CharType>::data() const noexcept
    {
        return string.data();
    }

    template <char_type CharType>
    constexpr size_type basic_hashed_string_view<CharType>::size() const noexcept
    {
        return string.size();
    }

    template <char_type CharType>
    constexpr size_type basic_hashed_string_view<CharType>::size_bytes() const noexcept
    {
        return string.size_bytes();
    }

    template <char_type CharType>
    constexpr bool basic_hashed_string_view<CharType>::empty() const noexcept
    {
        return string.empty();
    }

    template <char_type CharType>
    constexpr typename basic_hashed_string_view<CharType>::string_view_type basic_hashed_string_view<CharType>::
    view() const noexcept
    {
        return string.view();
    }

    [[rythe_always_inline]] constexpr auto operator<=>(hashed_string_view lhs, hashed_string_view rhs) noexcept
    {
        return lhs.hash <=> rhs.hash;
    }

    [[rythe_always_inline]] constexpr auto operator==(hashed_string_view lhs, hashed_string_view rhs) noexcept
    {
        return lhs.hash == rhs.hash;
    }

    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    constexpr basic_hashed_string<CharType, Alloc, StaticCapacity> basic_hashed_string<CharType, Alloc, StaticCapacity>::from_value(
            value_type& src
            ) noexcept(noexcept(string_type::from_value(src)))
    {
        basic_hashed_string result;
        result.string = string_type::from_value(src);
        result.hash = hash_string(result.string);

        return result;
    }

    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    template <size_type N>
    constexpr basic_hashed_string<CharType, Alloc, StaticCapacity> basic_hashed_string<CharType, Alloc, StaticCapacity>::from_array(
            const value_type (& arr)[N]
            ) noexcept(noexcept(string_type::from_array(arr)))
    {
        basic_hashed_string result;
        result.string = string_type::from_array(arr);
        result.hash = hash_string(result.string);

        return result;
    }

    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    template <size_type N>
    constexpr basic_hashed_string<CharType, Alloc, StaticCapacity> basic_hashed_string<CharType, Alloc, StaticCapacity>::from_array(
            value_type (&& arr)[N]
            ) noexcept(noexcept(string_type::from_array(rsl::move(arr))))
    {
        basic_hashed_string result;
        result.string = string_type::from_array(rsl::move(arr));
        result.hash = hash_string(result.string);

        return result;
    }

    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    constexpr basic_hashed_string<CharType, Alloc, StaticCapacity> basic_hashed_string<CharType, Alloc, StaticCapacity>::from_buffer(
            const value_type* ptr,
            size_type count
            ) noexcept(noexcept(string_type::from_buffer(nullptr, size_type{ 0 })))
    {
        basic_hashed_string result;
        result.string = string_type::from_buffer(ptr, count);
        result.hash = hash_string(result.string);

        return result;
    }

    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    constexpr basic_hashed_string<CharType, Alloc, StaticCapacity> basic_hashed_string<CharType, Alloc, StaticCapacity>::from_view(
            const_view_type src
            ) noexcept(noexcept(string_type::from_view(const_view_type{})))
    {
        basic_hashed_string result;
        result.string = string_type::from_view(src);
        result.hash = hash_string(result.string);

        return result;
    }

    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    constexpr basic_hashed_string<CharType, Alloc, StaticCapacity> basic_hashed_string<CharType, Alloc, StaticCapacity>::
    from_string_length(const CharType* str, CharType terminator) noexcept
    {
        return from_view(const_view_type::from_string_length(str, terminator));
    }

    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    constexpr basic_hashed_string<CharType, Alloc, StaticCapacity> basic_hashed_string<CharType, Alloc, StaticCapacity>::
    create_reserved(const size_type capacity) noexcept
        requires(can_allocate)
    {
        basic_hashed_string result;
        result.string.reserve(capacity);
        return result;
    }

    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    constexpr size_type basic_hashed_string<CharType, Alloc, StaticCapacity>::size() const noexcept
    {
        return string.size();
    }

    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    constexpr bool basic_hashed_string<CharType, Alloc, StaticCapacity>::empty() const noexcept
    {
        return string.empty();
    }

    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    constexpr size_type basic_hashed_string<CharType, Alloc, StaticCapacity>::capacity() const noexcept
    {
        return string.capacity();
    }

    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    constexpr CharType& basic_hashed_string<CharType, Alloc, StaticCapacity>::at(size_type i) noexcept
    {
        return string.at(i);
    }

    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    constexpr const CharType& basic_hashed_string<CharType, Alloc, StaticCapacity>::at(size_type i) const noexcept
    {
        return string.at(i);
    }

    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    constexpr typename basic_hashed_string<CharType, Alloc, StaticCapacity>::value_type& basic_hashed_string<CharType, Alloc,
        StaticCapacity>::operator[](size_type i) noexcept
    {
        return string[i];
    }

    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    constexpr const typename basic_hashed_string<CharType, Alloc, StaticCapacity>::value_type& basic_hashed_string<CharType, Alloc,
        StaticCapacity>::operator[](size_type i) const noexcept
    {
        return string[i];
    }

    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    constexpr typename basic_hashed_string<CharType, Alloc, StaticCapacity>::value_type* basic_hashed_string<CharType, Alloc,
        StaticCapacity>::data() noexcept
    {
        return string.data();
    }

    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    constexpr const typename basic_hashed_string<CharType, Alloc, StaticCapacity>::value_type* basic_hashed_string<CharType, Alloc,
        StaticCapacity>::data() const noexcept
    {
        return string.data();
    }

    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    constexpr typename basic_hashed_string<CharType, Alloc, StaticCapacity>::hashed_view_type basic_hashed_string<CharType, Alloc,
        StaticCapacity>::view() noexcept
    {
        return string.view();
    }

    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    constexpr basic_hashed_string_view<const CharType> basic_hashed_string<CharType, Alloc, StaticCapacity>::view() const noexcept
    {
        const_hashed_view_type result;
        result.string = string.view();
        result.hash = hash;
        return result;
    }

    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    constexpr basic_hashed_string<CharType, Alloc, StaticCapacity>::operator typename basic_dynamic_string<CharType, Alloc,
        StaticCapacity>::view_type() noexcept
    {
        return string.view();
    }

    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    constexpr basic_hashed_string<CharType, Alloc, StaticCapacity>::operator typename basic_dynamic_string<CharType, Alloc,
        StaticCapacity>::const_view_type() const noexcept
    {
        return string.view();
    }

    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    constexpr typename basic_hashed_string<CharType, Alloc, StaticCapacity>::value_type& basic_hashed_string<CharType, Alloc,
        StaticCapacity>::front() noexcept
    {
        return string.front();
    }

    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    constexpr const typename basic_hashed_string<CharType, Alloc, StaticCapacity>::value_type& basic_hashed_string<CharType, Alloc,
        StaticCapacity>::front() const noexcept
    {
        return string.front();
    }

    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    constexpr typename basic_hashed_string<CharType, Alloc, StaticCapacity>::value_type& basic_hashed_string<CharType, Alloc,
        StaticCapacity>::back() noexcept
    {
        return string.back();
    }

    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    constexpr const typename basic_hashed_string<CharType, Alloc, StaticCapacity>::value_type& basic_hashed_string<CharType, Alloc,
        StaticCapacity>::back() const noexcept
    {
        return string.back();
    }

    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    constexpr typename basic_hashed_string<CharType, Alloc, StaticCapacity>::iterator_type basic_hashed_string<CharType, Alloc,
        StaticCapacity>::begin() noexcept
    {
        return string.begin();
    }

    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    constexpr typename basic_hashed_string<CharType, Alloc, StaticCapacity>::const_iterator_type basic_hashed_string<CharType, Alloc,
        StaticCapacity>::begin() const noexcept
    {
        return string.begin();
    }

    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    constexpr typename basic_hashed_string<CharType, Alloc, StaticCapacity>::const_iterator_type basic_hashed_string<CharType, Alloc,
        StaticCapacity>::cbegin() const noexcept
    {
        return string.cbegin();
    }

    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    constexpr typename basic_hashed_string<CharType, Alloc, StaticCapacity>::iterator_type basic_hashed_string<CharType, Alloc,
        StaticCapacity>::end() noexcept
    {
        return string.end();
    }

    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    constexpr typename basic_hashed_string<CharType, Alloc, StaticCapacity>::const_iterator_type basic_hashed_string<CharType, Alloc,
        StaticCapacity>::end() const noexcept
    {
        return string.end();
    }

    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    constexpr typename basic_hashed_string<CharType, Alloc, StaticCapacity>::const_iterator_type basic_hashed_string<CharType, Alloc,
        StaticCapacity>::cend() const noexcept
    {
        return string.cend();
    }

    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    constexpr typename basic_hashed_string<CharType, Alloc, StaticCapacity>::reverse_iterator_type basic_hashed_string<CharType, Alloc,
        StaticCapacity>::rbegin() noexcept
    {
        return string.rbegin();
    }

    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    constexpr typename basic_hashed_string<CharType, Alloc, StaticCapacity>::const_reverse_iterator_type basic_hashed_string<CharType,
        Alloc, StaticCapacity>::rbegin() const noexcept
    {
        return string.rbegin();
    }

    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    constexpr typename basic_hashed_string<CharType, Alloc, StaticCapacity>::const_reverse_iterator_type basic_hashed_string<CharType,
        Alloc, StaticCapacity>::crbegin() const noexcept
    {
        return string.crbegin();
    }

    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    constexpr typename basic_hashed_string<CharType, Alloc, StaticCapacity>::reverse_iterator_type basic_hashed_string<CharType, Alloc,
        StaticCapacity>::rend() noexcept
    {
        return string.rend();
    }

    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    constexpr typename basic_hashed_string<CharType, Alloc, StaticCapacity>::const_reverse_iterator_type basic_hashed_string<CharType,
        Alloc, StaticCapacity>::rend() const noexcept
    {
        return string.rend();
    }

    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    constexpr typename basic_hashed_string<CharType, Alloc, StaticCapacity>::const_reverse_iterator_type basic_hashed_string<CharType,
        Alloc, StaticCapacity>::crend() const noexcept
    {
        return string.crend();
    }

    constexpr auto operator<=>(const hashed_string& lhs, const hashed_string& rhs) noexcept
    {
        return lhs.hash <=> rhs.hash;
    }

    constexpr auto operator==(const hashed_string& lhs, const hashed_string& rhs) noexcept
    {
        return lhs.hash == rhs.hash;
    }

    constexpr auto operator<=>(hashed_string_view lhs, const hashed_string& rhs) noexcept
    {
        return lhs.hash <=> rhs.hash;
    }

    constexpr auto operator==(const hashed_string& lhs, hashed_string_view rhs) noexcept
    {
        return lhs.hash == rhs.hash;
    }

    template <>
    struct hash<hashed_string>
    {
        [[rythe_always_inline]] constexpr id_type operator()(const hashed_string& val) const noexcept
        {
            return val.hash;
        }
    };

    template <>
    struct hash<hashed_string_view>
    {
        [[rythe_always_inline]] constexpr id_type operator()(const hashed_string_view& val) const noexcept
        {
            return val.hash;
        }
    };
}

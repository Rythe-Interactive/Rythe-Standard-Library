#pragma once

#include <span>

#include "../util/assert.hpp"
#include "../util/primitives.hpp"

namespace rsl {
    template<typename ValueType, size_type maxSize>
    struct buffered_list
    {
        using value_type = ValueType;
        using reference = value_type&;
        using const_reference = const value_type&;
        using iterator = value_type*;
        using const_iterator = const value_type*;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;
        using view_type = std::span<value_type, maxSize>;

    private:
        value_type m_buffer[maxSize + 1];
        size_type m_size;

        constexpr static size_type validate_size(size_type newSize) noexcept { return newSize < maxSize ? newSize : maxSize; }

    public:
        constexpr buffered_list() noexcept = default;

        constexpr buffered_list(const buffered_list& src) noexcept : m_size(validate_size(src.m_size))
        {
            for (size_type i = 0; i < m_size; i++)
                m_buffer[i] = src.m_buffer[i];
        }

        template<size_type N>
        constexpr buffered_list(const value_type(&str)[N]) noexcept : m_size(validate_size(N))
        {
            for (size_type i = 0; i < m_size; i++)
                m_buffer[i] = str[i];
        }

        constexpr buffered_list(view_type src) noexcept : m_size(validate_size(src.size()))
        {
            for (size_type i = 0; i < m_size; i++)
                m_buffer[i] = src[i];
        }

        constexpr buffered_list& operator=(const buffered_list& src) noexcept
        {
            m_size = validate_size(src.m_size);

            for (size_type i = 0; i < m_size; i++)
                m_buffer[i] = src.m_buffer[i];
            return *this;
        }

        template<size_type N>
        constexpr buffered_list& operator=(const value_type(&str)[N]) noexcept
        {
            m_size = validate_size(N);

            for (size_type i = 0; i < m_size; i++)
                m_buffer[i] = str[i];

            return *this;
        }

        constexpr buffered_list& operator=(view_type src) noexcept
        {
            m_size = validate_size(src.size());

            for (size_type i = 0; i < m_size; i++)
                m_buffer[i] = src[i];

            return *this;
        }

        constexpr reference at(size_type i)
        {
            rsl_assert_msg(i < m_size, "out of range");
            return m_buffer[i];
        }

        constexpr const_reference at(size_type i) const
        {
            rsl_assert_msg(i < m_size, "out of range");
            return m_buffer[i];
        }

        constexpr reference raw_at(size_type i)
        {
            return m_buffer[i];
        }

        constexpr const_reference raw_at(size_type i) const
        {
            return m_buffer[i];
        }

        constexpr reference operator[](size_type i) { return at(i); }
        constexpr const_reference operator[](size_type i) const { return at(i); }

        constexpr bool empty() const noexcept { return m_size == 0; }
        constexpr size_type size() const noexcept { return m_size; }
        constexpr size_type max_size() const noexcept { return maxSize; }

        constexpr void resize(size_type newSize) noexcept {
            size_type oldSize = m_size;
            m_size = validate_size(newSize);

            if (m_size > oldSize) {
                for (size_type i = oldSize; i < m_size; i++)
                    m_buffer[i] = ' ';
            }
        }

        constexpr view_type view() const noexcept { return view_type(m_buffer, m_size); }
        constexpr view_type raw_view() const noexcept { return view_type(m_buffer, maxSize); }

        constexpr operator const value_type* () const noexcept { return m_buffer; }
        constexpr operator view_type() const noexcept { return view(); }

        constexpr iterator begin() noexcept { return m_buffer; }
        constexpr const_iterator begin() const noexcept { return m_buffer; }
        constexpr const_iterator cbegin() const noexcept { return m_buffer; }

        constexpr iterator end() noexcept { return &m_buffer[m_size]; }
        constexpr const_iterator end() const noexcept { return &m_buffer[m_size]; }
        constexpr const_iterator cend() const noexcept { return &m_buffer[m_size]; }

        constexpr reverse_iterator rend() noexcept { return reverse_iterator(&m_buffer[m_size]); }
        constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(&m_buffer[m_size]); }
        constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(&m_buffer[m_size]); }

        constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(m_buffer); }
        constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(m_buffer); }
        constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(m_buffer); }
    };

    template<typename ValueType, size_type maxSize>
    buffered_list(const ValueType(&)[maxSize]) -> buffered_list<ValueType, maxSize>;

    template<typename ValueType>
    using b_list64 = buffered_list<ValueType, 64>;
    template<typename ValueType>
    using b_list128 = buffered_list<ValueType, 128>;
    template<typename ValueType>
    using b_list256 = buffered_list<ValueType, 256>;
    template<typename ValueType>
    using b_list512 = buffered_list<ValueType, 512>;
}

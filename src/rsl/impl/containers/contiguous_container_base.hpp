#pragma once
#include "memory_resource_base.hpp"

namespace rsl
{
    template<typename T, typename Alloc, typename Iter, typename ConstIter>
    struct contiguous_container_base : public memory_resource_base<T, Alloc>
    {
        using base_type = memory_resource_base<T, Alloc>;
        using value_type = typename base_type::value_type;
        using iterator_type = Iter;
        using const_iterator_type = ConstIter;
        using reverse_iterator_type = iterator_type;
        using const_reverse_iterator_type = const_iterator_type;

        [[nodiscard]] constexpr size_type size() const noexcept;
		[[nodiscard]] constexpr bool empty() const noexcept;

        [[nodiscard]] constexpr value_type& at(size_type i) noexcept;
        [[nodiscard]] constexpr const value_type& at(size_type i) const noexcept;

		[[nodiscard]] constexpr value_type& operator[](size_type i) noexcept;
		[[nodiscard]] constexpr const value_type& operator[](size_type i) const noexcept;

        [[nodiscard]] constexpr value_type& front() noexcept;
		[[nodiscard]] constexpr const value_type& front() const noexcept;

        [[nodiscard]] constexpr value_type& back() noexcept;
        [[nodiscard]] constexpr const value_type& back() const noexcept;

        [[nodiscard]] constexpr iterator_type begin() noexcept;
		[[nodiscard]] constexpr const_iterator_type begin() const noexcept;
		[[nodiscard]] constexpr const_iterator_type cbegin() const noexcept;

		[[nodiscard]] constexpr iterator_type end() noexcept;
		[[nodiscard]] constexpr const_iterator_type end() const noexcept;
		[[nodiscard]] constexpr const_iterator_type cend() const noexcept;

		[[nodiscard]] constexpr reverse_iterator_type rbegin() noexcept;
		[[nodiscard]] constexpr const_reverse_iterator_type rbegin() const noexcept;
		[[nodiscard]] constexpr const_reverse_iterator_type crbegin() const noexcept;

		[[nodiscard]] constexpr reverse_iterator_type rend() noexcept;
		[[nodiscard]] constexpr const_reverse_iterator_type rend() const noexcept;
		[[nodiscard]] constexpr const_reverse_iterator_type crend() const noexcept;

    protected:
        size_type m_size;
    };
} // namespace rsl

#include "contiguous_container_base.inl"

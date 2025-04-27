#pragma once

#include "../util/common.hpp"
#include "../util/primitives.hpp"
#include "iterators.hpp"

namespace rsl
{
	template <typename T, contiguous_iterator Iter = T*>
	struct view
	{
	public:
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;
		using iterator = Iter;
		using const_iterator = const Iter;
		using reverse_iterator = std::reverse_iterator<Iter>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		[[rythe_always_inline]] constexpr view() noexcept;
		template <typename It>
		[[rythe_always_inline]] constexpr view(It, size_type);
		template <typename It>
		[[rythe_always_inline]] constexpr view(It, It);
		template <size_type N>
		[[rythe_always_inline]] constexpr view(const value_type (&)[N]) noexcept;
		[[rythe_always_inline]] constexpr view(const view&) noexcept;
		[[rythe_always_inline]] constexpr view(value_type&&) noexcept;

	public:
		[[rythe_always_inline]] constexpr view& operator=(const view&) = default;

		[[rythe_always_inline]] constexpr bool operator==(const view&);
		[[rythe_always_inline]] constexpr bool operator!=(const view&);
		template <size_type N>
		[[rythe_always_inline]] constexpr bool operator==(const value_type (&)[N]);
		template <size_type N>
		[[rythe_always_inline]] constexpr bool operator!=(const value_type (&)[N]);

		[[rythe_always_inline]] constexpr iterator begin() noexcept;
		[[rythe_always_inline]] constexpr iterator begin() const noexcept;
		[[rythe_always_inline]] constexpr const_iterator cbegin() const noexcept;
		[[rythe_always_inline]] constexpr iterator end() noexcept;
		[[rythe_always_inline]] constexpr iterator end() const noexcept;
		[[rythe_always_inline]] constexpr const_iterator cend() const noexcept;

		[[rythe_always_inline]] constexpr iterator rbegin() noexcept;
		[[rythe_always_inline]] constexpr iterator rbegin() const noexcept;
		[[rythe_always_inline]] constexpr const_iterator crbegin() const noexcept;
		[[rythe_always_inline]] constexpr iterator rend() noexcept;
		[[rythe_always_inline]] constexpr iterator rend() const noexcept;
		[[rythe_always_inline]] constexpr const_iterator crend() const noexcept;

		[[rythe_always_inline]] constexpr reference front();
		[[rythe_always_inline]] constexpr reference front() const;
		[[rythe_always_inline]] constexpr reference back();
		[[rythe_always_inline]] constexpr reference back() const;
		[[rythe_always_inline]] constexpr reference at(size_type);
		[[rythe_always_inline]] constexpr reference at(size_type) const;
		[[rythe_always_inline]] constexpr reference operator[](size_type);
		[[rythe_always_inline]] constexpr reference operator[](size_type) const;
		[[rythe_always_inline]] constexpr pointer data() noexcept;
		[[rythe_always_inline]] constexpr pointer data() const noexcept;

		[[rythe_always_inline]] constexpr size_type size() const noexcept;
		[[rythe_always_inline]] constexpr size_type size_bytes() const noexcept;
		[[rythe_always_inline]] constexpr bool empty() const noexcept;

		[[rythe_always_inline]] constexpr view subview(size_type, size_type) const;

	private:
		pointer m_src = nullptr;
		size_type m_count = 0; //the length of our view relative to m_position
	};

	using string_view = rsl::view<char>;

	inline constexpr size_type find_first_of(string_view str, string_view other, size_type pos = 0) noexcept
	{
		size_type count = pos;
		for (string_view::iterator iter = str.begin() + pos; iter != str.end(); ++iter)
		{
			++count;
			for (string_view::iterator it = other.begin(); it != other.end(); ++it)
				if (*iter == *it)
					return count;
		}
		return -1;
	}

	inline constexpr size_type find_first_not_of(string_view str, string_view other, size_type pos = 0) noexcept
	{
		size_type count = pos;
		for (string_view::iterator iter = str.begin() + pos; iter != str.end(); ++iter)
		{
			++count;
			for (string_view::iterator it = other.begin(); it != other.end(); ++it)
				if (*iter != *it)
					return count;
		}
		return -1;
	}

	inline constexpr size_type find_last_of(string_view str, string_view other, size_type pos = 0) noexcept
	{
		size_type count = str.size();
		for (string_view::iterator iter = str.end(); iter != str.begin() + pos; --iter)
		{
			--count;
			for (string_view::iterator it = other.begin(); it != other.end(); ++it)
				if (*iter == *it)
					return count;
		}
		return -1;
	}

	inline constexpr size_type find_last_not_of(string_view str, string_view other, size_type pos = 0) noexcept
	{
		size_type count = str.size();
		for (string_view::iterator iter = str.end(); iter != str.begin() + pos; --iter)
		{
			--count;
			for (string_view::iterator it = other.begin(); it != other.end(); ++it)
				if (*iter != *it)
					return count;
		}
		return -1;
	}
}

#include "views.inl"

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
		using const_pointer = add_const_t<T>*;
		using reference = T&;
		using const_reference = add_const_t<T>&;
		using iterator_type = Iter;
		using const_iterator_type = add_const_t<T>*; // TODO(Rowan): const Iter makes a const * to a non const T, we should probably add a wrapper iterator like std::const_iterator<> to iterators.hpp. Also do we even want custom iterators for this type?
		using reverse_iterator_type = reverse_iterator<iterator_type>;
		using const_reverse_iterator_type = reverse_iterator<const_iterator_type>;

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

		[[rythe_always_inline]] constexpr iterator_type begin() noexcept;
		[[rythe_always_inline]] constexpr const_iterator_type begin() const noexcept;
		[[rythe_always_inline]] constexpr const_iterator_type cbegin() const noexcept;
		[[rythe_always_inline]] constexpr iterator_type end() noexcept;
		[[rythe_always_inline]] constexpr const_iterator_type end() const noexcept;
		[[rythe_always_inline]] constexpr const_iterator_type cend() const noexcept;

		[[rythe_always_inline]] constexpr reverse_iterator_type rbegin() noexcept;
		[[rythe_always_inline]] constexpr const_reverse_iterator_type rbegin() const noexcept;
		[[rythe_always_inline]] constexpr const_reverse_iterator_type crbegin() const noexcept;
		[[rythe_always_inline]] constexpr reverse_iterator_type rend() noexcept;
		[[rythe_always_inline]] constexpr const_reverse_iterator_type rend() const noexcept;
		[[rythe_always_inline]] constexpr const_reverse_iterator_type crend() const noexcept;

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

	// TODO(Rowan): Add more CTAD guides.
	template <typename It>
	view(It, size_type) -> view<remove_reference_t<iter_reference_t<It>>, It>;

	using string_view = rsl::view<const char>;

	// TODO(Rowan): The below functions don't use pos
	// TODO(Rowan): The below functions check for any occurrence of any of the items in other in str, not for the sequence of other in str. Is that intended?
	template <typename T, contiguous_iterator Iter>
	constexpr size_type find_first_of(view<T, Iter> str, view<T, Iter> other, [[maybe_unused]] size_type pos = 0) noexcept;

	template <typename T, contiguous_iterator Iter>
	constexpr size_type find_first_not_of(view<T, Iter> str, view<T, Iter> other, [[maybe_unused]] size_type pos = 0) noexcept;

	template <typename T, contiguous_iterator Iter>
	constexpr size_type find_last_of(view<T, Iter> str, view<T, Iter> other, [[maybe_unused]] size_type pos = 0) noexcept;

	template <typename T, contiguous_iterator Iter>
	constexpr size_type find_last_not_of(view<T, Iter> str, view<T, Iter> other, [[maybe_unused]] size_type pos = 0) noexcept;

	template <typename T, contiguous_iterator Iter>
	constexpr size_type find_first_of(view<T, Iter> str, add_const_t<T>& other, [[maybe_unused]] size_type pos = 0) noexcept;

	template <typename T, contiguous_iterator Iter>
	constexpr size_type find_first_not_of(view<T, Iter> str, add_const_t<T>& other, [[maybe_unused]] size_type pos = 0) noexcept;

	template <typename T, contiguous_iterator Iter>
	constexpr size_type find_last_of(view<T, Iter> str, add_const_t<T>& other, [[maybe_unused]] size_type pos = 0) noexcept;

	template <typename T, contiguous_iterator Iter>
	constexpr size_type find_last_not_of(view<T, Iter> str, add_const_t<T>& other, [[maybe_unused]] size_type pos = 0) noexcept;
}

#include "views.inl"

#pragma once

#include "../util/common.hpp"
#include "../util/primitives.hpp"

#include "iterators.hpp"

namespace rsl
{
	namespace internal
	{
		template <typename T, typename Iter>
		using select_const_iter = conditional_t<is_const_v<T>, Iter, conditional_t<same_as<Iter, T*>, const T*, const_iterator<Iter>>>;
	}

	template <typename T, contiguous_iterator Iter = T*, contiguous_iterator ConstIter = internal::select_const_iter<T, Iter>>
	struct array_view
	{
	public:
		using value_type = T;
		using pointer = T*;
		using const_pointer = add_const_t<T>*;
		using reference = T&;
		using const_reference = add_const_t<T>&;
		using iterator_type = Iter;
		using const_iterator_type = ConstIter;
		using reverse_iterator_type = reverse_iterator<iterator_type>;
		using const_reverse_iterator_type = reverse_iterator<const_iterator_type>;

		using const_view_type = conditional_t<is_const_v<T>, array_view, array_view<const value_type, const_iterator_type>>;

		[[rythe_always_inline]] constexpr array_view() noexcept = default;
		[[rythe_always_inline]] constexpr array_view(pointer ptr, size_type count) noexcept;
		template <contiguous_iterator It>
		[[rythe_always_inline]] constexpr array_view(It first, It last) noexcept(iter_noexcept_deref<It> && iter_noexcept_diff<It>) requires same_as<iter_pointer_t<It>, pointer>;
		template <size_type N>
		[[rythe_always_inline]] constexpr array_view(value_type (&arr)[N]) noexcept;
		[[rythe_always_inline]] constexpr array_view(const array_view& other) noexcept;
		[[rythe_always_inline]] constexpr array_view(value_type& src) noexcept;

		[[rythe_always_inline]] constexpr operator array_view<const value_type, const_iterator_type>() noexcept requires (!is_const_v<value_type>);

		[[rythe_always_inline]] constexpr static array_view from_string_length(T* str, T terminator = T{}) noexcept requires char_type<T>;

		[[rythe_always_inline]] constexpr array_view& operator=(const array_view&) = default;

		[[rythe_always_inline]] constexpr bool operator==(const array_view& rhs);
		[[rythe_always_inline]] constexpr bool operator!=(const array_view& rhs);
		template <size_type N>
		[[rythe_always_inline]] constexpr bool operator==(const value_type (&rhs)[N]);
		template <size_type N>
		[[rythe_always_inline]] constexpr bool operator!=(const value_type (&rhs)[N]);

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

		[[rythe_always_inline]] constexpr array_view subview(size_type, size_type) const;

	private:
		pointer m_src = nullptr;
		size_type m_count = 0; //the length of our view relative to m_position
	};

	template <typename It>
	array_view(It, size_type) -> array_view<iter_read_t<It>, It>;

	template <typename It>
	array_view(It, It) -> array_view<iter_read_t<It>, It>;

	template <typename T, size_type N>
	array_view(T (&)[N]) -> array_view<T>;

	template <typename T, contiguous_iterator Iter>
	array_view(array_view<T, Iter> other) -> array_view<const T, const_iterator<Iter>>;

	using string_view = rsl::array_view<const char>;

	[[nodiscard]] [[rythe_always_inline]] constexpr string_view operator""_sv(const char* str, const size_type size) noexcept
	{
		return string_view(str, size);
	}

	// TODO(Rowan): The below functions check for any occurrence of any of the items in other in str, not for the sequence of other in str. Is that intended?
	//				https://en.cppreference.com/w/cpp/string/basic_string_view/find_last_not_of.html see overload 1
	template <typename T, contiguous_iterator Iter>
	constexpr size_type find_first_of(array_view<const T, Iter> str, array_view<const T, Iter> key,
	                                  [[maybe_unused]] size_type pos = 0) noexcept;

	template <typename T, contiguous_iterator Iter>
	constexpr size_type find_first_not_of(array_view<const T, Iter> str, array_view<const T, Iter> key,
	                                      [[maybe_unused]] size_type pos = 0) noexcept;

	template <typename T, contiguous_iterator Iter>
	constexpr size_type find_last_of(array_view<const T, Iter> str, array_view<const T, Iter> key,
	                                 [[maybe_unused]] size_type pos = 0) noexcept;

	template <typename T, contiguous_iterator Iter>
	constexpr size_type find_last_not_of(array_view<const T, Iter> str, array_view<const T, Iter> key,
	                                     [[maybe_unused]] size_type pos = 0) noexcept;

	template <typename T, contiguous_iterator Iter>
	constexpr size_type find_first_of(array_view<const T, Iter> str, const T& key,
	                                  [[maybe_unused]] size_type pos = 0) noexcept;

	template <typename T, contiguous_iterator Iter, same_as<T> C>
	constexpr size_type find_first_not_of(array_view<const T, Iter> str, const C& key,
	                                      [[maybe_unused]] size_type pos = 0) noexcept;

	template <typename T, contiguous_iterator Iter, same_as<T> C>
	constexpr size_type find_last_of(array_view<const T, Iter> str, const C& key,
	                                 [[maybe_unused]] size_type pos = 0) noexcept;

	template <typename T, contiguous_iterator Iter, same_as<T> C>
	constexpr size_type find_last_not_of(array_view<const T, Iter> str, const C& key,
	                                     [[maybe_unused]] size_type pos = 0) noexcept;

	namespace internal
	{
		template<typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
		struct select_contiguous_view_impl
		{
			using contiguous_view = void;
		};


		template<typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
		struct select_contiguous_view_impl<T, Iter, ConstIter>
		{
			using contiguous_view = array_view<T, Iter, ConstIter>;
		};

		template<typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
		using select_contiguous_view = typename select_contiguous_view_impl<T, Iter, ConstIter>::contiguous_view;
	}

	template <typename T, input_or_output_iterator<T> Iter = T*, input_or_output_iterator<T> ConstIter = internal::select_const_iter<T, Iter>>
	struct iterator_view
	{
	public:
		using value_type = T;
		using pointer = T*;
		using const_pointer = add_const_t<T>*;
		using reference = T&;
		using const_reference = add_const_t<T>&;
		using iterator_type = Iter;
		using const_iterator_type = ConstIter;
		using reverse_iterator_type = reverse_iterator<iterator_type>;
		using const_reverse_iterator_type = reverse_iterator<const_iterator_type>;

		using const_view_type = conditional_t<is_const_v<T>, iterator_view, iterator_view<const value_type, const_iterator_type>>;
		using contiguous_view_type = internal::select_contiguous_view<T, iterator_type, const_iterator_type>;

		[[rythe_always_inline]] constexpr iterator_view() noexcept = default;
		[[rythe_always_inline]] constexpr iterator_view(iterator_type start, iterator_type end) noexcept;
		[[rythe_always_inline]] constexpr iterator_view(pointer ptr, size_type count) noexcept requires same_as<iterator_type, pointer>;
		template <size_type N>
		[[rythe_always_inline]] constexpr iterator_view(value_type (&arr)[N]) noexcept requires same_as<iterator_type, pointer>;
		[[rythe_always_inline]] constexpr iterator_view(const value_type& other) noexcept requires same_as<iterator_type, pointer>;

		[[rythe_always_inline]] constexpr operator iterator_view<const value_type, const_iterator_type>() noexcept requires (!is_const_v<value_type>);
		[[rythe_always_inline]] constexpr operator contiguous_view_type() noexcept requires (contiguous_iterator<iterator_type> && contiguous_iterator<const_iterator_type>);

		[[rythe_always_inline]] constexpr static iterator_view from_string_length(T* str, T terminator = T{}) noexcept requires (char_type<T> && same_as<iterator_type, pointer>);

		[[rythe_always_inline]] constexpr iterator_view& operator=(const iterator_view&) = default;

		[[rythe_always_inline]] constexpr bool operator==(const iterator_view& rhs);
		[[rythe_always_inline]] constexpr bool operator!=(const iterator_view& rhs);

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

		[[rythe_always_inline]] constexpr size_type size() const noexcept;
		[[rythe_always_inline]] constexpr bool empty() const noexcept;

	private:
		iterator_type m_start;
		iterator_type m_end;
	};
}

#include "views.inl"

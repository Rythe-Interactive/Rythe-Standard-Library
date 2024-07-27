#pragma once
#include "common.hpp"

namespace rsl
{
	template <typename Iter>
	using reverse_iterator = ::std::reverse_iterator<Iter>;

	template <typename Iter>
	[[nodiscard]] constexpr reverse_iterator<Iter> make_reverse_iterator(Iter iter)
		noexcept(::std::is_nothrow_move_constructible_v<Iter>)
	{
		return reverse_iterator<Iter>(::rsl::move(iter));
	}

	template <typename Container>
	[[nodiscard]] constexpr auto begin(Container& container)
		noexcept(noexcept(container.begin())) -> decltype(container.begin())
	{
		return container.begin();
	}

	template <typename Container>
	[[nodiscard]] constexpr auto begin(const Container& container)
		noexcept(noexcept(container.begin())) -> decltype(container.begin())
	{
		return container.begin();
	}

	template <typename Container>
	[[nodiscard]] constexpr auto end(Container& container)
		noexcept(noexcept(container.end())) -> decltype(container.end())
	{
		return container.end();
	}

	template <typename Container>
	[[nodiscard]] constexpr auto end(const Container& container)
		noexcept(noexcept(container.end())) -> decltype(container.end())
	{
		return container.end();
	}

	template <typename T, size_type Size>
	[[nodiscard]] constexpr T* begin(T (&arr)[Size]) noexcept
	{
		return arr;
	}

	template <typename T, size_type Size>
	[[nodiscard]] constexpr T* end(T (&arr)[Size]) noexcept
	{
		return arr + Size;
	}

	template <typename Container>
	[[nodiscard]] constexpr auto cbegin(const Container& container)
		noexcept(noexcept(::rsl::begin(container))) -> decltype(::rsl::begin(container))
	{
		return ::rsl::begin(container);
	}

	template <typename Container>
	[[nodiscard]] constexpr auto cend(const Container& container)
		noexcept(noexcept(::rsl::end(container))) -> decltype(::rsl::end(container))
	{
		return ::rsl::end(container);
	}

	template <typename Container>
	[[nodiscard]] constexpr auto rbegin(Container& container)
		noexcept(noexcept(container.rbegin())) -> decltype(container.rbegin())
	{
		return container.rbegin();
	}

	template <typename Container>
	[[nodiscard]] constexpr auto rbegin(const Container& container)
		noexcept(noexcept(container.rbegin())) -> decltype(container.rbegin())
	{
		return container.rbegin();
	}

	template <typename Container>
	[[nodiscard]] constexpr auto rend(Container& container)
		noexcept(noexcept(container.rend())) -> decltype(container.rend())
	{
		return container.rend();
	}

	template <typename Container>
	[[nodiscard]] constexpr auto rend(const Container& container)
		noexcept(noexcept(container.rend())) -> decltype(container.rend())
	{
		return container.rend();
	}

	template <typename T, size_type Size>
	[[nodiscard]] constexpr reverse_iterator<T*> rbegin(T (&arr)[Size]) noexcept
	{
		return reverse_iterator<T*>(arr + Size);
	}

	template <typename T, size_type Size>
	[[nodiscard]] constexpr reverse_iterator<T*> rend(T (&arr)[Size]) noexcept
	{
		return reverse_iterator<T*>(arr);
	}

	template <typename T>
	[[nodiscard]] constexpr reverse_iterator<const T*> rbegin(::std::initializer_list<T> initList) noexcept
	{
		return reverse_iterator<const T*>(initList.end());
	}

	template <typename T>
	[[nodiscard]] constexpr reverse_iterator<const T*> rend(::std::initializer_list<T> initList) noexcept
	{
		return reverse_iterator<const T*>(initList.begin());
	}

	template <typename Container>
	[[nodiscard]] constexpr auto crbegin(const Container& container)
		noexcept(noexcept(::rsl::rbegin(container))) -> decltype(::rsl::rbegin(container))
	{
		return ::rsl::rbegin(container);
	}

	template <typename Container>
	[[nodiscard]] constexpr auto crend(const Container& container)
		noexcept(noexcept(::rsl::rend(container))) -> decltype(::rsl::rend(container))
	{
		return ::rsl::rend(container);
	}

	template <typename Container>
	[[nodiscard]] constexpr auto size(const Container& container)
		noexcept(noexcept(container.size())) -> decltype(container.size())
	{
		return container.size();
	}

	template <typename T, size_type Size>
	[[nodiscard]] constexpr size_type size(const T (&)[Size]) noexcept
	{
		return Size;
	}

	template <typename Container>
	[[nodiscard]] constexpr auto empty(const Container& container)
		noexcept(noexcept(container.empty())) -> decltype(container.empty())
	{
		return container.empty();
	}

	template <typename T, size_type Size>
	[[nodiscard]] constexpr bool empty(const T (&)[Size]) noexcept
	{
		return false;
	}

	template <typename T>
	[[nodiscard]] constexpr bool empty(::std::initializer_list<T> initList) noexcept
	{
		return initList.size() == 0;
	}

	template <typename Container>
	[[nodiscard]] constexpr auto data(Container& container)
		noexcept(noexcept(container.data())) -> decltype(container.data())
	{
		return container.data();
	}

	template <typename Container>
	[[nodiscard]] constexpr auto data(const Container& container)
		noexcept(noexcept(container.data())) -> decltype(container.data())
	{
		return container.data();
	}

	template <typename T, size_type Size>
	[[nodiscard]] constexpr T* data(T (&arr)[Size]) noexcept
	{
		return arr;
	}

	template <typename T>
	[[nodiscard]] constexpr const T* data(::std::initializer_list<T> initList) noexcept
	{
		return initList.begin();
	}

} // namespace rsl

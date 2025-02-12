#pragma once
#include "pair.hpp"

namespace rsl
{
	template <typename T1, typename T2>
	template <constructable_type<> U1, constructable_type<> U2>
	inline constexpr pair<T1, T2>::pair() noexcept(is_nothrow_constructible_v<U1> && is_nothrow_constructible_v<U2>)
		: first(),
		  second()
	{
	}

	template <typename T1, typename T2>
	inline constexpr pair<T1, T2>::pair(first_type&& lhs, second_type&& rhs)
		noexcept(is_nothrow_move_constructible_v<first_type> && is_nothrow_move_constructible_v<second_type>)
		: first(std::move(lhs)),
		  second(std::move(rhs))
	{
	}

	template <typename T1, typename T2>
	template <typename U1, typename U2>
	inline constexpr pair<T1, T2>::pair(U1&& lhs, U2&& rhs)
		noexcept(is_nothrow_constructible_v<first_type, U1> && is_nothrow_constructible_v<second_type, U2>)
		: first(std::forward<U1>(lhs)),
		  second(std::forward<U2>(rhs))
	{
	}

	template <typename A, typename B>
	[[rythe_always_inline]] constexpr bool operator==(pair<A, B> const& x, pair<A, B> const& y) noexcept(
		noexcept(declval<A const&>() == declval<A const&>()) && noexcept(declval<B const&>() == declval<B const&>())
	)
	{
		return (x.first == y.first) && (x.second == y.second);
	}

	template <typename A, typename B>
	[[rythe_always_inline]] constexpr bool operator!=(pair<A, B> const& x, pair<A, B> const& y)
		noexcept(noexcept(x == y))
	{
		return !(x == y);
	}

	template <typename A, typename B>
	[[rythe_always_inline]] constexpr bool operator<(pair<A, B> const& x, pair<A, B> const& y) noexcept(
		noexcept(declval<A const&>() < declval<A const&>()) && noexcept(declval<B const&>() < declval<B const&>())
	)
	{
		return x.first < y.first || (!(y.first < x.first) && x.second < y.second);
	}

	template <typename A, typename B>
	[[rythe_always_inline]] constexpr bool operator>(pair<A, B> const& x, pair<A, B> const& y) noexcept(noexcept(y < x))
	{
		return y < x;
	}

	template <typename A, typename B>
	[[rythe_always_inline]] constexpr bool operator<=(pair<A, B> const& x, pair<A, B> const& y)
		noexcept(noexcept(x > y))
	{
		return !(x > y);
	}

	template <typename A, typename B>
	[[rythe_always_inline]] constexpr bool operator>=(pair<A, B> const& x, pair<A, B> const& y)
		noexcept(noexcept(x < y))
	{
		return !(x < y);
	}
} // namespace rsl

namespace std
{
	template <::std::size_t I, typename Tuple>
	struct tuple_element;

	template <::std::size_t I, typename T1, typename T2>
	struct tuple_element<I, rsl::pair<T1, T2>>
	{
		using type = rsl::element_at_t<I, T1, T2>;
	};

	template <::std::size_t I, typename T1, typename T2>
	const rsl::element_at_t<I, T1, T2>& get(const rsl::pair<T1, T2>& val)
	{
		if constexpr (I == 0)
		{
			return val.first;
		}
		else
		{
			return val.second;
		}
	}

	template <typename T, typename T1, typename T2>
	const T& get(const rsl::pair<T1, T2>& val)
	{
		return get<rsl::index_of_element_v<T, T1, T2>>(val);
	}

	template <::std::size_t I, typename T1, typename T2>
	rsl::element_at_t<I, T1, T2>& get(rsl::pair<T1, T2>& val)
	{
		return const_cast<rsl::element_at_t<I, T1, T2>&>(get<I>(rsl::as_const(val)));
	}

	template <typename T, typename T1, typename T2>
	T& get(rsl::pair<T1, T2>& val)
	{
		return const_cast<rsl::element_at_t<I, T1, T2>&>(get<T>(rsl::as_const(val)));
	}

	template <typename Tuple>
	struct tuple_size;

	template <typename T1, typename T2>
	struct tuple_size<rsl::pair<T1, T2>> : public std::tuple_size<std::pair<T1, T2>>
	{
	};
} // namespace std

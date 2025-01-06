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
	inline constexpr pair<T1, T2>::pair(const std::pair<first_type, second_type>& other)
		noexcept(is_nothrow_copy_constructible_v<first_type> && is_nothrow_copy_constructible_v<second_type>)
		: first(other.first),
		  second(other.second)
	{
	}

	template <typename T1, typename T2>
	inline constexpr pair<T1, T2>::pair(std::pair<first_type, second_type>&& other)
		noexcept(is_nothrow_move_constructible_v<first_type> && is_nothrow_move_constructible_v<second_type>)
		: first(std::move(other.first)),
		  second(std::move(other.second))
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
} // namespace rsl

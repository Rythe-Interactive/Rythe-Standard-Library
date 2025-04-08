#pragma once
#include <tuple>

#include "vector_base.hpp"

#include "vector_shared.inl"

namespace rsl::math
{
	template <size_type I, arithmetic_type Scalar, size_type Size, storage_mode Mode>
	constexpr Scalar& get(vector<Scalar, Size, Mode>& val) noexcept
	{
		return val[I];
	}
} // namespace rsl::math

template <::std::size_t I, rsl::arithmetic_type Scalar, rsl::size_type Size, rsl::math::storage_mode Mode>
struct ::std::tuple_element<I, ::rsl::math::vector<Scalar, Size, Mode>>
{
	using type = Scalar;
};

namespace std
{
	template <::std::size_t I, rsl::arithmetic_type Scalar, rsl::size_type Size, rsl::math::storage_mode Mode>
	constexpr Scalar& get(::rsl::math::vector<Scalar, Size, Mode>& val) noexcept
	{
		return ::rsl::math::get<I>(val);
	}
} // namespace std

template <rsl::arithmetic_type Scalar, rsl::size_type Size, rsl::math::storage_mode Mode>
struct ::std::tuple_size<::rsl::math::vector<Scalar, Size, Mode>> : public std::integral_constant<::std::size_t, Size>
{
};

template <rsl::arithmetic_type Scalar, rsl::size_type Size, rsl::math::storage_mode Mode>
struct ::std::tuple_size<const ::rsl::math::vector<Scalar, Size, Mode>> :
	public std::integral_constant<::std::size_t, Size>
{
};

template <rsl::arithmetic_type Scalar, rsl::size_type Size, rsl::math::storage_mode Mode>
struct ::std::tuple_size<volatile ::rsl::math::vector<Scalar, Size, Mode>> :
	public std::integral_constant<::std::size_t, Size>
{
};

template <rsl::arithmetic_type Scalar, rsl::size_type Size, rsl::math::storage_mode Mode>
struct ::std::tuple_size<const volatile ::rsl::math::vector<Scalar, Size, Mode>> :
	public std::integral_constant<::std::size_t, Size>
{
};

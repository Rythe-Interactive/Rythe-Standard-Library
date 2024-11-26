#include "vector_base.hpp"
#pragma once

#include "../util/type_util.hpp"

namespace rsl::math
{
	template <arithmetic_type Scalar, size_type Size, mode Mode>
	const vector<Scalar, Size, Mode> vector<Scalar, Size, Mode>::one(Scalar(1));

	template <arithmetic_type Scalar, size_type Size, mode Mode>
	const vector<Scalar, Size, Mode> vector<Scalar, Size, Mode>::zero(Scalar(0));

	template <arithmetic_type Scalar, size_type Size, mode Mode>
	constexpr vector<Scalar, Size, Mode>::vector() noexcept
	{
		for (size_type i = 0; i < size; i++)
		{
			data[i] = scalar(0);
		}
	}

	template <arithmetic_type Scalar, size_type Size, mode Mode>
	constexpr vector<Scalar, Size, Mode>::vector(scalar s) noexcept
	{
		for (size_type i = 0; i < size; i++)
		{
			data[i] = s;
		}
	}

	template <arithmetic_type Scalar, size_type Size, mode Mode>
	template <typename vec_type>
		requires not_same_as<Scalar, typename vec_type::scalar> || (vec_type::size != Size)
	constexpr vector<Scalar, Size, Mode>::vector(const vec_type& other) noexcept
	{
		if constexpr (size > vec_type::size)
		{
			for (size_type i = 0; i < vec_type::size; i++)
			{
				data[i] = static_cast<scalar>(other.data[i]);
			}

			for (size_type i = vec_type::size; i < size; i++)
			{
				data[i] = static_cast<scalar>(0);
			}
		}
		else
		{
			for (size_type i = 0; i < size; i++)
			{
				data[i] = static_cast<scalar>(other.data[i]);
			}
		}
	}

	template <arithmetic_type Scalar, size_type Size, mode Mode>
	constexpr Scalar& vector<Scalar, Size, Mode>::operator[](size_type i) noexcept
	{
		rsl_assert_out_of_range_msg((i >= 0) && (i < size), "vector subscript out of range");
		return data[i];
	}

	template <arithmetic_type Scalar, size_type Size, mode Mode>
	constexpr const Scalar& vector<Scalar, Size, Mode>::operator[](size_type i) const noexcept
	{
		rsl_assert_out_of_range_msg((i >= 0) && (i < size), "vector subscript out of range");
		return data[i];
	}

	template <size_type I, arithmetic_type Scalar, size_type Size, mode Mode>
	inline Scalar& get(vector<Scalar, Size, Mode>& val)
	{
		return val[I];
	}
} // namespace rsl::math

template <::std::size_t I, rsl::arithmetic_type Scalar, rsl::size_type Size, rsl::math::mode Mode>
struct ::std::tuple_element<I, ::rsl::math::vector<Scalar, Size, Mode>>
{
	using type = Scalar;
};

namespace std
{
	template <::std::size_t I, rsl::arithmetic_type Scalar, rsl::size_type Size, rsl::math::mode Mode>
	inline Scalar& get(::rsl::math::vector<Scalar, Size, Mode>& val)
	{
		return ::rsl::math::get<I>(val);
	}
} // namespace std

template <rsl::arithmetic_type Scalar, rsl::size_type Size, rsl::math::mode Mode>
struct ::std::tuple_size<::rsl::math::vector<Scalar, Size, Mode>> : public std::integral_constant<::std::size_t, Size>
{
};

template <rsl::arithmetic_type Scalar, rsl::size_type Size, rsl::math::mode Mode>
struct ::std::tuple_size<const ::rsl::math::vector<Scalar, Size, Mode>> :
	public std::integral_constant<::std::size_t, Size>
{
};

template <rsl::arithmetic_type Scalar, rsl::size_type Size, rsl::math::mode Mode>
struct ::std::tuple_size<volatile ::rsl::math::vector<Scalar, Size, Mode>> :
	public std::integral_constant<::std::size_t, Size>
{
};

template <rsl::arithmetic_type Scalar, rsl::size_type Size, rsl::math::mode Mode>
struct ::std::tuple_size<const volatile ::rsl::math::vector<Scalar, Size, Mode>> :
	public std::integral_constant<::std::size_t, Size>
{
};

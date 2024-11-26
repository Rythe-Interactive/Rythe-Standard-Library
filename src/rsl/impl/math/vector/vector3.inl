#include "vector3.hpp"
#pragma once

namespace rsl::math
{
	template <arithmetic_type Scalar, mode Mode>
		requires signed_type<Scalar>
	constexpr vector<Scalar, 3, Mode>::vector(const vector<scalar, 2>& v, scalar s) noexcept
		: vector(v.x, v.y, s)
	{
	}

	template <arithmetic_type Scalar, mode Mode>
		requires signed_type<Scalar>
	constexpr vector<Scalar, 3, Mode>::vector(scalar s, const vector<scalar, 2>& v) noexcept
		: vector(s, v.x, v.y)
	{
	}

	template <arithmetic_type Scalar, mode Mode>
		requires signed_type<Scalar>
	constexpr vector<Scalar, 3, Mode>::vector(scalar _x, scalar _y, scalar _z) noexcept
		: x(_x),
		  y(_y),
		  z(_z)
	{
	}

	template <arithmetic_type Scalar, mode Mode>
		requires signed_type<Scalar>
	const vector<Scalar, 3, Mode> vector<Scalar, 3, Mode>::up(Scalar(0), Scalar(1), Scalar(0));
	template <arithmetic_type Scalar, mode Mode>
		requires signed_type<Scalar>
	const vector<Scalar, 3, Mode> vector<Scalar, 3, Mode>::down(Scalar(0), Scalar(-1), Scalar(0));
	template <arithmetic_type Scalar, mode Mode>
		requires signed_type<Scalar>
	const vector<Scalar, 3, Mode> vector<Scalar, 3, Mode>::right(Scalar(1), Scalar(0), Scalar(0));
	template <arithmetic_type Scalar, mode Mode>
		requires signed_type<Scalar>
	const vector<Scalar, 3, Mode> vector<Scalar, 3, Mode>::left(Scalar(-1), Scalar(0), Scalar(0));
	template <arithmetic_type Scalar, mode Mode>
		requires signed_type<Scalar>
	const vector<Scalar, 3, Mode> vector<Scalar, 3, Mode>::forward(Scalar(0), Scalar(0), Scalar(1));
	template <arithmetic_type Scalar, mode Mode>
		requires signed_type<Scalar>
	const vector<Scalar, 3, Mode> vector<Scalar, 3, Mode>::backward(Scalar(0), Scalar(0), Scalar(-1));

	template <arithmetic_type Scalar, mode Mode>
		requires unsigned_type<Scalar>
	constexpr vector<Scalar, 3, Mode>::vector(const vector<scalar, 2>& v, scalar s) noexcept
		: vector(v.x, v.y, s)
	{
	}

	template <arithmetic_type Scalar, mode Mode>
		requires unsigned_type<Scalar>
	constexpr vector<Scalar, 3, Mode>::vector(scalar s, const vector<scalar, 2>& v) noexcept
		: vector(s, v.x, v.y)
	{
	}

	template <arithmetic_type Scalar, mode Mode>
		requires unsigned_type<Scalar>
	constexpr vector<Scalar, 3, Mode>::vector(scalar _x, scalar _y, scalar _z) noexcept
		: x(_x),
		  y(_y),
		  z(_z)
	{
	}

	template <arithmetic_type Scalar, mode Mode>
		requires unsigned_type<Scalar>
	const vector<Scalar, 3, Mode> vector<Scalar, 3, Mode>::up(Scalar(0), Scalar(1), Scalar(0));
	template <arithmetic_type Scalar, mode Mode>
		requires unsigned_type<Scalar>
	const vector<Scalar, 3, Mode> vector<Scalar, 3, Mode>::right(Scalar(1), Scalar(0), Scalar(0));
	template <arithmetic_type Scalar, mode Mode>
		requires unsigned_type<Scalar>
	const vector<Scalar, 3, Mode> vector<Scalar, 3, Mode>::forward(Scalar(0), Scalar(0), Scalar(1));
} // namespace rsl::math

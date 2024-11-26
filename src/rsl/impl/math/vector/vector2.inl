#include "vector2.hpp"
#pragma once

namespace rsl::math
{
	template <arithmetic_type Scalar, mode Mode>
		requires signed_type<Scalar>
	constexpr vector<Scalar, 2, Mode>::vector(scalar _x, scalar _y) noexcept
		: x(_x),
		  y(_y)
	{
	}

	template <arithmetic_type Scalar, mode Mode>
		requires signed_type<Scalar>
	const vector<Scalar, 2, Mode> vector<Scalar, 2, Mode>::up(Scalar(0), Scalar(1));
	template <arithmetic_type Scalar, mode Mode>
		requires signed_type<Scalar>
	const vector<Scalar, 2, Mode> vector<Scalar, 2, Mode>::down(Scalar(0), Scalar(-1));
	template <arithmetic_type Scalar, mode Mode>
		requires signed_type<Scalar>
	const vector<Scalar, 2, Mode> vector<Scalar, 2, Mode>::right(Scalar(1), Scalar(0));
	template <arithmetic_type Scalar, mode Mode>
		requires signed_type<Scalar>
	const vector<Scalar, 2, Mode> vector<Scalar, 2, Mode>::left(Scalar(-1), Scalar(0));

	template <arithmetic_type Scalar, mode Mode>
		requires unsigned_type<Scalar>
	constexpr vector<Scalar, 2, Mode>::vector(scalar _x, scalar _y) noexcept
		: x(_x),
		  y(_y)
	{
	}

	template <arithmetic_type Scalar, mode Mode>
		requires unsigned_type<Scalar>
	const vector<Scalar, 2, Mode> vector<Scalar, 2, Mode>::up(Scalar(0), Scalar(1));
	template <arithmetic_type Scalar, mode Mode>
		requires unsigned_type<Scalar>
	const vector<Scalar, 2, Mode> vector<Scalar, 2, Mode>::right(Scalar(1), Scalar(0));
} // namespace rsl::math

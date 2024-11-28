#pragma once
#include "vector3.hpp"

#define SizeSpecialization 3
#define ScalarSpecialization Scalar
#define VectorTemplate template <arithmetic_type Scalar, mode Mode>

#include "vector_shared.inl"

#define SizeSpecialization 3
#define ScalarSpecialization bool
#define VectorTemplate template <mode Mode>

#include "vector_shared.inl"

namespace rsl::math
{
	template <arithmetic_type Scalar, mode Mode>
	template <math::mode OtherMode>
	constexpr vector<Scalar, 3, Mode>::vector(const vector<scalar, 2, OtherMode>& v, scalar s) noexcept
		: vector(v.x, v.y, s)
	{
	}

	template <arithmetic_type Scalar, mode Mode>
	template <math::mode OtherMode>
	constexpr vector<Scalar, 3, Mode>::vector(scalar s, const vector<scalar, 2, OtherMode>& v) noexcept
		: vector(s, v.x, v.y)
	{
	}

	template <arithmetic_type Scalar, mode Mode>
	constexpr vector<Scalar, 3, Mode>::vector(scalar _x, scalar _y, scalar _z) noexcept
		: x(_x),
		  y(_y),
		  z(_z)
	{
	}

	template <mode Mode>
	template <math::mode OtherMode>
	constexpr vector<bool, 3, Mode>::vector(const vector<scalar, 2, OtherMode>& v, scalar s) noexcept
		: vector(v.x, v.y, s)
	{
	}

	template <mode Mode>
	template <math::mode OtherMode>
	constexpr vector<bool, 3, Mode>::vector(scalar s, const vector<scalar, 2, OtherMode>& v) noexcept
		: vector(s, v.x, v.y)
	{
	}

	template <mode Mode>
	constexpr vector<bool, 3, Mode>::vector(scalar _x, scalar _y, scalar _z) noexcept
		: x(_x),
		  y(_y),
		  z(_z)
	{
	}
} // namespace rsl::math

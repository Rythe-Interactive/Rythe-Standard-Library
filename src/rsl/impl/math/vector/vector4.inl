#pragma once
#include "vector4.hpp"

#define SizeSpecialization 4
#define ScalarSpecialization Scalar
#define VectorTemplate template <arithmetic_type Scalar, mode Mode>

#include "vector_shared.inl"

#define SizeSpecialization 4
#define ScalarSpecialization bool
#define VectorTemplate template <mode Mode>

#include "vector_shared.inl"

namespace rsl::math
{
	template <arithmetic_type Scalar, math::mode Mode>
	template <math::mode OtherMode>
	constexpr vector<Scalar, 4, Mode>::vector(const vector<scalar, 3, OtherMode>& v, scalar s) noexcept
		: vector(v.x, v.y, v.z, s)
	{
	}

	template <arithmetic_type Scalar, mode Mode>
	template <mode OtherMode>
	constexpr vector<Scalar, 4, Mode>::vector(scalar s, const vector<scalar, 3, OtherMode>& v) noexcept
		: vector(s, v.x, v.y, v.z)
	{
	}

	template <arithmetic_type Scalar, mode Mode>
	template <mode OtherMode>
	constexpr vector<Scalar, 4, Mode>::vector(scalar s0, const vector<scalar, 2, OtherMode>& v, scalar s1) noexcept
		: vector(s0, v.x, v.y, s1)
	{
	}

	template <arithmetic_type Scalar, mode Mode>
	template <mode OtherMode>
	constexpr vector<Scalar, 4, Mode>::vector(scalar s0, scalar s1, const vector<scalar, 2, OtherMode>& v) noexcept
		: vector(s0, s1, v.x, v.y)
	{
	}

	template <arithmetic_type Scalar, mode Mode>
	template <mode OtherMode>
	constexpr vector<Scalar, 4, Mode>::vector(const vector<scalar, 2, OtherMode>& v, scalar s0, scalar s1) noexcept
		: vector(v.x, v.y, s0, s1)
	{
	}

	template <arithmetic_type Scalar, mode Mode>
	template <mode Mode0, mode Mode1>
	constexpr vector<Scalar, 4, Mode>::vector(const vector<scalar, 2, Mode0>& v0, const vector<scalar, 2, Mode1>& v1)
		noexcept
		: vector(v0.x, v0.y, v1.x, v1.y)
	{
	}

	template <arithmetic_type Scalar, mode Mode>
	constexpr vector<Scalar, 4, Mode>::vector(scalar _x, scalar _y, scalar _z, scalar _w) noexcept
		: x(_x),
		  y(_y),
		  z(_z),
		  w(_w)
	{
	}

	template <math::mode Mode>
	template <math::mode OtherMode>
	constexpr vector<bool, 4, Mode>::vector(const vector<scalar, 3, OtherMode>& v, scalar s) noexcept
		: vector(v.x, v.y, v.z, s)
	{
	}

	template <math::mode Mode>
	template <mode OtherMode>
	constexpr vector<bool, 4, Mode>::vector(scalar s, const vector<scalar, 3, OtherMode>& v) noexcept
		: vector(s, v.x, v.y, v.z)
	{
	}

	template <math::mode Mode>
	template <mode OtherMode>
	constexpr vector<bool, 4, Mode>::vector(scalar s0, const vector<scalar, 2, OtherMode>& v, scalar s1) noexcept
		: vector(s0, v.x, v.y, s1)
	{
	}

	template <math::mode Mode>
	template <mode OtherMode>
	constexpr vector<bool, 4, Mode>::vector(scalar s0, scalar s1, const vector<scalar, 2, OtherMode>& v) noexcept
		: vector(s0, s1, v.x, v.y)
	{
	}

	template <math::mode Mode>
	template <mode OtherMode>
	constexpr vector<bool, 4, Mode>::vector(const vector<scalar, 2, OtherMode>& v, scalar s0, scalar s1) noexcept
		: vector(v.x, v.y, s0, s1)
	{
	}

	template <math::mode Mode>
	template <mode Mode0, mode Mode1>
	constexpr vector<bool, 4, Mode>::vector(const vector<scalar, 2, Mode0>& v0, const vector<scalar, 2, Mode1>& v1)
		noexcept
		: vector(v0.x, v0.y, v1.x, v1.y)
	{
	}

	template <math::mode Mode>
	constexpr vector<bool, 4, Mode>::vector(scalar _x, scalar _y, scalar _z, scalar _w) noexcept
		: x(_x),
		  y(_y),
		  z(_z),
		  w(_w)
	{
	}
} // namespace rsl::math

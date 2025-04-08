#pragma once
#include "vector3.hpp"

#define SizeSpecialization 3
#define ScalarSpecialization Scalar
#define VectorTemplate template <arithmetic_type Scalar, storage_mode Mode>

#include "vector_shared.inl"

#define SizeSpecialization 3
#define ScalarSpecialization bool
#define VectorTemplate template <storage_mode Mode>

#include "vector_shared.inl"

namespace rsl::math
{
	template <arithmetic_type Scalar, storage_mode Mode>
	template <math::storage_mode OtherMode>
	constexpr vector<Scalar, 3, Mode>::vector(const vector<scalar, 2, OtherMode>& v, scalar s) noexcept
		: vector(v.x, v.y, s)
	{
	}

	template <arithmetic_type Scalar, storage_mode Mode>
	template <math::storage_mode OtherMode>
	constexpr vector<Scalar, 3, Mode>::vector(scalar s, const vector<scalar, 2, OtherMode>& v) noexcept
		: vector(s, v.x, v.y)
	{
	}

	template <arithmetic_type Scalar, storage_mode Mode>
	constexpr vector<Scalar, 3, Mode>::vector(scalar _x, scalar _y, scalar _z) noexcept
		: data{
			  {_x, _y, _z}
    }
	{
	}

	template <storage_mode Mode>
	template <math::storage_mode OtherMode>
	constexpr vector<bool, 3, Mode>::vector(const vector<scalar, 2, OtherMode>& v, scalar s) noexcept
		: vector(v.x, v.y, s)
	{
	}

	template <storage_mode Mode>
	template <math::storage_mode OtherMode>
	constexpr vector<bool, 3, Mode>::vector(scalar s, const vector<scalar, 2, OtherMode>& v) noexcept
		: vector(s, v.x, v.y)
	{
	}

	template <storage_mode Mode>
	constexpr vector<bool, 3, Mode>::vector(scalar _x, scalar _y, scalar _z) noexcept
		: data{
			  {_x, _y, _z}
    }
	{
	}
} // namespace rsl::math

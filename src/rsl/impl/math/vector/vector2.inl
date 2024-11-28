#pragma once
#include "vector2.hpp"

#define SizeSpecialization 2
#define ScalarSpecialization Scalar
#define VectorTemplate template <arithmetic_type Scalar, mode Mode>

#include "vector_shared.inl"

#define SizeSpecialization 2
#define ScalarSpecialization bool
#define VectorTemplate template <mode Mode>

#include "vector_shared.inl"

namespace rsl::math
{
	template <arithmetic_type Scalar, mode Mode>
	constexpr vector<Scalar, 2, Mode>::vector(scalar _x, scalar _y) noexcept
		: x(_x),
		  y(_y)
	{
	}

	template <mode Mode>
	constexpr vector<bool, 2, Mode>::vector(scalar _x, scalar _y) noexcept
		: x(_x),
		  y(_y)
	{
	}
} // namespace rsl::math

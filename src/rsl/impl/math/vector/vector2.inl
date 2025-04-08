#pragma once
#include "vector2.hpp"

#define SizeSpecialization 2
#define ScalarSpecialization Scalar
#define VectorTemplate template <arithmetic_type Scalar, storage_mode Mode>

#include "vector_shared.inl"

#define SizeSpecialization 2
#define ScalarSpecialization bool
#define VectorTemplate template <storage_mode Mode>

#include "vector_shared.inl"

namespace rsl::math
{
	template <arithmetic_type Scalar, storage_mode Mode>
	constexpr vector<Scalar, 2, Mode>::vector(scalar _x, scalar _y) noexcept
		: data{
			  {_x, _y}
	}
	{
	}

	template <storage_mode Mode>
	constexpr vector<bool, 2, Mode>::vector(scalar _x, scalar _y) noexcept
		: data{
			  {_x, _y}
	}
	{
	}
} // namespace rsl::math

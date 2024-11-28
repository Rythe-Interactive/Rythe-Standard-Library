#pragma once
#include "matrix2.hpp"

#define RowCountSpecialization 2
#define ColCountSpecialization 2
#define ScalarSpecialization Scalar
#define MatrixTemplate template <arithmetic_type Scalar, mode Mode>

#include "matrix_shared.inl"

namespace rsl::math
{
	template <arithmetic_type Scalar, mode Mode>
	constexpr matrix<Scalar, 2, 2, Mode>::matrix(scalar s00, scalar s01, scalar s10, scalar s11) noexcept
		: row0(s00, s01),
		  row1(s10, s11)
	{
	}

	template <arithmetic_type Scalar, mode Mode>
	constexpr matrix<Scalar, 2, 2, Mode>::matrix(row_type r0, row_type r1) noexcept
		: row0(r0),
		  row1(r1)
	{
	}
} // namespace rsl::math

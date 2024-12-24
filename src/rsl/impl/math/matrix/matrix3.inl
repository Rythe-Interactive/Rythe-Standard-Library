#pragma once
#include "matrix3.hpp"

#define RowCountSpecialization 3
#define ColCountSpecialization 3
#define ScalarSpecialization Scalar
#define MatrixTemplate template <arithmetic_type Scalar, mode Mode>

#include "matrix_shared.inl"

namespace rsl::math
{
	template <arithmetic_type Scalar, mode Mode>
	constexpr matrix<Scalar, 3, 3, Mode>::matrix(
		scalar s00, scalar s01, scalar s02, scalar s10, scalar s11, scalar s12, scalar s20, scalar s21, scalar s22
	) noexcept
		: row0(s00, s01, s02),
		  row1(s10, s11, s12),
		  row2(s20, s21, s22)
	{
	}

	template <arithmetic_type Scalar, mode Mode>
	constexpr matrix<Scalar, 3, 3, Mode>::matrix(row_type r0, row_type r1, row_type r2) noexcept
		: row0(r0),
		  row1(r1),
		  row2(r2)
	{
	}
} // namespace rsl::math

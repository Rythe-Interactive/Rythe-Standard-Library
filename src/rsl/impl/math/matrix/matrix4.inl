#pragma once
#include "matrix4.hpp"

#define RowCountSpecialization 4
#define ColCountSpecialization 4
#define ScalarSpecialization Scalar
#define MatrixTemplate template <arithmetic_type Scalar, storage_mode Mode>

#include "matrix_shared.inl"

namespace rsl::math
{
	template <arithmetic_type Scalar, storage_mode Mode>
	constexpr matrix<Scalar, 4, 4, Mode>::matrix(
		scalar s00, scalar s01, scalar s02, scalar s03, scalar s10, scalar s11, scalar s12, scalar s13, scalar s20,
		scalar s21, scalar s22, scalar s23, scalar s30, scalar s31, scalar s32, scalar s33
	) noexcept
		: row0(s00, s01, s02, s03),
		  row1(s10, s11, s12, s13),
		  row2(s20, s21, s22, s23),
		  row3(s30, s31, s32, s33)
	{
	}

	template <arithmetic_type Scalar, storage_mode Mode>
	constexpr matrix<Scalar, 4, 4, Mode>::matrix(row_type r0, row_type r1, row_type r2, row_type r3) noexcept
		: row0(r0),
		  row1(r1),
		  row2(r2),
		  row3(r3)
	{
	}
} // namespace rsl::math

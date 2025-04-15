#pragma once
#include "matrix3.hpp"

#define ROW_COUNT_SPECIALIZATION 3
#define COL_COUNT_SPECIALIZATION 3
#define SCALAR_SPECIALIZATION Scalar
#define MATRIX_TEMPLATE template <arithmetic_type Scalar, storage_mode Mode>

#include "matrix_shared.inl"

#define SCALAR_SPECIALIZATION bool
#define ROW_COUNT_SPECIALIZATION 3
#define COL_COUNT_SPECIALIZATION 3
#define MATRIX_TEMPLATE template <storage_mode MODE_SPECIALIZATION>

#include "matrix_shared.inl"

namespace rsl::math
{
	template <arithmetic_type Scalar, storage_mode Mode>
	constexpr matrix<Scalar, 3, 3, Mode>::matrix(
		scalar s00, scalar s01, scalar s02, scalar s10, scalar s11, scalar s12, scalar s20, scalar s21, scalar s22
	) noexcept
		: col0(s00, s10, s20),
		  col1(s01, s11, s21),
		  col2(s02, s12, s22)
	{
	}

	template <arithmetic_type Scalar, storage_mode Mode>
	constexpr matrix<Scalar, 3, 3, Mode>::matrix(col_type c0, col_type c1, col_type c2) noexcept
		: col0(c0),
		  col1(c1),
		  col2(c2)
	{
	}

	template <storage_mode Mode>
	constexpr matrix<bool, 3, 3, Mode>::matrix(
		scalar s00, scalar s01, scalar s02, scalar s10, scalar s11, scalar s12, scalar s20, scalar s21, scalar s22
	) noexcept
		: col0(s00, s10, s20),
		  col1(s01, s11, s21),
		  col2(s02, s12, s22)
	{
	}

	template <storage_mode Mode>
	constexpr matrix<bool, 3, 3, Mode>::matrix(col_type c0, col_type c1, col_type c2) noexcept
		: col0(c0),
		  col1(c1),
		  col2(c2)
	{
	}
} // namespace rsl::math

#pragma once
#include "../vector/vector.hpp"

#include "column/column.hpp"
#include "matrix_base.hpp"

#include "matrix2.hpp"
#include "matrix3.hpp"
#include "matrix4.hpp"

#include "column/column.inl"
#include "matrix_base.inl"

#include "matrix2.inl"
#include "matrix3.inl"
#include "matrix4.inl"

#include "adjoint.hpp"
#include "decompose.hpp"
#include "determinant.hpp"
#include "util.hpp"

#include "adjoint.inl"
#include "decompose.inl"
#include "determinant.inl"
#include "util.inl"


#ifdef RYTHE_PCH
namespace rsl::math
{
	template struct matrix<float32, 2, 2>;
	template struct matrix<float64, 2, 2>;

	template struct matrix<float32, 3, 3>;
	template struct matrix<float64, 3, 3>;

	template struct matrix<float32, 4, 4>;
	template struct matrix<float64, 4, 4>;
} // namespace rsl::math
#endif

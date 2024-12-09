#pragma once
#include <immintrin.h>

#include "../../util/assert.hpp"
#include "../constants.hpp"
#include "../util/type_util.hpp"

namespace rsl::math
{
	template<typename Scalar, size_type RowCount, size_type ColCount>
	struct matrix;
}




#include "matrix_quat_conv.inl"

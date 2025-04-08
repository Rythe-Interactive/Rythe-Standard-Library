#pragma once
#include "../../util/concepts.hpp"
#include "../util/storage.hpp"
#include "../vector/predefined.hpp"

namespace rsl::math
{
	template <arithmetic_type Scalar, size_type RowCount, size_type ColCount, size_type ColIdx, storage_mode Mode>
	struct column;

	template <size_type RowCount, size_type ColCount, size_type ColIdx, storage_mode Mode>
	struct column<bool, RowCount, ColCount, ColIdx, Mode>;

	template <arithmetic_type Scalar, size_type ColCount, size_type ColIdx, storage_mode Mode>
	struct column<Scalar, 1, ColCount, ColIdx, Mode>;

	template <size_type ColCount, size_type ColIdx, storage_mode Mode>
	struct column<bool, 1, ColCount, ColIdx, Mode>;

	struct uniform_matrix_signal;
	struct identity_matrix_signal;

	template <arithmetic_type Scalar, storage_mode Mode>
	struct quaternion;

	template <arithmetic_type Scalar, size_type RowCount, size_type ColCount, storage_mode Mode>
	struct matrix;

	template <arithmetic_type Scalar, storage_mode Mode>
	struct matrix<Scalar, 2, 2, Mode>;

	template <arithmetic_type Scalar, storage_mode Mode>
	struct matrix<Scalar, 3, 3, Mode>;

	template <arithmetic_type Scalar, storage_mode Mode>
	struct matrix<Scalar, 4, 4, Mode>;
} // namespace rsl::math

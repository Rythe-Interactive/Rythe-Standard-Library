#pragma once
#include "matrix_base.hpp"

#include "matrix_shared.inl"

#define SCALAR_SPECIALIZATION bool
#define MATRIX_TEMPLATE                                                                                                \
	template <size_type ROW_COUNT_SPECIALIZATION, size_type COL_COUNT_SPECIALIZATION, storage_mode MODE_SPECIALIZATION>
#include "matrix_shared.inl"

#define COL_COUNT_SPECIALIZATION 1
#define ROW_COUNT_SPECIALIZATION 1
#define MATRIX_TEMPLATE template <arithmetic_type SCALAR_SPECIALIZATION, storage_mode MODE_SPECIALIZATION>
#include "matrix_shared.inl"

#pragma once
#include "matrix2.hpp"

#define ROW_COUNT_SPECIALIZATION 1
#define COL_COUNT_SPECIALIZATION 1
#define SCALAR_SPECIALIZATION Scalar
#define MATRIX_TEMPLATE template <arithmetic_type Scalar, storage_mode Mode>

#include "matrix_shared.inl"

#define ROW_COUNT_SPECIALIZATION 1
#define COL_COUNT_SPECIALIZATION 1
#define SCALAR_SPECIALIZATION bool
#define MATRIX_TEMPLATE template <storage_mode Mode>

#include "matrix_shared.inl"

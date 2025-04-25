#pragma once
#include "vector1.hpp"

#define SIZE_SPECIALIZATION 1
#define SCALAR_SPECIALIZATION Scalar
#define VECTOR_TEMPLATE template <arithmetic_type Scalar, storage_mode Mode>

#include "vector_shared.inl"

#define SIZE_SPECIALIZATION 1
#define SCALAR_SPECIALIZATION bool
#define VECTOR_TEMPLATE template <storage_mode Mode>

#include "vector_shared.inl"

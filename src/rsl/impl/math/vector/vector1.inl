#pragma once
#include "vector1.hpp"

#define SizeSpecialization 1
#define ScalarSpecialization Scalar
#define VectorTemplate template <arithmetic_type Scalar, storage_mode Mode>

#include "vector_shared.inl"

#define SizeSpecialization 1
#define ScalarSpecialization bool
#define VectorTemplate template <storage_mode Mode>

#include "vector_shared.inl"

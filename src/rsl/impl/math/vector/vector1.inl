#pragma once
#include "vector1.hpp"

#define SizeSpecialization 1
#define ScalarSpecialization Scalar
#define VectorTemplate template <arithmetic_type Scalar, mode Mode>

#include "vector_shared.inl"

#define SizeSpecialization 1
#define ScalarSpecialization bool
#define VectorTemplate template <mode Mode>

#include "vector_shared.inl"

#pragma once
#include <cmath>

#include "../../util/primitives.hpp"
#include "../matrix/determinant.hpp"
#include "../quaternion/quaternion.hpp"
#include "../vector/vector.hpp"
#include "dot.hpp"

namespace rsl::math
{
	template <vector_type VecType>
	[[nodiscard]] [[rythe_always_inline]] auto length(const VecType& v) noexcept;

	template <vector_type VecType>
	[[nodiscard]] [[rythe_always_inline]] constexpr auto length2(const VecType& v) noexcept;

	template <quat_type QuatType>
	[[nodiscard]] [[rythe_always_inline]] auto length(const QuatType& q) noexcept;

	template <quat_type QuatType>
	[[nodiscard]] [[rythe_always_inline]] constexpr auto length2(const QuatType& q) noexcept;

	template <matrix_type MatType>
	[[nodiscard]] [[rythe_always_inline]] auto length(const MatType& m) noexcept;

	template <matrix_type MatType>
	[[nodiscard]] [[rythe_always_inline]] constexpr auto length2(const MatType& m) noexcept;
} // namespace rsl::math

#include "length.inl"

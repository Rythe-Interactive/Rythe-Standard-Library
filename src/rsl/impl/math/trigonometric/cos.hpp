#pragma once
#include "../../util/primitives.hpp"
#include "../basic/constraint.hpp"
#include "../matrix/matrix.hpp"
#include "../quaternion/quaternion_base.hpp"
#include "../util/type_util.hpp"
#include "../vector/vector.hpp"

#include <cmath>

namespace rsl::math
{
	// cos
	template <typename T>
	[[nodiscard]] rythe_always_inline auto cos(T&& v) noexcept;

	// acos
	template <typename T>
	[[nodiscard]] rythe_always_inline auto acos(T&& v) noexcept;
} // namespace rsl::math

#include "cos.inl"

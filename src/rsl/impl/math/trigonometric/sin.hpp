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
	// sin
	template <typename T>
	[[nodiscard]] rythe_always_inline auto sin(T&& v) noexcept;

	// asin
	template <typename T>
	[[nodiscard]] rythe_always_inline auto asin(T&& v) noexcept;
} // namespace rsl::math

#include "sin.inl"

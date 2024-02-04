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
	// tan
	template <typename T>
	[[nodiscard]] rythe_always_inline auto tan(T&& v) noexcept;

	// atan
	template <typename T>
	[[nodiscard]] rythe_always_inline auto atan(T&& v) noexcept;
} // namespace rsl::math

#include "tan.inl"

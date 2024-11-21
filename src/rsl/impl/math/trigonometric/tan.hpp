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
	template <typename T>
	[[nodiscard]] [[rythe_always_inline]] auto tan(T&& v) noexcept;

	template <typename T>
	[[nodiscard]] [[rythe_always_inline]] auto atan(T&& v) noexcept;

	template <typename X, typename Y>
		requires same_as<remove_cvr_t<X>, remove_cvr_t<Y>>
	[[nodiscard]] [[rythe_always_inline]] auto atan2(X&& x, Y&& y) noexcept;
} // namespace rsl::math

#include "tan.inl"

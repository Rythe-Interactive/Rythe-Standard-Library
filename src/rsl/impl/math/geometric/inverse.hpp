#pragma once
#include "../../util/primitives.hpp"
#include "../arithmetic/division.hpp"
#include "../arithmetic/negate.hpp"
#include "../matrix/matrix.hpp"
#include "../quaternion/quaternion.hpp"
#include "../vector/vector.hpp"

namespace rsl::math
{
	template <typename vec_type, ::std::enable_if_t<is_vector_v<vec_type>, bool> = true>
	[[nodiscard]] rythe_always_inline auto inverse(const vec_type& v) noexcept;

	template <typename quat_type, ::std::enable_if_t<is_quat_v<quat_type>, bool> = true>
	[[nodiscard]] rythe_always_inline auto inverse(const quat_type& q) noexcept;

	template <typename mat_type, ::std::enable_if_t<is_matrix_v<mat_type>, bool> = true>
	[[nodiscard]] rythe_always_inline auto inverse(const mat_type& m) noexcept;
} // namespace rsl::math

#include "inverse.inl"

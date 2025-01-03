#pragma once
#include <cmath>

#include "../../util/primitives.hpp"
#include "../matrix/determinant.hpp"
#include "../quaternion/quaternion.hpp"
#include "../vector/vector.hpp"
#include "dot.hpp"

namespace rsl::math
{
	template <typename vec_type, ::std::enable_if_t<is_vector_v<vec_type>, bool> = true>
	[[nodiscard]] [[rythe_always_inline]] auto length(const vec_type& v) noexcept;

	template <typename vec_type, ::std::enable_if_t<is_vector_v<vec_type>, bool> = true>
	[[nodiscard]] [[rythe_always_inline]] constexpr auto length2(const vec_type& v) noexcept;

	template <typename quat_type, ::std::enable_if_t<is_quat_v<quat_type>, bool> = true>
	[[nodiscard]] [[rythe_always_inline]] auto length(const quat_type& q) noexcept;

	template <typename quat_type, ::std::enable_if_t<is_quat_v<quat_type>, bool> = true>
	[[nodiscard]] [[rythe_always_inline]] constexpr auto length2(const quat_type& q) noexcept;

	template <typename mat_type, ::std::enable_if_t<is_matrix_v<mat_type>, bool> = true>
	[[nodiscard]] [[rythe_always_inline]] auto length(const mat_type& m) noexcept;

	template <typename mat_type, ::std::enable_if_t<is_matrix_v<mat_type>, bool> = true>
	[[nodiscard]] [[rythe_always_inline]] constexpr auto length2(const mat_type& m) noexcept;
} // namespace rsl::math

#include "length.inl"

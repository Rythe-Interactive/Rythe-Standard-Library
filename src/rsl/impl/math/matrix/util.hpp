#pragma once

#include "../geometric/geometric.hpp"

namespace rsl::math
{
	template <typename Scalar>
	[[nodiscard]] matrix<Scalar, 4, 4> perspective(Scalar fovY, Scalar aspect, Scalar nearZ, Scalar farZ) noexcept;

	template <typename Scalar>
	[[nodiscard]] matrix<Scalar, 4, 4>
	orthographic(Scalar left, Scalar right, Scalar bottom, Scalar top, Scalar nearZ, Scalar farZ) noexcept;

	template <typename Scalar>
	[[nodiscard]] matrix<Scalar, 4, 4>
	look_at(vector<Scalar, 3> eye, vector<Scalar, 3> center, vector<Scalar, 3> up) noexcept;

	template <typename Scalar>
	[[nodiscard]] [[rythe_always_inline]] constexpr matrix<Scalar, 4, 4> transpose(const matrix<Scalar, 4, 4>& mat
	) noexcept;

	template <typename Scalar>
	[[nodiscard]] [[rythe_always_inline]] constexpr matrix<Scalar, 4, 4>
	translate(const matrix<Scalar, 4, 4>& mat, const vector<Scalar, 3>& pos) noexcept;

	/*template<typename Scalar>
	[[nodiscard]] matrix<Scalar, 4, 4> rotate(matrix<Scalar, 4, 4> matrix, quaternion<Scalar> orientation) noexcept;*/

	template <typename Scalar>
	[[nodiscard]] [[rythe_always_inline]] matrix<Scalar, 4, 4>
	rotate(const matrix<Scalar, 4, 4>& mat, radians<Scalar> angle, const vector<Scalar, 3>& axis) noexcept;

	template <typename Scalar>
	[[nodiscard]] [[rythe_always_inline]] constexpr matrix<Scalar, 4, 4>
	scale(const matrix<Scalar, 4, 4>& mat, const vector<Scalar, 3>& scale) noexcept;
} // namespace rsl::math

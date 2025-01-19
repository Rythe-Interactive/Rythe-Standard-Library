#pragma once
#include "predefined.hpp"

#include "../../util/primitives.hpp"

#include "../geometric/geometric.hpp"
#include "../trigonometric/trigonometric.hpp"

namespace rsl::math
{
	template <typename Scalar>
	[[nodiscard]] matrix<Scalar, 4, 4> perspective(Scalar rads, Scalar fov, Scalar nearZ, Scalar farZ) noexcept;

	template <typename Scalar>
	[[nodiscard]] matrix<Scalar, 4, 4>
	orthographic(Scalar left, Scalar right, Scalar bottom, Scalar top, Scalar nearZ, Scalar farZ) noexcept;

	template <typename Scalar>
	[[nodiscard]] matrix<Scalar, 4, 4>
	look_at(vector<Scalar, 3> eye, vector<Scalar, 3> center, vector<Scalar, 3> up) noexcept;

	template <typename Scalar>
	[[nodiscard]] matrix<Scalar, 3, 3> transpose(matrix<Scalar, 3, 3> mat) noexcept;

	template <typename Scalar>
	[[nodiscard]] matrix<Scalar, 4, 4> transpose(matrix<Scalar, 4, 4> mat) noexcept;

	template <typename Scalar>
	[[nodiscard]] matrix<Scalar, 4, 4> translate(matrix<Scalar, 4, 4> mat, vector<Scalar, 3> pos) noexcept;
	/*template<typename Scalar>
	[[nodiscard]] matrix<Scalar, 4, 4> rotate(matrix<Scalar, 4, 4> matrix, quaternion<Scalar> orientation) noexcept;*/
	template <typename Scalar>
	[[nodiscard]] matrix<Scalar, 4, 4> rotate(matrix<Scalar, 4, 4> mat, Scalar rad, vector<Scalar, 3> axis) noexcept;
	template <typename Scalar>
	[[nodiscard]] matrix<Scalar, 4, 4> scale(matrix<Scalar, 4, 4> mat, vector<Scalar, 3> scale) noexcept;
} // namespace rsl::math

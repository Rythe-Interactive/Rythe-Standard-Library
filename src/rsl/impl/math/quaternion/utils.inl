#pragma once
#include <cmath>

#include "../constants.hpp"
#include "../trigonometry.hpp"
#include "../exponential/sqrt.hpp"
#include "../trigonometric/trigonometric.hpp"
#include "../arithmetic/multiplication.hpp"
#include "../geometric/cross.hpp"
#include "../geometric/normalize.hpp"
#include "quaternion_base.hpp"

namespace rsl::math
{
	template <typename Scalar>
    vector<Scalar, 3> quaternion<Scalar>::right() const noexcept
	{
		return vec_type::right * *this;
	}

	template <typename Scalar>
    vector<Scalar, 3> quaternion<Scalar>::forward() const noexcept
	{
		return vec_type::forward * *this;
	}

	template <typename Scalar>
    vector<Scalar, 3> quaternion<Scalar>::up() const noexcept
	{
		return vec_type::up * *this;
	}

	template <typename Scalar>
    vector<Scalar, 3> quaternion<Scalar>::euler_angles() const noexcept
	{// TODO(Glyn): Optimize
		vector<Scalar, 3> angles;

		Scalar sinr_cosp = static_cast<Scalar>(2) * (w * i + j * k);
		Scalar cosr_cosp = static_cast<Scalar>(1) - static_cast<Scalar>(2) * (i * i + j * j);
		angles.x = atan2(sinr_cosp, cosr_cosp);

		Scalar sinp = sqrt(static_cast<Scalar>(1) + static_cast<Scalar>(2) * (w * j - i * k));
		Scalar cosp = sqrt(static_cast<Scalar>(1) - static_cast<Scalar>(2) * (w * j - i * k));
		angles.y = static_cast<Scalar>(2) * atan2(sinp, cosp) - pi() / static_cast<Scalar>(2);

		Scalar siny_cosp = static_cast<Scalar>(2) * (w * k + i * j);
		Scalar cosy_cosp = static_cast<Scalar>(1) - static_cast<Scalar>(2) * (j * j + k * k);
		angles.z = atan2(siny_cosp, cosy_cosp);

		return angles;
	}

	template <typename Scalar>
	quaternion<Scalar> quaternion<Scalar>::angle_axis(radians<scalar> angle, const vec_type& vec) noexcept
	{
		auto halfAngle = angle.value * static_cast<scalar>(0.5);
		return quaternion<scalar>(cos(halfAngle), sin(halfAngle) * vec);
	}

	template <typename Scalar>
	[[nodiscard]] [[rythe_always_inline]] quaternion<Scalar> quaternion<Scalar>::look_at(const vec_type& _pos, const vec_type& _center, const vec_type& _up) noexcept
	{// TODO(Glyn): Optimize
		vec_type const forward(normalize(_center - _pos));
		vec_type const right(normalize(cross(_up, forward)));
		vec_type const up(cross(forward, right));

		const scalar qwijk[] = {
			right.x - up.y - forward.z,
			up.y - right.x - forward.z,
			forward.z - right.x - up.y,
			right.x + up.y + forward.z
		};

		size_type idx = 0;
		scalar qMax = qwijk[0];
		for (size_type i = 1; i < 4; i++)
			if (qwijk[i] > qMax)
			{
				qMax = qwijk[i];
				idx = i;
			}

		qMax = sqrt(qMax + static_cast<scalar>(1)) * static_cast<scalar>(0.5);
		scalar mult = static_cast<scalar>(0.25) / qMax;

		scalar qPerms[] = {
			qMax,
			(forward.y - up.z) * mult,
			(right.z - forward.x) * mult,
			(up.x - right.y) * mult
		};

		size_type invIdx = 3 - idx;
		return quaternion(
			qPerms[idx],
			qPerms[(invIdx + 2) % 4],
			qPerms[(idx + 2) % 4],
			qPerms[invIdx]
		);
	}

	template <typename Scalar>
	[[nodiscard]] [[rythe_always_inline]] quaternion<Scalar> quaternion<Scalar>::conjugate(const quaternion<Scalar>& _quat) noexcept
	{
		return quaternion<Scalar>(_quat.w, -_quat.i, -_quat.j, -_quat.k);
	}

	template <typename Scalar>
	inline quaternion<Scalar> quaternion<Scalar>::from_euler(const vec_type& euler) noexcept
	{// TODO(Glyn): Optimize
		vector<Scalar, 3> c = cos(euler * static_cast<Scalar>(0.5));
		vector<Scalar, 3> s = sin(euler * static_cast<Scalar>(0.5));

		quaternion<Scalar> result;
		result.w = c.x * c.y * c.z + s.x * s.y * s.z;
		result.i = s.x * c.y * c.z - c.x * s.y * s.z;
		result.j = c.x * s.y * c.z + s.x * c.y * s.z;
		result.k = c.x * c.y * s.z - s.x * s.y * c.z;
		return result;
	}
} // namespace rsl::math

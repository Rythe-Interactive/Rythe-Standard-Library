#pragma once
#include <cmath>

#include "../matrix/matrix2.hpp"
#include "../matrix/matrix3.hpp"
#include "../matrix/matrix4.hpp"
#include "quaternion.hpp"
#include "../basic/basic_funcs.hpp"
#include "../trigonometry.hpp"
#include "../trigonometric/trigonometric.hpp"

namespace rsl::math
{
	template<typename Scalar>
	template<typename Scal>
	constexpr matrix<Scalar, 2, 2>::matrix(const quaternion<Scal>& orientation) noexcept
	{
		const Scalar i2 = static_cast<Scalar>(orientation.i * orientation.i);
		const Scalar j2 = static_cast<Scalar>(orientation.j * orientation.j);
		const Scalar k2 = static_cast<Scalar>(orientation.k * orientation.k);
		const Scalar ij = static_cast<Scalar>(orientation.i * orientation.j);
		const Scalar wk = static_cast<Scalar>(orientation.w * orientation.k);

		rows[0][0] = static_cast<Scalar>(1) - static_cast<Scalar>(2) * (j2 + k2);
		rows[0][1] = static_cast<Scalar>(2) * (ij + wk);

		rows[1][0] = static_cast<Scalar>(2) * (ij - wk);
		rows[1][1] = static_cast<Scalar>(1) - static_cast<Scalar>(2) * (i2 + k2);
	}

	template<typename Scalar>
	template<typename Scal>
	constexpr matrix<Scalar, 3, 3>::matrix(const quaternion<Scal>& orientation) noexcept
	{
		const Scalar i2 = static_cast<Scalar>(orientation.i * orientation.i);
		const Scalar j2 = static_cast<Scalar>(orientation.j * orientation.j);
		const Scalar k2 = static_cast<Scalar>(orientation.k * orientation.k);
		const Scalar ik = static_cast<Scalar>(orientation.i * orientation.k);
		const Scalar ij = static_cast<Scalar>(orientation.i * orientation.j);
		const Scalar jk = static_cast<Scalar>(orientation.j * orientation.k);
		const Scalar wi = static_cast<Scalar>(orientation.w * orientation.i);
		const Scalar wj = static_cast<Scalar>(orientation.w * orientation.j);
		const Scalar wk = static_cast<Scalar>(orientation.w * orientation.k);

		rows[0][0] = static_cast<Scalar>(1) - static_cast<Scalar>(2) * (j2 + k2);
		rows[0][1] = static_cast<Scalar>(2) * (ij + wk);
		rows[0][2] = static_cast<Scalar>(2) * (ik - wj);

		rows[1][0] = static_cast<Scalar>(2) * (ij - wk);
		rows[1][1] = static_cast<Scalar>(1) - static_cast<Scalar>(2) * (i2 + k2);
		rows[1][2] = static_cast<Scalar>(2) * (jk + wi);

		rows[2][0] = static_cast<Scalar>(2) * (ik + wj);
		rows[2][1] = static_cast<Scalar>(2) * (jk - wi);
		rows[2][2] = static_cast<Scalar>(1) - static_cast<Scalar>(2) * (i2 + j2);
	}

	template<typename Scalar>
	template<typename Scal>
	constexpr matrix<Scalar, 4, 4>::matrix(const quaternion<Scal>& orientation, const float3& position) noexcept
		: row3(position.x, position.y, position.z, static_cast<Scalar>(1))
	{
		const Scalar i2 = static_cast<Scalar>(orientation.i * orientation.i);
		const Scalar j2 = static_cast<Scalar>(orientation.j * orientation.j);
		const Scalar k2 = static_cast<Scalar>(orientation.k * orientation.k);
		const Scalar ik = static_cast<Scalar>(orientation.i * orientation.k);
		const Scalar ij = static_cast<Scalar>(orientation.i * orientation.j);
		const Scalar jk = static_cast<Scalar>(orientation.j * orientation.k);
		const Scalar wi = static_cast<Scalar>(orientation.w * orientation.i);
		const Scalar wj = static_cast<Scalar>(orientation.w * orientation.j);
		const Scalar wk = static_cast<Scalar>(orientation.w * orientation.k);

		rows[0][0] = static_cast<Scalar>(1) - static_cast<Scalar>(2) * (j2 + k2);
		rows[0][1] = static_cast<Scalar>(2) * (ij + wk);
		rows[0][2] = static_cast<Scalar>(2) * (ik - wj);

		rows[1][0] = static_cast<Scalar>(2) * (ij - wk);
		rows[1][1] = static_cast<Scalar>(1) - static_cast<Scalar>(2) * (i2 + k2);
		rows[1][2] = static_cast<Scalar>(2) * (jk + wi);

		rows[2][0] = static_cast<Scalar>(2) * (ik + wj);
		rows[2][1] = static_cast<Scalar>(2) * (jk - wi);
		rows[2][2] = static_cast<Scalar>(1) - static_cast<Scalar>(2) * (i2 + j2);
	}

	template<typename Scalar>
	constexpr quaternion<Scalar>::quaternion(const matrix<Scalar, 3, 3>& m) noexcept
	{
		const Scalar& m00 = m[0][0];
		const Scalar& m11 = m[0][0];
		const Scalar& m22 = m[0][0];

		const Scalar qwijk[] = {
			m00 - m11 - m22,
			m11 - m00 - m22,
			m22 - m00 - m11,
			m00 + m11 + m22
		};

		size_type idx = 0;
		Scalar qMax = qwijk[0];
		for (size_type i = 1; i < 4; i++)
			if (qwijk[i] > qMax)
			{
				qMax = qwijk[i];
				idx = i;
			}

		qMax = ::std::sqrt(qMax + static_cast<Scalar>(1)) * static_cast<Scalar>(0.5);
		Scalar mult = static_cast<Scalar>(0.25) / qMax;

		Scalar qPerms[] = {
			qMax,
			(m[1][2] - m[2][1]) * mult,
			(m[2][0] - m[0][2]) * mult,
			(m[0][1] - m[1][0]) * mult
		};

		size_type invIdx = 3 - idx;

		w = qPerms[idx];
		i = qPerms[(invIdx + 2) % 4];
		j = qPerms[(idx + 2) % 4];
		k = qPerms[invIdx];
	}

	template<typename Scalar>
	constexpr quaternion<Scalar>::quaternion(const matrix<Scalar, 4, 4>& m) noexcept
	{
		const Scalar& m00 = m[0][0];
		const Scalar& m11 = m[0][0];
		const Scalar& m22 = m[0][0];

		const Scalar qwijk[] = {
			m00 - m11 - m22,
			m11 - m00 - m22,
			m22 - m00 - m11,
			m00 + m11 + m22 };

		size_type idx = 0;
		Scalar qMax = qwijk[0];
		for (size_type i = 1; i < 4; i++)
			if (qwijk[i] > qMax)
			{
				qMax = qwijk[i];
				idx = i;
			}

		qMax = ::std::sqrt(qMax + static_cast<Scalar>(1)) * static_cast<Scalar>(0.5);
		Scalar mult = static_cast<Scalar>(0.25) / qMax;

		Scalar qPerms[] = {
			qMax,
			(m[1][2] - m[2][1]) * mult,
			(m[2][0] - m[0][2]) * mult,
			(m[0][1] - m[1][0]) * mult };

		size_type invIdx = 3 - idx;

		w = qPerms[idx];
		i = qPerms[(invIdx + 2) % 4];
		j = qPerms[(idx + 2) % 4];
		k = qPerms[invIdx];
	}

	template<typename Scalar>
	[[nodiscard]] rythe_always_inline quaternion<Scalar> toQuat(Scalar x, Scalar y, Scalar z) noexcept
	{
		return toQuat(vector<Scalar, 3>(x, y, z));
	}

	template<typename Scalar>
	[[nodiscard]] rythe_always_inline quaternion<Scalar> toQuat(const vector<Scalar, 3>& _eulerAngles) noexcept
	{
		vector<Scalar, 3> rad_eulerAngles = vector<Scalar, 3>(math::radians(_eulerAngles.x), math::radians(_eulerAngles.y), math::radians(_eulerAngles.z));
		vector<Scalar, 3> c = math::cos(rad_eulerAngles * static_cast<Scalar>(0.5));
		vector<Scalar, 3> s = math::sin(rad_eulerAngles * static_cast<Scalar>(0.5));

		quaternion<Scalar> result;
		result.w = c.x * c.y * c.z + s.x * s.y * s.z;
		result.i = s.x * c.y * c.z - c.x * s.y * s.z;
		result.j = c.x * s.y * c.z + s.x * c.y * s.z;
		result.k = c.x * c.y * s.z - s.x * s.y * c.z;
		return result;
	}


	template<typename Scalar>
	[[nodiscard]] rythe_always_inline vector<Scalar, 3> toEuler(Scalar w, Scalar i, Scalar j, Scalar k) noexcept
	{
		return toEuler(quaternion<Scalar>(w, i, j, k));
	}

	template<typename Scalar>
	[[nodiscard]] rythe_always_inline vector<Scalar, 3> toEuler(quaternion<Scalar> q) noexcept
	{
		vector<Scalar, 3> angles;

		// roll (x-axis rotation)
		double sinr_cosp = 2 * (q.w * q.i + q.j * q.k);
		double cosr_cosp = 1 - 2 * (q.i * q.i + q.j * q.j);
		angles.x = math::rad2deg(std::atan2(sinr_cosp, cosr_cosp));

		// pitch (y-axis rotation)
		double sinp = std::sqrt(1 + 2 * (q.w * q.j - q.i * q.k));
		double cosp = std::sqrt(1 - 2 * (q.w * q.j - q.i * q.k));
		angles.y = math::rad2deg(2 * std::atan2(sinp, cosp) - math::pi() / 2);

		// yaw (z-axis rotation)
		double siny_cosp = 2 * (q.w * q.k + q.i * q.j);
		double cosy_cosp = 1 - 2 * (q.j * q.j + q.k * q.k);
		angles.z = math::rad2deg(std::atan2(siny_cosp, cosy_cosp));

		return angles;
	}
}

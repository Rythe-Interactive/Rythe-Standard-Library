#pragma once
#include <cmath>

#include "../arithmetic/multiplication.hpp"
#include "../constants.hpp"
#include "../exponential/sqrt.hpp"
#include "../geometric/cross.hpp"
#include "../geometric/normalize.hpp"
#include "../trigonometric/trigonometric.hpp"
#include "quaternion_base.hpp"

namespace rsl::math
{
	template <arithmetic_type Scalar, mode Mode>
	constexpr quaternion<Scalar, Mode>::quaternion() noexcept
		: w(static_cast<scalar>(1)),
		  i(static_cast<scalar>(0)),
		  j(static_cast<scalar>(0)),
		  k(static_cast<scalar>(0))
	{
	}

	template <arithmetic_type Scalar, mode Mode>
	constexpr quaternion<Scalar, Mode>::quaternion(scalar _w, scalar _i, scalar _j, scalar _k) noexcept
		: w(_w),
		  i(_i),
		  j(_j),
		  k(_k)
	{
	}

	template <arithmetic_type Scalar, mode Mode>
	template <vector_type VecType>
		requires(VecType::size == 3)
	constexpr quaternion<Scalar, Mode>::quaternion(typename VecType::scalar s, const VecType& v) noexcept
		: w(static_cast<scalar>(s)),
		  i(static_cast<scalar>(v.x)),
		  j(static_cast<scalar>(v.y)),
		  k(static_cast<scalar>(v.z))
	{
	}

	template <arithmetic_type Scalar, mode Mode>
	template <vector_type VecType>
		requires(VecType::size == 4)
	constexpr quaternion<Scalar, Mode>::quaternion(const VecType& other) noexcept
		: w(static_cast<scalar>(other.w)),
		  i(static_cast<scalar>(other.x)),
		  j(static_cast<scalar>(other.y)),
		  k(static_cast<scalar>(other.z))
	{
	}

	template <arithmetic_type Scalar, mode Mode>
	constexpr Scalar& quaternion<Scalar, Mode>::operator[](size_type index) noexcept
	{
		rsl_assert_out_of_range_msg((index >= 0) && (index < size), "quaternion subscript out of range");
		return data[index];
	}

	template <arithmetic_type Scalar, mode Mode>
	constexpr const Scalar& quaternion<Scalar, Mode>::operator[](size_type index) const noexcept
	{
		rsl_assert_out_of_range_msg((index >= 0) && (index < size), "quaternion subscript out of range");
		return data[index];
	}

	template <arithmetic_type Scalar, mode Mode>
	constexpr vector<Scalar, 3, Mode> quaternion<Scalar, Mode>::right() const noexcept
	{
		return vector<Scalar,3, Mode>(1,0,0) * *this;
	}

	template <arithmetic_type Scalar, mode Mode>
	constexpr vector<Scalar, 3, Mode> quaternion<Scalar, Mode>::forward() const noexcept
	{
		return vector<Scalar, 3, Mode>(0, 0, 1)* *this;
	}

	template <arithmetic_type Scalar, mode Mode>
	constexpr vector<Scalar, 3, Mode> quaternion<Scalar, Mode>::up() const noexcept
	{
		return vector<Scalar, 3, Mode>(0, 1, 0)* *this;
	}

	template <arithmetic_type Scalar, mode Mode>
	constexpr vector<Scalar, 3, Mode> quaternion<Scalar, Mode>::euler_angles() const noexcept
	{ // TODO(Glyn): Optimize
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

	template <arithmetic_type Scalar, mode Mode>
	constexpr quaternion<Scalar, Mode>
	quaternion<Scalar, Mode>::angle_axis(radians<scalar> angle, const vec_type& vec) noexcept
	{
		auto halfAngle = angle.value * static_cast<scalar>(0.5);
		return quaternion<scalar>(cos(halfAngle), sin(halfAngle) * vec);
	}

	template <arithmetic_type Scalar, mode Mode>
	constexpr quaternion<Scalar, Mode>
	quaternion<Scalar, Mode>::look_at(const vec_type& _pos, const vec_type& _center, const vec_type& _up) noexcept
	{ // TODO(Glyn): Optimize
		vec_type const forward(normalize(_center - _pos));
		vec_type const right(normalize(cross(_up, forward)));
		vec_type const up(cross(forward, right));

		const scalar qwijk[] = {
			right.x - up.y - forward.z, up.y - right.x - forward.z, forward.z - right.x - up.y,
			right.x + up.y + forward.z
		};

		size_type idx = 0;
		scalar qMax = qwijk[0];
		for (size_type i = 1; i < 4; i++)
		{
			if (qwijk[i] > qMax)
			{
				qMax = qwijk[i];
				idx = i;
			}
		}

		qMax = sqrt(qMax + static_cast<scalar>(1)) * static_cast<scalar>(0.5);
		scalar mult = static_cast<scalar>(0.25) / qMax;

		scalar qPerms[] = {qMax, (forward.y - up.z) * mult, (right.z - forward.x) * mult, (up.x - right.y) * mult};

		size_type invIdx = 3 - idx;
		return quaternion(qPerms[idx], qPerms[(invIdx + 2) % 4], qPerms[(idx + 2) % 4], qPerms[invIdx]);
	}

	template <arithmetic_type Scalar, mode Mode>
	constexpr quaternion<Scalar, Mode> quaternion<Scalar, Mode>::conjugate(const quaternion<Scalar, Mode>& _quat
	) noexcept
	{
		return quaternion<Scalar>(_quat.w, -_quat.i, -_quat.j, -_quat.k);
	}

	template <arithmetic_type Scalar, mode Mode>
	constexpr quaternion<Scalar, Mode> quaternion<Scalar, Mode>::from_euler(const vec_type& euler) noexcept
	{ // TODO(Glyn): Optimize
		vector<Scalar, 3> c = cos(euler * static_cast<Scalar>(0.5));
		vector<Scalar, 3> s = sin(euler * static_cast<Scalar>(0.5));

		quaternion<Scalar> result;
		result.w = c.x * c.y * c.z + s.x * s.y * s.z;
		result.i = s.x * c.y * c.z - c.x * s.y * s.z;
		result.j = c.x * s.y * c.z + s.x * c.y * s.z;
		result.k = c.x * c.y * s.z - s.x * s.y * c.z;
		return result;
	}

#define sin45 0.70710678118654752440084436210485L
#define cos45 0.70710678118654752440084436210485L

	template <arithmetic_type Scalar, mode Mode>
	const quaternion<Scalar, Mode> quaternion<Scalar, Mode>::identity(
		static_cast<Scalar>(1), static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(0)
	);
	template <arithmetic_type Scalar, mode Mode>
	const quaternion<Scalar, Mode> quaternion<Scalar, Mode>::rotate_x_90(
		static_cast<Scalar>(cos45), static_cast<Scalar>(sin45), static_cast<Scalar>(0), static_cast<Scalar>(0)
	);
	template <arithmetic_type Scalar, mode Mode>
	const quaternion<Scalar, Mode> quaternion<Scalar, Mode>::rotate_y_90(
		static_cast<Scalar>(cos45), static_cast<Scalar>(0), static_cast<Scalar>(sin45), static_cast<Scalar>(0)
	);
	template <arithmetic_type Scalar, mode Mode>
	const quaternion<Scalar, Mode> quaternion<Scalar, Mode>::rotate_z_90(
		static_cast<Scalar>(cos45), static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(sin45)
	);
	template <arithmetic_type Scalar, mode Mode>
	const quaternion<Scalar, Mode> quaternion<Scalar, Mode>::rotate_x_180(
		static_cast<Scalar>(0), static_cast<Scalar>(1), static_cast<Scalar>(0), static_cast<Scalar>(0)
	);
	template <arithmetic_type Scalar, mode Mode>
	const quaternion<Scalar, Mode> quaternion<Scalar, Mode>::rotate_y_180(
		static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(1), static_cast<Scalar>(0)
	);
	template <arithmetic_type Scalar, mode Mode>
	const quaternion<Scalar, Mode> quaternion<Scalar, Mode>::rotate_z_180(
		static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(1)
	);

#undef sin45
#undef cos45
} // namespace rsl::math

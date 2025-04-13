#pragma once

#include "../arithmetic/multiplication.hpp"
#include "../constants.hpp"
#include "../exponential/sqrt.hpp"
#include "../geometric/cross.hpp"
#include "../geometric/normalize.hpp"
#include "../trigonometric/trigonometric.hpp"
#include "quaternion_base.hpp"

namespace rsl::math
{
	template <arithmetic_type Scalar, storage_mode Mode>
	constexpr quaternion<Scalar, Mode>::quaternion() noexcept
		: w(static_cast<scalar>(1)),
		  i(static_cast<scalar>(0)),
		  j(static_cast<scalar>(0)),
		  k(static_cast<scalar>(0))
	{
	}

	template <arithmetic_type Scalar, storage_mode Mode>
	constexpr quaternion<Scalar, Mode>::quaternion(scalar _w, scalar _i, scalar _j, scalar _k) noexcept
		: w(_w),
		  i(_i),
		  j(_j),
		  k(_k)
	{
	}

	template <arithmetic_type Scalar, storage_mode Mode>
	template <vector_type VecType>
		requires(VecType::size == 3)
	constexpr quaternion<Scalar, Mode>::quaternion(typename VecType::scalar s, const VecType& v) noexcept
		: w(static_cast<scalar>(s)),
		  i(static_cast<scalar>(v.x)),
		  j(static_cast<scalar>(v.y)),
		  k(static_cast<scalar>(v.z))
	{
	}

	template <arithmetic_type Scalar, storage_mode Mode>
	template <vector_type VecType>
		requires(VecType::size == 4)
	constexpr quaternion<Scalar, Mode>::quaternion(const VecType& other) noexcept
		: w(static_cast<scalar>(other.w)),
		  i(static_cast<scalar>(other.x)),
		  j(static_cast<scalar>(other.y)),
		  k(static_cast<scalar>(other.z))
	{
	}

	template <arithmetic_type Scalar, storage_mode Mode>
	constexpr Scalar& quaternion<Scalar, Mode>::operator[](size_type index) noexcept
	{
		rsl_assert_out_of_range_msg(index < size, "quaternion subscript out of range");
		return data[index];
	}

	template <arithmetic_type Scalar, storage_mode Mode>
	constexpr const Scalar& quaternion<Scalar, Mode>::operator[](size_type index) const noexcept
	{
		rsl_assert_out_of_range_msg(index < size, "quaternion subscript out of range");
		return data[index];
	}

	template <arithmetic_type Scalar, storage_mode Mode>
	constexpr vector<Scalar, 3, Mode> quaternion<Scalar, Mode>::right() const noexcept
	{
		return vec_type::right * *this;
	}

	template <arithmetic_type Scalar, storage_mode Mode>
	constexpr vector<Scalar, 3, Mode> quaternion<Scalar, Mode>::forward() const noexcept
	{
		return vec_type::forward * *this;
	}

	template <arithmetic_type Scalar, storage_mode Mode>
	constexpr vector<Scalar, 3, Mode> quaternion<Scalar, Mode>::up() const noexcept
	{
		return vec_type::up * *this;
	}

	template <arithmetic_type Scalar, storage_mode Mode>
	constexpr vector<Scalar, 3, Mode> quaternion<Scalar, Mode>::euler_angles() const noexcept
	{ // TODO(Glyn): Optimize
		vector<Scalar, 3> angles;

		Scalar sinRCosP = static_cast<Scalar>(2) * (w * i + j * k);
		Scalar cosRCosP = static_cast<Scalar>(1) - static_cast<Scalar>(2) * (i * i + j * j);
		angles.x = atan2(sinRCosP, cosRCosP);

		Scalar sinP = sqrt(static_cast<Scalar>(1) + static_cast<Scalar>(2) * (w * j - i * k));
		Scalar cosP = sqrt(static_cast<Scalar>(1) - static_cast<Scalar>(2) * (w * j - i * k));
		angles.y = static_cast<Scalar>(2) * atan2(sinP, cosP) - pi() / static_cast<Scalar>(2);

		Scalar sinYCosP = static_cast<Scalar>(2) * (w * k + i * j);
		Scalar cosYCosP = static_cast<Scalar>(1) - static_cast<Scalar>(2) * (j * j + k * k);
		angles.z = atan2(sinYCosP, cosYCosP);

		return angles;
	}

	template <arithmetic_type Scalar, storage_mode Mode>
	constexpr quaternion<Scalar, Mode>
	quaternion<Scalar, Mode>::angle_axis(radians<scalar> angle, const vec_type& vec) noexcept
	{
		auto halfAngle = angle.value * static_cast<scalar>(0.5);
		return quaternion<scalar>(cos(halfAngle), sin(halfAngle) * normalize(vec));
	}

	template <arithmetic_type Scalar, storage_mode Mode>
	constexpr quaternion<Scalar, Mode>
	quaternion<Scalar, Mode>::look_at(const vec_type& pos, const vec_type& center, const vec_type& _up) noexcept
	{ // TODO(Glyn): Optimize
		vec_type const forward(normalize(center - pos));
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

	template <arithmetic_type Scalar, storage_mode Mode>
	constexpr quaternion<Scalar, Mode> quaternion<Scalar, Mode>::conjugate(const quaternion<Scalar, Mode>& quat
	) noexcept
	{
		return quaternion<Scalar>(quat.w, -quat.i, -quat.j, -quat.k);
	}

	template <arithmetic_type Scalar, storage_mode Mode>
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

	template <arithmetic_type Scalar, storage_mode Mode>
	constexpr vector<Scalar, 4, Mode> as_vec(quaternion<Scalar, Mode> quat) noexcept
	{
		return vector<Scalar, 4>(quat.i, quat.j, quat.k, quat.w);
	}

#define SIN22_5 0.3826834323650897717284599840304L
#define COS22_5 0.92387953251128675612818318939679L
#define SIN45 0.70710678118654752440084436210485L
#define COS45 0.70710678118654752440084436210485L

	template <arithmetic_type Scalar, storage_mode Mode>
	const quaternion<Scalar, Mode> quaternion<Scalar, Mode>::identity(
		static_cast<Scalar>(1), static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(0)
	);
	template <arithmetic_type Scalar, storage_mode Mode>
	const quaternion<Scalar, Mode> quaternion<Scalar, Mode>::rotate_x_45(
		static_cast<Scalar>(COS22_5), static_cast<Scalar>(SIN22_5), static_cast<Scalar>(0), static_cast<Scalar>(0)
	);
	template <arithmetic_type Scalar, storage_mode Mode>
	const quaternion<Scalar, Mode> quaternion<Scalar, Mode>::rotate_y_45(
		static_cast<Scalar>(COS22_5), static_cast<Scalar>(0), static_cast<Scalar>(SIN22_5), static_cast<Scalar>(0)
	);
	template <arithmetic_type Scalar, storage_mode Mode>
	const quaternion<Scalar, Mode> quaternion<Scalar, Mode>::rotate_z_45(
		static_cast<Scalar>(COS22_5), static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(SIN22_5)
	);
	template <arithmetic_type Scalar, storage_mode Mode>
	const quaternion<Scalar, Mode> quaternion<Scalar, Mode>::rotate_x_90(
		static_cast<Scalar>(COS45), static_cast<Scalar>(SIN45), static_cast<Scalar>(0), static_cast<Scalar>(0)
	);
	template <arithmetic_type Scalar, storage_mode Mode>
	const quaternion<Scalar, Mode> quaternion<Scalar, Mode>::rotate_y_90(
		static_cast<Scalar>(COS45), static_cast<Scalar>(0), static_cast<Scalar>(SIN45), static_cast<Scalar>(0)
	);
	template <arithmetic_type Scalar, storage_mode Mode>
	const quaternion<Scalar, Mode> quaternion<Scalar, Mode>::rotate_z_90(
		static_cast<Scalar>(COS45), static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(SIN45)
	);
	template <arithmetic_type Scalar, storage_mode Mode>
	const quaternion<Scalar, Mode> quaternion<Scalar, Mode>::rotate_x_180(
		static_cast<Scalar>(0), static_cast<Scalar>(1), static_cast<Scalar>(0), static_cast<Scalar>(0)
	);
	template <arithmetic_type Scalar, storage_mode Mode>
	const quaternion<Scalar, Mode> quaternion<Scalar, Mode>::rotate_y_180(
		static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(1), static_cast<Scalar>(0)
	);
	template <arithmetic_type Scalar, storage_mode Mode>
	const quaternion<Scalar, Mode> quaternion<Scalar, Mode>::rotate_z_180(
		static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(1)
	);

#undef SIN22_5
#undef COS22_5
#undef SIN45
#undef COS45
} // namespace rsl::math

#pragma once
#include "util.hpp"

namespace rsl::math
{
	template<typename Scalar>
	[[nodiscard]] matrix<Scalar, 4, 4> perspective(Scalar degrees, Scalar aspect, Scalar nearZ, Scalar farZ) noexcept
	{
		Scalar const tanHalfFovy = tan(radians(degrees) * static_cast<Scalar>(0.5));

		matrix<Scalar, 4, 4> result(static_cast<Scalar>(0.0));
		result[0][0] = 1.0f / (aspect * tanHalfFovy);
		result[1][1] = 1.0f / (tanHalfFovy);
		result[2][2] = (farZ + nearZ) / (farZ - nearZ);
		result[2][3] = static_cast<Scalar>(1.0);
		result[3][2] = -(2.0f * farZ * nearZ) / (farZ - nearZ);
		return result;
	}

	template<typename Scalar>
	[[nodiscard]] matrix<Scalar, 4, 4> orthographic(Scalar left, Scalar right, Scalar bottom, Scalar top, Scalar nearZ, Scalar farZ) noexcept
	{
		matrix<Scalar, 4, 4> result(static_cast<Scalar>(1.0));
		result[0][0] = static_cast<Scalar>(2.0) / (right - left);
		result[1][1] = static_cast<Scalar>(2.0) / (top - bottom);
		result[2][2] = static_cast<Scalar>(1.0) / (farZ - nearZ);

		result[3][0] = -(right + left) / (right - left);
		result[3][1] = -(top + bottom) / (top - bottom);
		result[3][2] = -nearZ / (farZ - nearZ);

		return result;
	}

	template<typename Scalar>
	[[nodiscard]] matrix<Scalar, 4, 4> lookAt(vector<Scalar, 3> position, vector<Scalar, 3> target, vector<Scalar, 3> up) noexcept
	{
		vector<Scalar, 3> const forward(normalize(target - position));
		vector<Scalar, 3> const right(normalize(cross(up, forward)));
		vector<Scalar, 3> const newup(normalize(cross(forward, right)));

		matrix<Scalar, 4, 4> result(1);
		//New Right
		result[0][0] = right.x;
		result[1][0] = right.y;
		result[2][0] = right.z;

		//New Up
		result[0][1] = newup.x;
		result[1][1] = newup.y;
		result[2][1] = newup.z;

		//New Forward
		result[0][2] = forward.x;
		result[1][2] = forward.y;
		result[2][2] = forward.z;

		result[3][0] = -dot(right, position);
		result[3][1] = -dot(newup, position);
		result[3][2] = -dot(forward, position);

		return result;
	}

	template<typename Scalar>
	[[nodiscard]] matrix<Scalar, 4, 4> transpose(matrix<Scalar, 4, 4> mat) noexcept
	{
		matrix<Scalar, 4, 4> result(1);
		result[0][0] = mat[0][0];
		result[0][1] = mat[1][0];
		result[0][2] = mat[2][0];
		result[0][3] = mat[3][0];

		result[1][0] = mat[0][1];
		result[1][1] = mat[1][1];
		result[1][2] = mat[2][1];
		result[1][3] = mat[3][1];

		result[2][0] = mat[0][2];
		result[2][1] = mat[1][2];
		result[2][2] = mat[2][2];
		result[2][3] = mat[3][2];

		result[3][0] = mat[0][3];
		result[3][1] = mat[1][3];
		result[3][2] = mat[2][3];
		result[3][3] = mat[3][3];
		return result;
	}

	template<typename Scalar>
	[[nodiscard]] matrix<Scalar, 4, 4> translate(matrix<Scalar, 4, 4> mat, vector<Scalar, 3> pos) noexcept
	{
		matrix<Scalar, 4, 4> result(mat);
		result[3] = mat[0] * pos[0] + mat[1] * pos[1] + mat[2] * pos[2] + mat[3];
		return result;
	}
	//template<typename Scalar>
	//[[nodiscard]] matrix<Scalar, 4, 4> rotate(matrix<Scalar, 4, 4> matrix, quaternion<Scalar> orientation) noexcept
	//{

	//}
	template<typename Scalar>
	[[nodiscard]] matrix<Scalar, 4, 4> rotate(matrix<Scalar, 4, 4> mat, Scalar rad, vector<Scalar, 3> axis) noexcept
	{
		Scalar const a = rad;
		Scalar const c = cos(a);
		Scalar const s = sin(a);

		vector<Scalar, 3> _axis(normalize(axis));
		vector<Scalar, 3> temp((Scalar(1) - c) * _axis);

		matrix<Scalar, 4, 4> rot;
		rot[0][0] = c + temp[0] * _axis[0];
		rot[0][1] = temp[0] * _axis[1] + s * _axis[2];
		rot[0][2] = temp[0] * _axis[2] - s * _axis[1];

		rot[1][0] = temp[1] * _axis[0] - s * _axis[2];
		rot[1][1] = c + temp[1] * _axis[1];
		rot[1][2] = temp[1] * _axis[2] + s * _axis[0];

		rot[2][0] = temp[2] * _axis[0] + s * _axis[1];
		rot[2][1] = temp[2] * _axis[1] - s * _axis[0];
		rot[2][2] = c + temp[2] * _axis[2];

		matrix<Scalar, 4, 4> result;
		result[0] = mat[0] * rot[0][0] + mat[1] * rot[0][1] + mat[2] * rot[0][2];
		result[1] = mat[0] * rot[1][0] + mat[1] * rot[1][1] + mat[2] * rot[1][2];
		result[2] = mat[0] * rot[2][0] + mat[1] * rot[2][1] + mat[2] * rot[2][2];
		result[3] = mat[3];
		return result;
	}
	template<typename Scalar>
	[[nodiscard]] matrix<Scalar, 4, 4> scale(matrix<Scalar, 4, 4> mat, vector<Scalar, 3> scale) noexcept
	{
		matrix<Scalar, 4, 4> result;
		result[0] = mat[0] * scale[0];
		result[1] = mat[1] * scale[1];
		result[2] = mat[2] * scale[2];
		result[3] = mat[3];
		return result;
	}
}
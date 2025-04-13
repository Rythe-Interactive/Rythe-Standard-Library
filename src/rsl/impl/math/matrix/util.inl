#pragma once
#include "util.hpp"

namespace rsl::math
{
	template <typename Scalar>
	[[nodiscard]] matrix<Scalar, 4, 4> perspective(Scalar fovY, Scalar aspect, Scalar nearZ, Scalar farZ) noexcept
	{
		Scalar const tanHalfFovY = tan(fovY * static_cast<Scalar>(0.5));
		Scalar inverseFrustumDepth = static_cast<Scalar>(1) / (farZ - nearZ);

		matrix<Scalar, 4, 4> result(static_cast<Scalar>(0));

		result[0][0] = static_cast<Scalar>(1) / (aspect * tanHalfFovY);
		result[1][1] = static_cast<Scalar>(1) / (tanHalfFovY);
		result[2][2] = (farZ + nearZ) * inverseFrustumDepth;

		result[2][3] = static_cast<Scalar>(1);
		result[3][2] = -(static_cast<Scalar>(2) * nearZ * farZ) * inverseFrustumDepth;

		return result;
	}

	template <typename Scalar>
	[[nodiscard]] matrix<Scalar, 4, 4>
	orthographic(Scalar left, Scalar right, Scalar bottom, Scalar top, Scalar nearZ, Scalar farZ) noexcept
	{
		vector<Scalar, 3> frustumSize = vector<Scalar, 3>(right, top, farZ) - vector<Scalar, 3>(left, bottom, nearZ);
		vector<Scalar, 3> inverseFrustumSize = vector<Scalar, 3>(static_cast<Scalar>(1)) / frustumSize;

		vector<Scalar, 3> result0 =
			vector<Scalar, 3>(static_cast<Scalar>(2), static_cast<Scalar>(2), static_cast<Scalar>(1)) *
			inverseFrustumSize;

		matrix<Scalar, 4, 4> result(static_cast<Scalar>(1));

		result[0][0] = result0.x;
		result[1][1] = result0.y;
		result[2][2] = result0.z;

		result[3].xyz = vector<Scalar, 3>(-(right + left), -(top + bottom), -nearZ) * inverseFrustumSize;

		return result;
	}

	template <typename Scalar>
	[[nodiscard]] matrix<Scalar, 4, 4>
	look_at(vector<Scalar, 3> eye, vector<Scalar, 3> center, vector<Scalar, 3> up) noexcept
	{
		const vector<Scalar, 4> f(normalize(center - eye));
		const vector<Scalar, 4> r(normalize(cross(up, f)));
		const vector<Scalar, 4> u(cross(f, r)); // Length of u is 1 because the angle between f and r is 90 degrees

		matrix<Scalar, 4, 4> result(static_cast<Scalar>(1));
		result.col0 = r;
		result.col1 = u;
		result.col2 = f;

		vector<Scalar, 3> x{r.x, u.x, f.x};
		vector<Scalar, 3> y{r.y, u.y, f.y};
		vector<Scalar, 3> z{r.z, u.z, f.z};

		result.col3 = -(x * eye.xxx + y * eye.yyy + z * eye.zzz);

		return result;
	}

	template <typename Scalar>
	constexpr matrix<Scalar, 4, 4> transpose(const matrix<Scalar, 4, 4>& mat) noexcept
	{
		if (is_constant_evaluated())
		{
			matrix<Scalar, 4, 4> result{};
			for (size_t i = 0; i < 4; ++i)
			{
				for (size_t j = 0; j < 4; ++j)
				{
					result[j][i] = mat[i][j];
				}
			}
			return result;
		}
		else
		{
			matrix<Scalar, 4, 4> result(1);
			result.row0 = mat.col0;
			result.row1 = mat.col1;
			result.row2 = mat.col2;
			result.row3 = mat.col3;
			return result;
		}
	}

	template <typename Scalar>
	matrix<Scalar, 4, 4> constexpr translate(const matrix<Scalar, 4, 4>& mat, const vector<Scalar, 3>& pos) noexcept
	{
		if (is_constant_evaluated())
		{
			matrix<Scalar, 4, 4> result(mat);
			result[3][0] += pos[0];
			result[3][1] += pos[1];
			result[3][2] += pos[2];
			return result;
		}
		else
		{
			matrix<Scalar, 4, 4> result(mat);
			result[3].xyz += pos;
			return result;
		}
	}

	// template<typename Scalar>
	//[[nodiscard]] matrix<Scalar, 4, 4> rotate(matrix<Scalar, 4, 4> matrix, quaternion<Scalar> orientation) noexcept
	//{

	//}

	template <typename Scalar>
	matrix<Scalar, 4, 4>
	rotate(const matrix<Scalar, 4, 4>& mat, radians<Scalar> angle, const vector<Scalar, 3>& _axis) noexcept
	{
		Scalar const a = angle;
		Scalar const c = cos(a);
		Scalar const s = sin(a);

		vector<Scalar, 3> axis(normalize(_axis));
		vector<Scalar, 3> temp((Scalar(1) - c) * axis);

		matrix<Scalar, 4, 4> rot;
		rot[0][0] = c + temp[0] * axis[0];
		rot[0][1] = temp[0] * axis[1] + s * axis[2];
		rot[0][2] = temp[0] * axis[2] - s * axis[1];

		rot[1][0] = temp[1] * axis[0] - s * axis[2];
		rot[1][1] = c + temp[1] * axis[1];
		rot[1][2] = temp[1] * axis[2] + s * axis[0];

		rot[2][0] = temp[2] * axis[0] + s * axis[1];
		rot[2][1] = temp[2] * axis[1] - s * axis[0];
		rot[2][2] = c + temp[2] * axis[2];

		matrix<Scalar, 4, 4> result;
		result[0] = mat[0] * rot[0][0] + mat[1] * rot[0][1] + mat[2] * rot[0][2];
		result[1] = mat[0] * rot[1][0] + mat[1] * rot[1][1] + mat[2] * rot[1][2];
		result[2] = mat[0] * rot[2][0] + mat[1] * rot[2][1] + mat[2] * rot[2][2];
		result[3] = mat[3];
		return result;
	}

	template <typename Scalar>
	constexpr matrix<Scalar, 4, 4> scale(const matrix<Scalar, 4, 4>& mat, const vector<Scalar, 3>& scale) noexcept
	{
		matrix<Scalar, 4, 4> result;
		result[0] = mat[0] * scale[0];
		result[1] = mat[1] * scale[1];
		result[2] = mat[2] * scale[2];
		result[3] = mat[3];
		return result;
	}
} // namespace rsl::math

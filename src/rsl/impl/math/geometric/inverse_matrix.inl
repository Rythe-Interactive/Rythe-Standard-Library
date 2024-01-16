#pragma once
#include "inverse.hpp"

namespace rsl::math::detail
{
	template<typename T>
	struct compute_inverse;

	template<typename Scalar, size_type RowCount, size_type ColCount>
	struct compute_inverse<matrix<Scalar, RowCount, ColCount>>
	{
		using mat_type = matrix<Scalar, RowCount, ColCount>;

		[[nodiscard]] rythe_always_inline static auto compute(const mat_type& m) noexcept
		{
			static_assert(RowCount == ColCount, "Determinants can only be calculated of square matrices");
			
			if constexpr (RowCount == 2)
			{
				Scalar OneOverDeterminant = static_cast<Scalar>(1) / (
					+m[0][0] * m[1][1]
					- m[1][0] * m[0][1]);

				matrix<Scalar,2,2> Inverse(
					+m[1][1] * OneOverDeterminant,
					-m[0][1] * OneOverDeterminant,
					-m[1][0] * OneOverDeterminant,
					+m[0][0] * OneOverDeterminant);

				return Inverse;
			}
			else if constexpr (RowCount == 3)
			{
				Scalar OneOverDeterminant = static_cast<Scalar>(1) / (
					+m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2])
					- m[1][0] * (m[0][1] * m[2][2] - m[2][1] * m[0][2])
					+ m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]));

				matrix<Scalar, 3, 3>  Inverse;
				Inverse[0][0] = +(m[1][1] * m[2][2] - m[2][1] * m[1][2]) * OneOverDeterminant;
				Inverse[1][0] = -(m[1][0] * m[2][2] - m[2][0] * m[1][2]) * OneOverDeterminant;
				Inverse[2][0] = +(m[1][0] * m[2][1] - m[2][0] * m[1][1]) * OneOverDeterminant;
				Inverse[0][1] = -(m[0][1] * m[2][2] - m[2][1] * m[0][2]) * OneOverDeterminant;
				Inverse[1][1] = +(m[0][0] * m[2][2] - m[2][0] * m[0][2]) * OneOverDeterminant;
				Inverse[2][1] = -(m[0][0] * m[2][1] - m[2][0] * m[0][1]) * OneOverDeterminant;
				Inverse[0][2] = +(m[0][1] * m[1][2] - m[1][1] * m[0][2]) * OneOverDeterminant;
				Inverse[1][2] = -(m[0][0] * m[1][2] - m[1][0] * m[0][2]) * OneOverDeterminant;
				Inverse[2][2] = +(m[0][0] * m[1][1] - m[1][0] * m[0][1]) * OneOverDeterminant;
				return Inverse;
			}
			else if constexpr(RowCount == 4)
			{

				Scalar Coef00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
				Scalar Coef02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
				Scalar Coef03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

				Scalar Coef04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
				Scalar Coef06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
				Scalar Coef07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

				Scalar Coef08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
				Scalar Coef10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
				Scalar Coef11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

				Scalar Coef12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
				Scalar Coef14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
				Scalar Coef15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

				Scalar Coef16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
				Scalar Coef18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
				Scalar Coef19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

				Scalar Coef20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
				Scalar Coef22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
				Scalar Coef23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

				vector<Scalar, 4> Fac0(Coef00, Coef00, Coef02, Coef03);
				vector<Scalar, 4> Fac1(Coef04, Coef04, Coef06, Coef07);
				vector<Scalar, 4> Fac2(Coef08, Coef08, Coef10, Coef11);
				vector<Scalar, 4> Fac3(Coef12, Coef12, Coef14, Coef15);
				vector<Scalar, 4> Fac4(Coef16, Coef16, Coef18, Coef19);
				vector<Scalar, 4> Fac5(Coef20, Coef20, Coef22, Coef23);

				vector<Scalar, 4>  Vec0(m[1][0], m[0][0], m[0][0], m[0][0]);
				vector<Scalar, 4>  Vec1(m[1][1], m[0][1], m[0][1], m[0][1]);
				vector<Scalar, 4>  Vec2(m[1][2], m[0][2], m[0][2], m[0][2]);
				vector<Scalar, 4>  Vec3(m[1][3], m[0][3], m[0][3], m[0][3]);

				vector<Scalar, 4> Inv0(Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
				vector<Scalar, 4> Inv1(Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
				vector<Scalar, 4> Inv2(Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);
				vector<Scalar, 4> Inv3(Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);

				vector<Scalar, 4> SignA(+1, -1, +1, -1);
				vector<Scalar, 4> SignB(-1, +1, -1, +1);
				matrix<Scalar, 4, 4> Inverse(Inv0 * SignA, Inv1 * SignB, Inv2 * SignA, Inv3 * SignB);

				vector<Scalar, 4> Row0(Inverse[0][0], Inverse[1][0], Inverse[2][0], Inverse[3][0]);

				vector<Scalar, 4> Dot0(m[0] * Row0);
				Scalar Dot1 = (Dot0.x + Dot0.y) + (Dot0.z + Dot0.w);

				matrix<Scalar, 4, 4> result(Inverse[0] / Dot1, Inverse[1] / Dot1, Inverse[2] / Dot1, Inverse[3] / Dot1);
				return result;
			}
		}
	};
}

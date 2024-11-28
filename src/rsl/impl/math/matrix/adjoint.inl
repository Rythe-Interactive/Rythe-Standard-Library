#include "adjoint.hpp"
#pragma once

namespace rsl::math
{
	namespace internal
	{
		template <typename T>
		struct compute_adjoint;

		template <typename Scalar, size_type RowCount, size_type ColCount>
		struct compute_adjoint<matrix<Scalar, RowCount, ColCount>>
		{
			using mat_type = matrix<Scalar, RowCount, ColCount>;
			using sub_mat_type = matrix<Scalar, RowCount - 1, ColCount - 1>;

			[[nodiscard]] [[rythe_always_inline]] constexpr static auto compute(const mat_type& v) noexcept
			{
				static_assert(RowCount == ColCount, "Adjoint can only be calculated of square matrices");


				mat_type result;
				for (size_type i = 0; i < RowCount; i++)
				{
					for (size_type j = 0; j < ColCount; j++)
					{
						auto subMat = extract_sub_mat(v, i, j);
						Scalar s = static_cast<Scalar>(((i + j) % 2) ? -1 : 1);
						result[j][i] = static_cast<Scalar>(s * determinant(subMat));
					}
				}

				return result;
			}
		};
	} // namespace internal

	template <typename mat_type, ::std::enable_if_t<is_matrix_v<mat_type>, bool>>
	[[nodiscard]] constexpr auto adjoint(const mat_type& mat) noexcept
	{
		return internal::compute_adjoint<mat_type>::compute(mat);
	}
} // namespace rsl::math

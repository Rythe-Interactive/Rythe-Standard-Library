#include "determinant.hpp"
#pragma once

namespace rsl::math
{
	namespace detail
	{
		template <typename mat_type>
		[[nodiscard]] rythe_always_inline constexpr auto extract_sub_mat(const mat_type& v, size_type rowIdx, size_type colIdx)
		{
			using sub_mat_type = matrix<typename mat_type::scalar, mat_type::row_count - 1, mat_type::col_count - 1>;

			sub_mat_type result;
			size_type resultRow = 0;
			size_type resultCol = 0;

			for (size_type srcRow = 1; srcRow < mat_type::row_count; srcRow++)
				if (srcRow != rowIdx)
					for (size_type srcCol = 0; srcCol < mat_type::col_count; srcCol++)
						if (srcCol != colIdx)
						{
							if (resultCol >= sub_mat_type::col_count)
							{
								resultCol = 0;
								resultRow++;
							}

							result[resultRow][resultCol++] = v[srcRow][srcCol];
						}

			return result;
		}

		template <typename T>
		struct compute_determinant;

		template <typename Scalar, size_type RowCount, size_type ColCount>
		struct compute_determinant<matrix<Scalar, RowCount, ColCount>>
		{
			using mat_type = matrix<Scalar, RowCount, ColCount>;
			using sub_mat_type = matrix<Scalar, RowCount - 1, ColCount - 1>;

			[[nodiscard]] rythe_always_inline constexpr static Scalar compute(const mat_type& v) noexcept
			{
				static_assert(RowCount == ColCount, "Determinants can only be calculated of square matrices");

				Scalar result = static_cast<Scalar>(0);
				for (size_type i = 0; i < ColCount; i++)
					result += static_cast<Scalar>(i % 2 ? -1 : 1) * v[0][i] * compute_determinant<sub_mat_type>::compute(extract_sub_mat(v, 0, i));
				return result;
			}
		};

		template <typename Scalar>
		struct compute_determinant<matrix<Scalar, 1, 1>>
		{
			using mat_type = matrix<Scalar, 1, 1>;

			[[nodiscard]] rythe_always_inline constexpr static Scalar compute(const mat_type& v) noexcept
			{
				return v[0][0];
			}
		};

		template <typename Scalar>
		struct compute_determinant<matrix<Scalar, 2, 2>>
		{
			using mat_type = matrix<Scalar, 2, 2>;

			[[nodiscard]] rythe_always_inline constexpr static Scalar compute(const mat_type& v) noexcept
			{
				return v[0][0] * v[1][1] - v[0][1] * v[1][0];
			}
		};
	} // namespace detail

	template <typename mat_type, ::std::enable_if_t<is_matrix_v<mat_type>, bool>>
	[[nodiscard]] rythe_always_inline constexpr auto determinant(const mat_type& mat) noexcept
	{
		return detail::compute_determinant<mat_type>::compute(mat);
	}
} // namespace rsl::math

#pragma once
#include "../vector/vector3.hpp"
#include "column/column_base.hpp"
#include "matrix_base.hpp"

namespace rsl::math
{
	template <typename Scalar>
	struct quaternion;

	template <typename Scalar>
	struct matrix<Scalar, 3, 3> : matrix_base
	{
		using scalar = Scalar;
		static constexpr size_type row_count = 3;
		static constexpr size_type col_count = 3;
		static constexpr size_type size = row_count * col_count;
		using type = matrix<Scalar, 3, 3>;

		using row_type = vector<scalar, col_count>;

		template <size_type idx>
		using col_type = column<scalar, row_count, col_count, idx>;

		union
		{
			row_type rows[row_count];

			struct
			{
				row_type row0, row1, row2;
			};

			scalar data[size];
			col_type<0> col0;
			col_type<1> col1;
			col_type<2> col2;
		};

		static const matrix identity;
		static const matrix zero;

		constexpr matrix() noexcept
			: row0(float3::right),
			  row1(float3::up),
			  row2(float3::forward)
		{
		}

		constexpr matrix(const matrix&) noexcept = default;

		explicit constexpr matrix(scalar s) noexcept
			: row0(s, static_cast<scalar>(0), static_cast<scalar>(0)),
			  row1(static_cast<scalar>(0), s, static_cast<scalar>(0)),
			  row2(static_cast<scalar>(0), static_cast<scalar>(0), s)
		{
		}

		explicit constexpr matrix(
			scalar s00, scalar s01, scalar s02,
			scalar s10, scalar s11, scalar s12,
			scalar s20, scalar s21, scalar s22
		) noexcept
			: row0(s00, s01, s02),
			  row1(s10, s11, s12),
			  row2(s20, s21, s22)
		{
		}

		explicit constexpr matrix(row_type r0, row_type r1, row_type r2) noexcept
			: row0(r0),
			  row1(r1),
			  row2(r2)
		{
		}

		template <typename Scal>
		explicit constexpr matrix(const quaternion<Scal>& orientation) noexcept;

		template <typename Scal, ::std::enable_if_t<!::std::is_same_v<scalar, Scal>, bool> = true>
		constexpr explicit matrix(const matrix<Scal, row_count, col_count>& other) noexcept;

		template <typename mat_type, ::std::enable_if_t<3 != mat_type::row_count || 3 != mat_type::col_count, bool> = true>
		constexpr matrix(const mat_type& other) noexcept;

		constexpr matrix& operator=(const matrix&) noexcept = default;

		[[nodiscard]] constexpr row_type& operator[](size_type i) noexcept
		{
			rsl_assert_msg((i >= 0) && (i < row_count), "matrix subscript out of range");
			return rows[i];
		}
		[[nodiscard]] constexpr const row_type& operator[](size_type i) const noexcept
		{
			rsl_assert_msg((i >= 0) && (i < row_count), "matrix subscript out of range");
			return rows[i];
		}
	};

	using float3x3 = matrix<float32, 3, 3>;
	using mat3 = float3x3;
	using double3x3 = matrix<float64, 3, 3>;
	using dmat3 = double3x3;
} // namespace rsl::math

#include "../quaternion/matrix_quat_conv.inl"

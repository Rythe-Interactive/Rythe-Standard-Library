#pragma once
#include "../vector/vector4.hpp"
#include "column/column_base.hpp"
#include "matrix_base.hpp"

namespace rsl::math
{
	template <typename Scalar>
	struct quaternion;

	template <typename Scalar>
	struct matrix<Scalar, 4, 4> : matrix_base
	{
		using scalar = Scalar;
		static constexpr size_type row_count = 4;
		static constexpr size_type col_count = 4;
		static constexpr size_type size = row_count * col_count;
		using type = matrix<Scalar, 4, 4>;

		using row_type = vector<scalar, col_count>;

		template <size_type idx>
		using col_type = column<scalar, row_count, col_count, idx>;

		union
		{
			row_type rows[row_count];

			struct
			{
				row_type row0, row1, row2, row3;
			};

			scalar data[size];
			col_type<0> col0;
			col_type<1> col1;
			col_type<2> col2;
			col_type<3> col3;
		};

		static const matrix identity;
		static const matrix zero;

		constexpr matrix() noexcept
			: row0(float4::right),
			  row1(float4::up),
			  row2(float4::forward),
			  row3(float4::positiveW)
		{
		}

		constexpr matrix(const matrix&) noexcept = default;

		explicit constexpr matrix(scalar s) noexcept
			: row0(s, static_cast<scalar>(0), static_cast<scalar>(0), static_cast<scalar>(0)),
			  row1(static_cast<scalar>(0), s, static_cast<scalar>(0), static_cast<scalar>(0)),
			  row2(static_cast<scalar>(0), static_cast<scalar>(0), s, static_cast<scalar>(0)),
			  row3(static_cast<scalar>(0), static_cast<scalar>(0), static_cast<scalar>(0), s)
		{
		}

		explicit constexpr matrix(
			scalar s00, scalar s01, scalar s02, scalar s03,
			scalar s10, scalar s11, scalar s12, scalar s13,
			scalar s20, scalar s21, scalar s22, scalar s23,
			scalar s30, scalar s31, scalar s32, scalar s33
		) noexcept
			: row0(s00, s01, s02, s03),
			  row1(s10, s11, s12, s13),
			  row2(s20, s21, s22, s23),
			  row3(s30, s31, s32, s33)
		{
		}

		explicit constexpr matrix(row_type r0, row_type r1, row_type r2, row_type r3) noexcept
			: row0(r0),
			  row1(r1),
			  row2(r2),
			  row3(r3)
		{
		}

		template <typename Scal>
		explicit constexpr matrix(const quaternion<Scal>& orientation, const float3& position = float3::zero) noexcept;

		template <typename Scal, ::std::enable_if_t<!::std::is_same_v<scalar, Scal>, bool> = true>
		constexpr explicit matrix(const matrix<Scal, row_count, col_count>& other) noexcept;

		template <typename mat_type, ::std::enable_if_t<4 != mat_type::row_count || 4 != mat_type::col_count, bool> = true>
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

	template <typename Scalar>
	const matrix<Scalar, 4, 4> matrix<Scalar, 4, 4>::identity = matrix<Scalar, 4, 4>(1.0f);
	template <typename Scalar>
	const matrix<Scalar, 4, 4> matrix<Scalar, 4, 4>::zero = matrix<Scalar, 4, 4>(0.0f);

	using float4x4 = matrix<float32, 4, 4>;
	using mat4 = float4x4;
	using double4x4 = matrix<float64, 4, 4>;
	using dmat4 = double4x4;
} // namespace rsl::math

#include "../quaternion/matrix_quat_conv.inl"

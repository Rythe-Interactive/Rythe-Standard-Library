#pragma once
#include "predefined.hpp"

RYTHE_MSVC_SUPPRESS_WARNING_WITH_PUSH(4201) // anonymous struct

namespace rsl::math
{
	template <arithmetic_type Scalar, mode Mode>
	struct matrix<Scalar, 4, 4, Mode>
	{
		using scalar = Scalar;
		static constexpr size_type row_count = 4;
		static constexpr size_type col_count = 4;
		static constexpr size_type size = row_count * col_count;
		static constexpr mode mode = Mode;

		using row_type = vector<scalar, col_count, Mode>;

		template <size_type idx>
		using col_type = column<scalar, row_count, col_count, idx, Mode>;

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

		[[rythe_always_inline]] constexpr matrix() noexcept;
		[[rythe_always_inline]] constexpr matrix(const matrix&) noexcept = default;
		[[rythe_always_inline]] explicit constexpr matrix(scalar s) noexcept;
		[[rythe_always_inline]] explicit constexpr matrix(scalar s, uniform_matrix_signal) noexcept;
		[[rythe_always_inline]] explicit constexpr matrix(scalar s, identity_matrix_signal) noexcept;
		[[rythe_always_inline]] explicit constexpr matrix(
			scalar s00, scalar s01, scalar s02, scalar s03, scalar s10, scalar s11, scalar s12, scalar s13, scalar s20,
			scalar s21, scalar s22, scalar s23, scalar s30, scalar s31, scalar s32, scalar s33
		) noexcept;
		[[rythe_always_inline]] explicit constexpr matrix(row_type r0, row_type r1, row_type r2, row_type r3) noexcept;

		template <arithmetic_type Scal0, math::mode M0, arithmetic_type Scal1, math::mode M1>
		[[rythe_always_inline]] explicit constexpr matrix(
			const quaternion<Scal0, M0>& orientation, const vector<Scal1, 3, M1>& position = vector<Scal1, 3, M1>::zero
		) noexcept;

		template <typename mat_type>
			requires not_same_as<Scalar, typename mat_type::scalar> || (mat_type::row_count != 4) ||
					 (mat_type::col_count != 4)
		[[rythe_always_inline]] constexpr matrix(const mat_type& other) noexcept;

		[[rythe_always_inline]] constexpr matrix& operator=(const matrix&) noexcept = default;

		[[nodiscard]] [[rythe_always_inline]] constexpr row_type& operator[](size_type i) noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const row_type& operator[](size_type i) const noexcept;
	};

	using float4x4 = matrix<float32, 4, 4>;
	using double4x4 = matrix<float64, 4, 4>;
	using mat4 = float4x4;
	using dmat4 = double4x4;
} // namespace rsl::math

RYTHE_MSVC_SUPPRESS_WARNING_POP

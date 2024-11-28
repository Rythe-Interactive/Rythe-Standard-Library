#pragma once
#include "predefined.hpp"

RYTHE_MSVC_SUPPRESS_WARNING_WITH_PUSH(4201) // anonymous struct

namespace rsl::math
{
	template <arithmetic_type Scalar, mode Mode>
	struct matrix<Scalar, 2, 2, Mode>
	{
		using scalar = Scalar;
		static constexpr size_type row_count = 2;
		static constexpr size_type col_count = 2;
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
				row_type row0, row1;
			};

			scalar data[size];
			col_type<0> col0;
			col_type<1> col1;
		};

		static const matrix identity;
		static const matrix zero;

		[[rythe_always_inline]] constexpr matrix() noexcept;
		[[rythe_always_inline]] constexpr matrix(const matrix&) noexcept = default;
		[[rythe_always_inline]] explicit constexpr matrix(scalar s) noexcept;
		[[rythe_always_inline]] explicit constexpr matrix(scalar s, uniform_matrix_signal) noexcept;
		[[rythe_always_inline]] explicit constexpr matrix(scalar s, identity_matrix_signal) noexcept;
		[[rythe_always_inline]] explicit constexpr matrix(scalar s00, scalar s01, scalar s10, scalar s11) noexcept;
		[[rythe_always_inline]] explicit constexpr matrix(row_type r0, row_type r1) noexcept;

		template <arithmetic_type Scal, math::mode M>
		[[rythe_always_inline]] explicit constexpr matrix(const quaternion<Scal, M>& orientation) noexcept;

		template <typename mat_type>
			requires not_same_as<Scalar, typename mat_type::scalar> || (mat_type::row_count != 2) ||
					 (mat_type::col_count != 2)
		[[rythe_always_inline]] constexpr matrix(const mat_type& other) noexcept;

		[[rythe_always_inline]] constexpr matrix& operator=(const matrix&) noexcept = default;

		[[nodiscard]] [[rythe_always_inline]] constexpr row_type& operator[](size_type i) noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const row_type& operator[](size_type i) const noexcept;
	};

	using float2x2 = matrix<float32, 2, 2>;
	using double2x2 = matrix<float64, 2, 2>;
} // namespace rsl::math

RYTHE_MSVC_SUPPRESS_WARNING_POP

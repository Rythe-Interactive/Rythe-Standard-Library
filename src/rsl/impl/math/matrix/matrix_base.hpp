#pragma once
#include "../../util/primitives.hpp"

#include "../vector/vector_base.hpp"

namespace rsl::math
{
	struct matrix_base
	{
	};

	struct uniform_matrix_signal
	{
	};
	struct identity_matrix_signal
	{
	};

	template <typename Scalar, size_type RowCount, size_type ColCount>
	struct matrix : matrix_base
	{
		using scalar = Scalar;
		static constexpr size_type row_count = RowCount;
		static constexpr size_type col_count = ColCount;
		static constexpr size_type size = row_count * col_count;
		using type = matrix<Scalar, RowCount, ColCount>;

		using row_type = vector<scalar, col_count>;

		union
		{
			row_type rows[row_count];
			scalar data[size];
		};

		static const matrix identity;
		static const matrix zero;

		constexpr matrix() noexcept;

		constexpr matrix(const matrix&) noexcept = default;
		explicit constexpr matrix(scalar s) noexcept;
		explicit constexpr matrix(scalar s, uniform_matrix_signal) noexcept;
		explicit constexpr matrix(scalar s, identity_matrix_signal) noexcept;

		template <typename Scal, ::std::enable_if_t<!::std::is_same_v<scalar, Scal>, bool> = true>
		constexpr explicit matrix(const matrix<Scal, row_count, col_count>& other) noexcept;

		template <typename mat_type, ::std::enable_if_t<RowCount != mat_type::row_count || ColCount != mat_type::col_count, bool> = true>
		constexpr matrix(const mat_type& other) noexcept;

		constexpr matrix& operator=(const matrix&) noexcept = default;

		[[nodiscard]] constexpr row_type& operator[](size_type i) noexcept;
		[[nodiscard]] constexpr const row_type& operator[](size_type i) const noexcept;
	};
} // namespace rsl::math

#include "matrix_base.inl"

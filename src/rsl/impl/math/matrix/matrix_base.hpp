#pragma once
#include "predefined.hpp"

namespace rsl::math
{
	struct uniform_matrix_signal
	{
	};
	struct identity_matrix_signal
	{
	};

	template <
		arithmetic_type Scalar, size_type RowCount, size_type ColCount, storage_mode Mode = storage_mode::defaultp>
	struct matrix
	{
		using scalar = Scalar;
		static constexpr size_type row_count = RowCount;
		static constexpr size_type col_count = ColCount;
		static constexpr size_type size = row_count * col_count;
		static constexpr storage_mode mode = Mode;

		using col_type = vector<scalar, row_count, Mode>;

		union
		{
			col_type columns[col_count];
			scalar data[size];
		};

		static const matrix identity;
		static const matrix zero;

		[[rythe_always_inline]] constexpr matrix() noexcept;

		[[rythe_always_inline]] constexpr matrix(const matrix&) noexcept = default;
		[[rythe_always_inline]] explicit constexpr matrix(scalar s) noexcept;
		[[rythe_always_inline]] explicit constexpr matrix(scalar s, uniform_matrix_signal) noexcept;
		[[rythe_always_inline]] explicit constexpr matrix(scalar s, identity_matrix_signal) noexcept;

		template <typename mat_type>
			requires not_same_as<Scalar, typename mat_type::scalar> || (mat_type::row_count != RowCount) ||
					 (mat_type::col_count != ColCount)
		[[rythe_always_inline]] constexpr matrix(const mat_type& other) noexcept;

		[[rythe_always_inline]] constexpr matrix& operator=(const matrix&) noexcept = default;

		[[nodiscard]] [[rythe_always_inline]] constexpr col_type& operator[](size_type i) noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const col_type& operator[](size_type i) const noexcept;
	};
} // namespace rsl::math

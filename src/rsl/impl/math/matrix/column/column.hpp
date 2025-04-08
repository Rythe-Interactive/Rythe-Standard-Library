#pragma once
#include "../predefined.hpp"

namespace rsl::math
{
	template <arithmetic_type Scalar, size_type RowCount, size_type ColCount, size_type ColIdx, storage_mode Mode>
	struct column
	{
		using scalar = Scalar;
		static constexpr size_type size = RowCount;
		static constexpr storage_mode mode = Mode;
		static constexpr size_type row_count = RowCount;
		static constexpr size_type col_count = ColCount;
		static constexpr size_type col_idx = ColIdx;
		using conv_type = vector<scalar, size, Mode>;
		using storage_type = vector<scalar, col_count, Mode>;

		storage_type mx[row_count];

		COPY_FUNCS_DECO([[rythe_always_inline]] constexpr, column, noexcept);

		[[nodiscard]] [[rythe_always_inline]] constexpr operator conv_type() const noexcept;

		[[rythe_always_inline]] constexpr column& operator=(const conv_type& other) noexcept;

		[[rythe_always_inline]] constexpr scalar& operator[](size_type i) noexcept
		{
			rsl_assert_out_of_range_msg((i >= 0) && (i < size), "vector subscript out of range");
			return mx[i][ColIdx];
		}

		[[rythe_always_inline]] constexpr const scalar& operator[](size_type i) const noexcept
		{
			rsl_assert_out_of_range_msg((i >= 0) && (i < size), "vector subscript out of range");
			return mx[i][ColIdx];
		}
	};

	template <size_type RowCount, size_type ColCount, size_type ColIdx, storage_mode Mode>
	struct column<bool, RowCount, ColCount, ColIdx, Mode>
	{
		using scalar = bool;
		static constexpr size_type size = RowCount;
		static constexpr storage_mode mode = Mode;
		static constexpr size_type row_count = RowCount;
		static constexpr size_type col_count = ColCount;
		static constexpr size_type col_idx = ColIdx;
		using conv_type = vector<scalar, size, Mode>;
		using storage_type = vector<scalar, col_count, Mode>;

		storage_type mx[row_count];

		COPY_FUNCS_DECO([[rythe_always_inline]] constexpr, column, noexcept);

		[[rythe_always_inline]] constexpr void set_mask(bitfield8 mask) noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr bitfield8 mask() const noexcept;

		[[nodiscard]] [[rythe_always_inline]] constexpr operator conv_type() const noexcept;

		[[rythe_always_inline]] constexpr column& operator=(const conv_type& other) noexcept;

		[[rythe_always_inline]] constexpr scalar& operator[](size_type i) noexcept
		{
			rsl_assert_out_of_range_msg((i >= 0) && (i < size), "vector subscript out of range");
			return mx[i][ColIdx];
		}

		[[rythe_always_inline]] constexpr const scalar& operator[](size_type i) const noexcept
		{
			rsl_assert_out_of_range_msg((i >= 0) && (i < size), "vector subscript out of range");
			return mx[i][ColIdx];
		}
	};

	template <arithmetic_type Scalar, size_type ColCount, size_type ColIdx, storage_mode Mode>
	struct column<Scalar, 1, ColCount, ColIdx, Mode>
	{
		using scalar = Scalar;
		static constexpr size_type size = 1;
		static constexpr storage_mode mode = Mode;
		static constexpr size_type row_count = 1;
		static constexpr size_type col_count = ColCount;
		static constexpr size_type col_idx = ColIdx;
		using conv_type = vector<scalar, size, Mode>;
		using storage_type = vector<scalar, col_count, Mode>;

		storage_type mx[row_count];

		COPY_FUNCS_DECO([[rythe_always_inline]] constexpr, column, noexcept);

		[[nodiscard]] [[rythe_always_inline]] constexpr operator scalar() const noexcept;

		[[nodiscard]] [[rythe_always_inline]] constexpr operator conv_type() const noexcept;

		[[rythe_always_inline]] constexpr column& operator=(const conv_type& other) noexcept;

		[[rythe_always_inline]] constexpr column& operator=(scalar value) noexcept;

		[[rythe_always_inline]] constexpr scalar& operator[](size_type i) noexcept
		{
			rsl_assert_out_of_range_msg((i >= 0) && (i < size), "vector subscript out of range");
			return mx[i][ColIdx];
		}

		[[rythe_always_inline]] constexpr const scalar& operator[](size_type i) const noexcept
		{
			rsl_assert_out_of_range_msg((i >= 0) && (i < size), "vector subscript out of range");
			return mx[i][ColIdx];
		}
	};

	template <size_type ColCount, size_type ColIdx, storage_mode Mode>
	struct column<bool, 1, ColCount, ColIdx, Mode>
	{
		using scalar = bool;
		static constexpr size_type size = 1;
		static constexpr storage_mode mode = Mode;
		static constexpr size_type row_count = 1;
		static constexpr size_type col_count = ColCount;
		static constexpr size_type col_idx = ColIdx;
		using conv_type = vector<scalar, size, Mode>;
		using storage_type = vector<scalar, col_count, Mode>;

		storage_type mx[row_count];

		COPY_FUNCS_DECO([[rythe_always_inline]] constexpr, column, noexcept);

		[[rythe_always_inline]] constexpr void set_mask(bitfield8 mask) noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr bitfield8 mask() const noexcept;

		[[nodiscard]] [[rythe_always_inline]] constexpr operator scalar() const noexcept;

		[[nodiscard]] [[rythe_always_inline]] constexpr operator conv_type() const noexcept;

		[[rythe_always_inline]] constexpr column& operator=(const conv_type& other) noexcept;

		[[rythe_always_inline]] constexpr column& operator=(scalar value) noexcept;

		[[rythe_always_inline]] constexpr scalar& operator[](size_type i) noexcept
		{
			rsl_assert_out_of_range_msg((i >= 0) && (i < size), "vector subscript out of range");
			return mx[i][ColIdx];
		}

		[[rythe_always_inline]] constexpr const scalar& operator[](size_type i) const noexcept
		{
			rsl_assert_out_of_range_msg((i >= 0) && (i < size), "vector subscript out of range");
			return mx[i][ColIdx];
		}
	};
} // namespace rsl::math

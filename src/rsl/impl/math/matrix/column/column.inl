#pragma once
#include "column.hpp"

namespace rsl::math
{
	template <arithmetic_type Scalar, size_type RowCount, size_type ColCount, size_type ColIdx, storage_mode Mode>
	constexpr column<Scalar, RowCount, ColCount, ColIdx, Mode>::operator conv_type() const noexcept
	{
		conv_type result;

		for (size_type i = 0; i < size; i++)
		{
			result[i] = mx[i][ColIdx];
		}

		return result;
	}

	template <arithmetic_type Scalar, size_type RowCount, size_type ColCount, size_type ColIdx, storage_mode Mode>
	constexpr column<Scalar, RowCount, ColCount, ColIdx, Mode>&
	column<Scalar, RowCount, ColCount, ColIdx, Mode>::operator=(const conv_type& other) noexcept
	{
		for (size_type i = 0; i < size; i++)
		{
			mx[i][ColIdx] = other[i];
		}

		return *this;
	}

	template <size_type RowCount, size_type ColCount, size_type ColIdx, storage_mode Mode>
	constexpr void column<bool, RowCount, ColCount, ColIdx, Mode>::set_mask(bitfield8 mask) noexcept
	{
		for (size_type i = 0; i < size; i++)
		{
			mx[i][ColIdx] = (mask & (1 << i)) != 0;
		}
	}

	template <size_type RowCount, size_type ColCount, size_type ColIdx, storage_mode Mode>
	constexpr bitfield8 column<bool, RowCount, ColCount, ColIdx, Mode>::mask() const noexcept
	{
		bitfield8 result = 0;

		for (size_type i = 0; i < size; i++)
		{
			result |= static_cast<bitfield8>(static_cast<uint32>(mx[i][ColIdx]) << i);
		}

		return result;
	}

	template <size_type RowCount, size_type ColCount, size_type ColIdx, storage_mode Mode>
	constexpr column<bool, RowCount, ColCount, ColIdx, Mode>::operator conv_type() const noexcept
	{
		conv_type result;

		for (size_type i = 0; i < size; i++)
		{
			result[i] = mx[i][ColIdx];
		}

		return result;
	}

	template <size_type RowCount, size_type ColCount, size_type ColIdx, storage_mode Mode>
	constexpr column<bool, RowCount, ColCount, ColIdx, Mode>&
	column<bool, RowCount, ColCount, ColIdx, Mode>::operator=(const conv_type& other) noexcept
	{
		for (size_type i = 0; i < size; i++)
		{
			mx[i][ColIdx] = other[i];
		}

		return *this;
	}

	template <arithmetic_type Scalar, size_type ColCount, size_type ColIdx, storage_mode Mode>
	inline constexpr column<Scalar, 1, ColCount, ColIdx, Mode>::operator scalar() const noexcept
	{
		return mx[0][ColIdx];
	}

	template <arithmetic_type Scalar, size_type ColCount, size_type ColIdx, storage_mode Mode>
	constexpr column<Scalar, 1, ColCount, ColIdx, Mode>::operator conv_type() const noexcept
	{
		return conv_type(mx[0][ColIdx]);
	}

	template <arithmetic_type Scalar, size_type ColCount, size_type ColIdx, storage_mode Mode>
	constexpr column<Scalar, 1, ColCount, ColIdx, Mode>&
	column<Scalar, 1, ColCount, ColIdx, Mode>::operator=(const conv_type& other) noexcept
	{
		mx[0][ColIdx] = other[0];
		return *this;
	}

	template <arithmetic_type Scalar, size_type ColCount, size_type ColIdx, storage_mode Mode>
	constexpr column<Scalar, 1, ColCount, ColIdx, Mode>&
	column<Scalar, 1, ColCount, ColIdx, Mode>::operator=(scalar value) noexcept
	{
		mx[0][ColIdx] = value;
		return *this;
	}

	template <size_type ColCount, size_type ColIdx, storage_mode Mode>
	constexpr void column<bool, 1, ColCount, ColIdx, Mode>::set_mask(bitfield8 mask) noexcept
	{
		mx[0][ColIdx] = (mask & 1) != 0;
	}

	template <size_type ColCount, size_type ColIdx, storage_mode Mode>
	constexpr bitfield8 column<bool, 1, ColCount, ColIdx, Mode>::mask() const noexcept
	{
		return static_cast<bitfield8>(mx[0][ColIdx]);
	}

	template <size_type ColCount, size_type ColIdx, storage_mode Mode>
	inline constexpr column<bool, 1, ColCount, ColIdx, Mode>::operator scalar() const noexcept
	{
		return mx[0][ColIdx];
	}

	template <size_type ColCount, size_type ColIdx, storage_mode Mode>
	constexpr column<bool, 1, ColCount, ColIdx, Mode>::operator conv_type() const noexcept
	{
		return conv_type(mx[0][ColIdx]);
	}

	template <size_type ColCount, size_type ColIdx, storage_mode Mode>
	constexpr column<bool, 1, ColCount, ColIdx, Mode>&
	column<bool, 1, ColCount, ColIdx, Mode>::operator=(const conv_type& other) noexcept
	{
		mx[0][ColIdx] = other[0];
		return *this;
	}

	template <size_type ColCount, size_type ColIdx, storage_mode Mode>
	constexpr column<bool, 1, ColCount, ColIdx, Mode>& column<bool, 1, ColCount, ColIdx, Mode>::operator=(scalar value
	) noexcept
	{
		mx[0][ColIdx] = value;
		return *this;
	}
} // namespace rsl::math

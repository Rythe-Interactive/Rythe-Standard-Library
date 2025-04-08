#pragma once
#include "../../../defines.hpp"
#include "../predefined.hpp"

namespace rsl::math
{
	template <arithmetic_type Scalar, size_type Size, storage_mode Mode, size_type... args>
	struct swizzle;

	template <arithmetic_type Scalar, size_type Size, storage_mode Mode, size_type s0>
	struct swizzle<Scalar, Size, Mode, s0>
	{
		using scalar = Scalar;
		static constexpr size_type size = 1;
		static constexpr storage_mode mode = Mode;
		using conv_type = vector<scalar, 1, Mode>;
		using storage_type = storage_t<scalar, Size, Mode>;

		storage_type data;

		[[rythe_always_inline]] constexpr swizzle() noexcept = default;
		COPY_FUNCS_DECO([[rythe_always_inline]] constexpr, swizzle, noexcept);

		[[rythe_always_inline]] constexpr swizzle(scalar x) noexcept;

		[[rythe_always_inline]] constexpr operator scalar() const noexcept;

		[[rythe_always_inline]] constexpr operator conv_type() const noexcept;

		[[rythe_always_inline]] constexpr swizzle& operator=(const conv_type& other) noexcept;

		[[rythe_always_inline]] constexpr swizzle& operator=(scalar value) noexcept;

		[[rythe_always_inline]] constexpr scalar& operator[](size_type i) noexcept
		{
			rsl_assert_out_of_range_msg((i >= 0) && (i < size), "vector subscript out of range");
			return data[s0];
		}

		[[rythe_always_inline]] constexpr const scalar& operator[](size_type i) const noexcept
		{
			rsl_assert_out_of_range_msg((i >= 0) && (i < size), "vector subscript out of range");
			return data[s0];
		}
	};

	template <size_type Size, storage_mode Mode, size_type s0>
	struct swizzle<bool, Size, Mode, s0>
	{
		using scalar = bool;
		static constexpr size_type size = 1;
		static constexpr storage_mode mode = Mode;
		using conv_type = vector<scalar, 1, Mode>;
		using storage_type = storage_t<scalar, Size, Mode>;

		storage_type data;

		[[rythe_always_inline]] constexpr swizzle() noexcept = default;
		COPY_FUNCS_DECO([[rythe_always_inline]] constexpr, swizzle, noexcept);

		[[rythe_always_inline]] constexpr swizzle(scalar x) noexcept;

		[[rythe_always_inline]] constexpr void set_mask(bitfield8 mask) noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr bitfield8 mask() const noexcept;

		[[rythe_always_inline]] constexpr operator scalar() const noexcept;

		[[rythe_always_inline]] constexpr operator conv_type() const noexcept;

		[[rythe_always_inline]] constexpr swizzle& operator=(const conv_type& other) noexcept;

		[[rythe_always_inline]] constexpr swizzle& operator=(scalar value) noexcept;

		[[rythe_always_inline]] constexpr scalar& operator[](size_type i) noexcept
		{
			rsl_assert_out_of_range_msg((i >= 0) && (i < size), "vector subscript out of range");
			return data[s0];
		}

		[[rythe_always_inline]] constexpr const scalar& operator[](size_type i) const noexcept
		{
			rsl_assert_out_of_range_msg((i >= 0) && (i < size), "vector subscript out of range");
			return data[s0];
		}
	};

	template <arithmetic_type Scalar, size_type Size, storage_mode Mode, size_type s0, size_type s1>
	struct swizzle<Scalar, Size, Mode, s0, s1>
	{
	private:
		constexpr static size_type lookup[] = {s0, s1};

	public:
		using scalar = Scalar;
		static constexpr size_type size = 2;
		static constexpr storage_mode mode = Mode;
		using conv_type = vector<scalar, 2, Mode>;
		using storage_type = storage_t<scalar, Size, Mode>;

		storage_type data;

		[[rythe_always_inline]] constexpr swizzle() noexcept = default;
		COPY_FUNCS_DECO([[rythe_always_inline]] constexpr, swizzle, noexcept);

		[[rythe_always_inline]] constexpr swizzle(scalar x, scalar y) noexcept;

		[[rythe_always_inline]] constexpr operator conv_type() const noexcept;

		[[rythe_always_inline]] constexpr swizzle& operator=(const conv_type& other) noexcept;

		[[rythe_always_inline]] constexpr scalar& operator[](size_type i) noexcept
		{
			rsl_assert_out_of_range_msg((i >= 0) && (i < size), "vector subscript out of range");
			return data[lookup[i]];
		}

		[[rythe_always_inline]] constexpr const scalar& operator[](size_type i) const noexcept
		{
			rsl_assert_out_of_range_msg((i >= 0) && (i < size), "vector subscript out of range");
			return data[lookup[i]];
		}
	};

	template <size_type Size, storage_mode Mode, size_type s0, size_type s1>
	struct swizzle<bool, Size, Mode, s0, s1>
	{
	private:
		constexpr static size_type lookup[] = {s0, s1};

	public:
		using scalar = bool;
		static constexpr size_type size = 2;
		static constexpr storage_mode mode = Mode;
		using conv_type = vector<scalar, 2, Mode>;
		using storage_type = storage_t<scalar, Size, Mode>;

		storage_type data;

		[[rythe_always_inline]] constexpr swizzle() noexcept = default;
		COPY_FUNCS_DECO([[rythe_always_inline]] constexpr, swizzle, noexcept);

		[[rythe_always_inline]] constexpr swizzle(scalar x, scalar y) noexcept;

		[[rythe_always_inline]] constexpr void set_mask(bitfield8 mask) noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr bitfield8 mask() const noexcept;

		[[rythe_always_inline]] constexpr operator conv_type() const noexcept;

		[[rythe_always_inline]] constexpr swizzle& operator=(const conv_type& other) noexcept;

		[[rythe_always_inline]] constexpr scalar& operator[](size_type i) noexcept
		{
			rsl_assert_out_of_range_msg((i >= 0) && (i < size), "vector subscript out of range");
			return data[lookup[i]];
		}

		[[rythe_always_inline]] constexpr const scalar& operator[](size_type i) const noexcept
		{
			rsl_assert_out_of_range_msg((i >= 0) && (i < size), "vector subscript out of range");
			return data[lookup[i]];
		}
	};

	template <arithmetic_type Scalar, size_type Size, storage_mode Mode, size_type s0, size_type s1, size_type s2>
	struct swizzle<Scalar, Size, Mode, s0, s1, s2>
	{
	private:
		constexpr static size_type lookup[] = {s0, s1, s2};

	public:
		using scalar = Scalar;
		static constexpr size_type size = 3;
		static constexpr storage_mode mode = Mode;
		using conv_type = vector<scalar, 3, Mode>;
		using storage_type = storage_t<scalar, Size, Mode>;

		storage_type data;

		[[rythe_always_inline]] constexpr swizzle() noexcept = default;
		COPY_FUNCS_DECO([[rythe_always_inline]] constexpr, swizzle, noexcept);

		[[rythe_always_inline]] constexpr swizzle(scalar x, scalar y, scalar z) noexcept;

		[[rythe_always_inline]] constexpr operator conv_type() const noexcept;

		[[rythe_always_inline]] constexpr swizzle& operator=(const conv_type& other) noexcept;

		[[rythe_always_inline]] constexpr scalar& operator[](size_type i) noexcept
		{
			rsl_assert_out_of_range_msg((i >= 0) && (i < size), "vector subscript out of range");
			return data[lookup[i]];
		}

		[[rythe_always_inline]] constexpr const scalar& operator[](size_type i) const noexcept
		{
			rsl_assert_out_of_range_msg((i >= 0) && (i < size), "vector subscript out of range");
			return data[lookup[i]];
		}
	};

	template <size_type Size, storage_mode Mode, size_type s0, size_type s1, size_type s2>
	struct swizzle<bool, Size, Mode, s0, s1, s2>
	{
	private:
		constexpr static size_type lookup[] = {s0, s1, s2};

	public:
		using scalar = bool;
		static constexpr size_type size = 3;
		static constexpr storage_mode mode = Mode;
		using conv_type = vector<scalar, 3, Mode>;
		using storage_type = storage_t<scalar, Size, Mode>;

		storage_type data;

		[[rythe_always_inline]] constexpr swizzle() noexcept = default;
		COPY_FUNCS_DECO([[rythe_always_inline]] constexpr, swizzle, noexcept);

		[[rythe_always_inline]] constexpr swizzle(bool x, bool y, bool z) noexcept;

		[[rythe_always_inline]] constexpr void set_mask(bitfield8 mask) noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr bitfield8 mask() const noexcept;

		[[rythe_always_inline]] constexpr operator conv_type() const noexcept;

		[[rythe_always_inline]] constexpr swizzle& operator=(const conv_type& other) noexcept;

		[[rythe_always_inline]] constexpr scalar& operator[](size_type i) noexcept
		{
			rsl_assert_out_of_range_msg((i >= 0) && (i < size), "vector subscript out of range");
			return data[lookup[i]];
		}

		[[rythe_always_inline]] constexpr const scalar& operator[](size_type i) const noexcept
		{
			rsl_assert_out_of_range_msg((i >= 0) && (i < size), "vector subscript out of range");
			return data[lookup[i]];
		}
	};

	template <arithmetic_type Scalar, size_type Size, storage_mode Mode, size_type s0, size_type s1, size_type s2, size_type s3>
	struct swizzle<Scalar, Size, Mode, s0, s1, s2, s3>
	{
	private:
		constexpr static size_type lookup[] = {s0, s1, s2, s3};

	public:
		using scalar = Scalar;
		static constexpr size_type size = 4;
		static constexpr storage_mode mode = Mode;
		using conv_type = vector<scalar, 4, Mode>;
		using storage_type = storage_t<scalar, Size, Mode>;

		storage_type data;

		[[rythe_always_inline]] constexpr swizzle() noexcept = default;
		COPY_FUNCS_DECO([[rythe_always_inline]] constexpr, swizzle, noexcept);

		[[rythe_always_inline]] constexpr swizzle(scalar x, scalar y, scalar z, scalar w) noexcept;

		[[rythe_always_inline]] constexpr operator conv_type() const noexcept;

		[[rythe_always_inline]] constexpr swizzle& operator=(const conv_type& other) noexcept;

		[[rythe_always_inline]] constexpr scalar& operator[](size_type i) noexcept
		{
			rsl_assert_out_of_range_msg((i >= 0) && (i < size), "vector subscript out of range");
			return data[lookup[i]];
		}

		[[rythe_always_inline]] constexpr const scalar& operator[](size_type i) const noexcept
		{
			rsl_assert_out_of_range_msg((i >= 0) && (i < size), "vector subscript out of range");
			return data[lookup[i]];
		}
	};

	template <size_type Size, storage_mode Mode, size_type s0, size_type s1, size_type s2, size_type s3>
	struct swizzle<bool, Size, Mode, s0, s1, s2, s3>
	{
	private:
		constexpr static size_type lookup[] = {s0, s1, s2, s3};

	public:
		using scalar = bool;
		static constexpr size_type size = 4;
		static constexpr storage_mode mode = Mode;
		using conv_type = vector<scalar, 4, Mode>;
		using storage_type = storage_t<scalar, Size, Mode>;

		storage_type data;

		[[rythe_always_inline]] constexpr swizzle() noexcept = default;
		COPY_FUNCS_DECO([[rythe_always_inline]] constexpr, swizzle, noexcept);

		[[rythe_always_inline]] constexpr swizzle(scalar x, scalar y, scalar z, scalar w) noexcept;

		[[rythe_always_inline]] constexpr void set_mask(bitfield8 mask) noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr bitfield8 mask() const noexcept;

		[[rythe_always_inline]] constexpr operator conv_type() const noexcept;

		[[rythe_always_inline]] constexpr swizzle& operator=(const conv_type& other) noexcept;

		[[rythe_always_inline]] constexpr scalar& operator[](size_type i) noexcept
		{
			rsl_assert_out_of_range_msg((i >= 0) && (i < size), "vector subscript out of range");
			return data[lookup[i]];
		}

		[[rythe_always_inline]] constexpr const scalar& operator[](size_type i) const noexcept
		{
			rsl_assert_out_of_range_msg((i >= 0) && (i < size), "vector subscript out of range");
			return data[lookup[i]];
		}
	};
} // namespace rsl::math

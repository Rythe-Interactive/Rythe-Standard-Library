#pragma once
#include "swizzle/swizzle1.hpp"
#include "vector_base.hpp"

RYTHE_MSVC_SUPPRESS_WARNING_WITH_PUSH(4201) // anonymous struct

namespace rsl::math
{
	template <arithmetic_type Scalar, mode Mode>
	struct vector<Scalar, 1, Mode>
	{
		using scalar = Scalar;
		static constexpr size_type size = 1;
		static constexpr mode mode = Mode;
		using type = vector<Scalar, 1>;
		using storage_type = storage_t<Scalar, size, Mode>;

		union
		{
			storage_type data;

			_MATH_SWIZZLE_1_1_(scalar, Mode);
			_MATH_SWIZZLE_1_2_(scalar, Mode);
			_MATH_SWIZZLE_1_3_(scalar, Mode);
			_MATH_SWIZZLE_1_4_(scalar, Mode);
		};

		[[rythe_always_inline]] constexpr vector() noexcept;
		[[rythe_always_inline]] constexpr vector(const vector&) noexcept = default;
		[[rythe_always_inline]] constexpr vector(scalar s) noexcept;

		template <typename vec_type>
			requires not_same_as<Scalar, typename vec_type::scalar> || (vec_type::size != 1)
		[[rythe_always_inline]] constexpr vector(const vec_type& other) noexcept;

		static const vector one;
		static const vector zero;

		[[rythe_always_inline]] constexpr vector& operator=(const vector&) noexcept = default;
		[[nodiscard]] [[rythe_always_inline]] constexpr operator scalar() const noexcept { return x; }

		[[nodiscard]] [[rythe_always_inline]] constexpr scalar& operator[](size_type i) noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const scalar& operator[](size_type i) const noexcept;
	};

	template <mode Mode>
	struct vector<bool, 1, Mode>
	{
		using scalar = bool;
		static constexpr size_type size = 1;
		static constexpr mode mode = Mode;
		using type = vector<bool, 1>;
		using storage_type = storage_t<scalar, size, Mode>;

		union
		{
			storage_type data;

			_MATH_SWIZZLE_1_1_(scalar, Mode);
			_MATH_SWIZZLE_1_2_(scalar, Mode);
			_MATH_SWIZZLE_1_3_(scalar, Mode);
			_MATH_SWIZZLE_1_4_(scalar, Mode);
		};

		[[rythe_always_inline]] constexpr vector() noexcept
			: x(static_cast<scalar>(0))
		{
		}
		[[rythe_always_inline]] constexpr vector(const vector&) noexcept = default;
		[[rythe_always_inline]] constexpr vector(scalar s) noexcept
			: x(static_cast<scalar>(s))
		{
		}

		static const vector one;
		static const vector zero;

		[[rythe_always_inline]] constexpr void set_mask(bitfield8 mask) noexcept { x = mask & 1; }
		[[nodiscard]] [[rythe_always_inline]] constexpr bitfield8 mask() const noexcept { return static_cast<bitfield8>(x); }

		[[rythe_always_inline]] constexpr vector& operator=(const vector&) noexcept = default;
		[[nodiscard]] [[rythe_always_inline]] constexpr operator bool() const noexcept { return x; }

		[[nodiscard]] [[rythe_always_inline]] constexpr scalar& operator[](size_type i) noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const scalar& operator[](size_type i) const noexcept;
	};

	using float1 = vector<float32, 1>;
	using vec1 = float1;
	using double1 = vector<float64, 1>;
	using dvec1 = double1;
	using int1 = vector<int, 1>;
	using ivec1 = int1;
	using uint1 = vector<uint, 1>;
	using uvec1 = uint1;
	using bool1 = vector<bool, 1>;
	using bvec1 = bool1;

#ifdef RYTHE_PCH
	template struct vector<float32, 1>;
	template struct vector<float64, 1>;
	template struct vector<int, 1>;
	template struct vector<uint, 1>;
	template struct vector<bool, 1>;
#endif

} // namespace rsl::math

RYTHE_MSVC_SUPPRESS_WARNING_POP

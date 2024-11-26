#pragma once
#include "../util/type_util.hpp"
#include "swizzle/swizzle2.hpp"
#include "vector_base.hpp"
#include "vector_base.inl"

RYTHE_MSVC_SUPPRESS_WARNING_WITH_PUSH(4201) // anonymous struct

namespace rsl::math
{
	template <arithmetic_type Scalar, mode Mode>
		requires signed_type<Scalar>
	struct vector<Scalar, 2, Mode>
	{
		using scalar = Scalar;
		static constexpr size_type size = 2;
		static constexpr mode mode = Mode;
		using type = vector<Scalar, 2>;
		using storage_type = storage_t<Scalar, size, Mode>;

		union
		{
			storage_type data;

			_MATH_SWIZZLE_2_1_(scalar, Mode);
			_MATH_SWIZZLE_2_2_(scalar, Mode);
			_MATH_SWIZZLE_2_3_(scalar, Mode);
			_MATH_SWIZZLE_2_4_(scalar, Mode);
		};

		[[rythe_always_inline]] constexpr vector() noexcept;
		[[rythe_always_inline]] constexpr vector(const vector&) noexcept = default;
		[[rythe_always_inline]] explicit constexpr vector(scalar s) noexcept;
		[[rythe_always_inline]] constexpr vector(scalar _x, scalar _y) noexcept;

		template <typename vec_type>
			requires not_same_as<Scalar, typename vec_type::scalar> || (vec_type::size != 2)
		[[rythe_always_inline]] constexpr vector(const vec_type& other) noexcept;

		static const vector up;
		static const vector down;
		static const vector right;
		static const vector left;
		static const vector one;
		static const vector zero;

		[[rythe_always_inline]] constexpr vector& operator=(const vector&) noexcept = default;

		[[nodiscard]] [[rythe_always_inline]] constexpr scalar& operator[](size_type i) noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const scalar& operator[](size_type i) const noexcept;
	};
	template <arithmetic_type Scalar, mode Mode>
		requires unsigned_type<Scalar>
	struct vector<Scalar, 2, Mode>
	{
		using scalar = Scalar;
		static constexpr size_type size = 2;
		static constexpr mode mode = Mode;
		using type = vector<Scalar, 2>;
		using storage_type = storage_t<Scalar, size, Mode>;

		union
		{
			storage_type data;

			_MATH_SWIZZLE_2_1_(scalar, Mode);
			_MATH_SWIZZLE_2_2_(scalar, Mode);
			_MATH_SWIZZLE_2_3_(scalar, Mode);
			_MATH_SWIZZLE_2_4_(scalar, Mode);
		};

		[[rythe_always_inline]] constexpr vector() noexcept;
		[[rythe_always_inline]] constexpr vector(const vector&) noexcept = default;
		[[rythe_always_inline]] explicit constexpr vector(scalar s) noexcept;
		[[rythe_always_inline]] constexpr vector(scalar _x, scalar _y) noexcept;

		template <typename vec_type>
			requires not_same_as<Scalar, typename vec_type::scalar> || (vec_type::size != 2)
		[[rythe_always_inline]] constexpr vector(const vec_type& other) noexcept;

		static const vector up;
		static const vector right;
		static const vector one;
		static const vector zero;

		[[rythe_always_inline]] constexpr vector& operator=(const vector&) noexcept = default;

		[[nodiscard]] [[rythe_always_inline]] constexpr scalar& operator[](size_type i) noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const scalar& operator[](size_type i) const noexcept;
	};

	template <mode Mode>
	struct vector<bool, 2, Mode>
	{
		using scalar = bool;
		static constexpr size_type size = 2;
		static constexpr mode mode = Mode;
		using type = vector<bool, 2>;
		using storage_type = storage_t<scalar, size, Mode>;

		union
		{
			storage_type data;

			_MATH_SWIZZLE_2_1_(scalar, Mode);
			_MATH_SWIZZLE_2_2_(scalar, Mode);
			_MATH_SWIZZLE_2_3_(scalar, Mode);
			_MATH_SWIZZLE_2_4_(scalar, Mode);
		};

		[[rythe_always_inline]] constexpr vector() noexcept;
		[[rythe_always_inline]] constexpr vector(const vector&) noexcept = default;
		[[rythe_always_inline]] explicit constexpr vector(scalar s) noexcept;
		[[rythe_always_inline]] constexpr vector(scalar _x, scalar _y) noexcept;

		template <typename vec_type>
			requires not_same_as<bool, typename vec_type::scalar> || (vec_type::size != 2)
		[[rythe_always_inline]] constexpr vector(const vec_type& other) noexcept;

		static const vector up;
		static const vector right;
		static const vector one;
		static const vector zero;

		[[rythe_always_inline]] constexpr void set_mask(bitfield8 mask) noexcept
		{
			x = mask & 1;
			y = mask & 2;
		}

		[[nodiscard]] [[rythe_always_inline]] constexpr bitfield8 mask() const noexcept
		{
			return static_cast<bitfield8>(static_cast<uint32>(x) | static_cast<uint32>(y) << 1u);
		}

		[[rythe_always_inline]] constexpr operator bool() const noexcept { return x && y; }

		[[rythe_always_inline]] constexpr vector& operator=(const vector&) noexcept = default;

		[[rythe_always_inline]] constexpr scalar& operator[](size_type i) noexcept;
		[[rythe_always_inline]] constexpr const scalar& operator[](size_type i) const noexcept;
	};

	using float2 = vector<float32, 2>;
	using vec2 = float2;
	using double2 = vector<float64, 2>;
	using dvec2 = double2;
	using int2 = vector<int, 2>;
	using ivec2 = int2;
	using uint2 = vector<uint, 2>;
	using uvec2 = uint2;
	using bool2 = vector<bool, 2>;
	using bvec2 = bool2;

#ifdef RYTHE_PCH
	template struct vector<float32, 2>;
	template struct vector<float64, 2>;
	template struct vector<int, 2>;
	template struct vector<uint, 2>;
	template struct vector<bool, 2>;
#endif // RYTHE_PCH


} // namespace rsl::math

RYTHE_MSVC_SUPPRESS_WARNING_POP

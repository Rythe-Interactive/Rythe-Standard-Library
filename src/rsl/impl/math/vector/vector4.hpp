#pragma once
#include <immintrin.h>

#include "../util/type_util.hpp"
#include "swizzle/swizzle4.hpp"
#include "vector_base.hpp"
#include "vector_base.inl"

namespace rsl::math
{
	template <typename Scalar>
	struct vector<Scalar, 4> : vector_base
	{
		using scalar = Scalar;
		static constexpr size_type size = 4;
		using type = vector<Scalar, 4>;

		union
		{
			scalar data[4];
			__m128 intrin[(sizeof(scalar) * size) / sizeof(__m128)];

			_MATH_SWIZZLE_4_1_(scalar);
			_MATH_SWIZZLE_4_2_(scalar);
			_MATH_SWIZZLE_4_3_(scalar);
			_MATH_SWIZZLE_4_4_(scalar);
		};

		constexpr vector() noexcept
			: vector(static_cast<scalar>(0), static_cast<scalar>(0), static_cast<scalar>(0), static_cast<scalar>(0))
		{
		}

		constexpr vector(const vector&) noexcept = default;

		explicit constexpr vector(scalar s) noexcept
			: vector(static_cast<scalar>(s), static_cast<scalar>(s), static_cast<scalar>(s), static_cast<scalar>(s))
		{
		}

		explicit constexpr vector(const vector<scalar, 3>& v, scalar s = static_cast<scalar>(0)) noexcept;
		constexpr vector(scalar s, const vector<scalar, 3>& v) noexcept;
		constexpr vector(scalar s0, const vector<scalar, 2>& v, scalar s1 = static_cast<scalar>(0)) noexcept;
		constexpr vector(scalar s0, scalar s1, const vector<scalar, 2>& v) noexcept;
		explicit constexpr vector(const vector<scalar, 2>& v, scalar s0 = static_cast<scalar>(0), scalar s1 = static_cast<scalar>(0)) noexcept;
		constexpr vector(const vector<scalar, 2>& v0, const vector<scalar, 2>& v1) noexcept;
		constexpr vector(scalar _x, scalar _y, scalar _z = static_cast<scalar>(0), scalar _w = static_cast<scalar>(0)) noexcept;

		template <typename vec_type, ::std::enable_if_t<is_vector_v<vec_type> && (vec_type::size != 4 || !std::is_same_v<Scalar, typename vec_type::scalar>), bool> = true>
		constexpr vector(const vec_type& other) noexcept;

		static const vector up;
		static const vector down;
		static const vector right;
		static const vector left;
		static const vector forward;
		static const vector backward;
		static const vector positiveW;
		static const vector negativeW;
		static const vector one;
		static const vector zero;

		constexpr vector& operator=(const vector&) noexcept = default;

		constexpr scalar& operator[](size_type i) noexcept
		{
			rsl_assert_msg((i >= 0) && (i < size), "vector subscript out of range");
			return data[i];
		}
		constexpr const scalar& operator[](size_type i) const noexcept
		{
			rsl_assert_msg((i >= 0) && (i < size), "vector subscript out of range");
			return data[i];
		}

		rythe_always_inline scalar length() const noexcept;
		constexpr scalar length2() const noexcept;
	};

	template <>
	struct vector<bool, 4> : vector_base
	{
		using scalar = bool;
		static constexpr size_type size = 4;
		using type = vector<bool, 4>;

		union
		{
			scalar data[4];

			_MATH_SWIZZLE_4_1_(scalar);
			_MATH_SWIZZLE_4_2_(scalar);
			_MATH_SWIZZLE_4_3_(scalar);
			_MATH_SWIZZLE_4_4_(scalar);
		};

		constexpr vector() noexcept
			: vector(static_cast<scalar>(0), static_cast<scalar>(0), static_cast<scalar>(0), static_cast<scalar>(0))
		{
		}

		constexpr vector(const vector&) noexcept = default;

		explicit constexpr vector(scalar s) noexcept
			: vector(static_cast<scalar>(s), static_cast<scalar>(s), static_cast<scalar>(s), static_cast<scalar>(s))
		{
		}


		explicit constexpr vector(const vector<scalar, 3>& v, scalar s = static_cast<scalar>(0)) noexcept;
		constexpr vector(scalar s, const vector<scalar, 3>& v) noexcept;
		constexpr vector(scalar s0, const vector<scalar, 2>& v, scalar s1 = static_cast<scalar>(0)) noexcept;
		constexpr vector(scalar s0, scalar s1, const vector<scalar, 2>& v) noexcept;
		explicit constexpr vector(const vector<scalar, 2>& v, scalar s0 = static_cast<scalar>(0), scalar s1 = static_cast<scalar>(0)) noexcept;
		constexpr vector(const vector<scalar, 2>& v0, const vector<scalar, 2>& v1) noexcept;
		constexpr vector(scalar _x, scalar _y, scalar _z = static_cast<scalar>(0), scalar _w = static_cast<scalar>(0)) noexcept
			: x(_x),
			  y(_y),
			  z(_z),
			  w(_w)
		{
		}

		template <typename vec_type, ::std::enable_if_t<is_vector_v<vec_type> && (vec_type::size != 4 || !std::is_same_v<bool, typename vec_type::scalar>), bool> = true>
		constexpr vector(const vec_type& other) noexcept;

		static const vector up;
		static const vector down;
		static const vector right;
		static const vector left;
		static const vector forward;
		static const vector backward;
		static const vector positiveW;
		static const vector negativeW;
		static const vector one;
		static const vector zero;

		constexpr void set_mask(bitfield8 mask) noexcept
		{
			x = mask & 1;
			y = mask & 2;
			z = mask & 4;
			w = mask & 8;
		}
		constexpr bitfield8 mask() const noexcept { return static_cast<bitfield8>(x | y << 1u | z << 2u | w << 3u); }

		constexpr operator bool() const noexcept { return x && y && z && w; }

		constexpr vector& operator=(const vector&) noexcept = default;

		constexpr scalar& operator[](size_type i) noexcept
		{
			rsl_assert_msg((i >= 0) && (i < size), "vector subscript out of range");
			return data[i];
		}
		constexpr const scalar& operator[](size_type i) const noexcept
		{
			rsl_assert_msg((i >= 0) && (i < size), "vector subscript out of range");
			return data[i];
		}

		constexpr scalar length() const noexcept { return (x && y && z && w) ? 2 : (x || y || z || w); };
		constexpr scalar length2() const noexcept { return this->length() * this->length(); }
	};

	template <typename Scalar>
	const vector<Scalar, 4> vector<Scalar, 4>::up(static_cast<Scalar>(0), static_cast<Scalar>(1), static_cast<Scalar>(0), static_cast<scalar>(0));
	template <typename Scalar>
	const vector<Scalar, 4> vector<Scalar, 4>::down(static_cast<Scalar>(0), static_cast<Scalar>(-1), static_cast<Scalar>(0), static_cast<scalar>(0));
	template <typename Scalar>
	const vector<Scalar, 4> vector<Scalar, 4>::right(static_cast<Scalar>(1), static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<scalar>(0));
	template <typename Scalar>
	const vector<Scalar, 4> vector<Scalar, 4>::left(static_cast<Scalar>(-1), static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<scalar>(0));
	template <typename Scalar>
	const vector<Scalar, 4> vector<Scalar, 4>::forward(static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(1), static_cast<scalar>(0));
	template <typename Scalar>
	const vector<Scalar, 4> vector<Scalar, 4>::backward(static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(-1), static_cast<scalar>(0));
	template <typename Scalar>
	const vector<Scalar, 4> vector<Scalar, 4>::positiveW(static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<scalar>(0), static_cast<Scalar>(1));
	template <typename Scalar>
	const vector<Scalar, 4> vector<Scalar, 4>::negativeW(static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<scalar>(0), static_cast<Scalar>(-1));
	template <typename Scalar>
	const vector<Scalar, 4> vector<Scalar, 4>::one(static_cast<Scalar>(1));
	template <typename Scalar>
	const vector<Scalar, 4> vector<Scalar, 4>::zero(static_cast<Scalar>(0));

	using float4 = vector<float32, 4>;
	using vec4 = float4;
	using double4 = vector<float64, 4>;
	using dvec4 = double4;
	using int4 = vector<int, 4>;
	using ivec4 = int4;
	using uint4 = vector<uint, 4>;
	using uvec4 = uint4;
	using bool4 = vector<bool, 4>;
	using bvec4 = bool4;
} // namespace rsl::math

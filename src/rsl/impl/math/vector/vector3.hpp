#pragma once
#include "../util/type_util.hpp"
#include "swizzle/swizzle3.hpp"
#include "vector_base.hpp"
#include "vector_base.inl"

namespace rsl::math
{
	template <typename Scalar>
	struct vector<Scalar, 3> : vector_base
	{
		using scalar = Scalar;
		static constexpr size_type size = 3;
		using type = vector<Scalar, 3>;

		union
		{
			scalar data[3];

			_MATH_SWIZZLE_3_1_(scalar);
			_MATH_SWIZZLE_3_2_(scalar);
			_MATH_SWIZZLE_3_3_(scalar);
			_MATH_SWIZZLE_3_4_(scalar);
		};

		constexpr vector() noexcept
			: vector(static_cast<scalar>(0), static_cast<scalar>(0), static_cast<scalar>(0))
		{
		}

		constexpr vector(const vector&) noexcept = default;

		explicit constexpr vector(scalar s) noexcept
			: vector(s, s, s)
		{
		}

		explicit constexpr vector(const vector<scalar, 2>& v, scalar s = static_cast<scalar>(0)) noexcept;
		constexpr vector(scalar s, const vector<scalar, 2>& v) noexcept;
		constexpr vector(scalar _x, scalar _y, scalar _z = static_cast<scalar>(0)) noexcept;

		template <typename vec_type, ::std::enable_if_t<is_vector_v<vec_type> && (vec_type::size != 3 || !std::is_same_v<Scalar, typename vec_type::scalar>), bool> = true>
		constexpr vector(const vec_type& other) noexcept;

		static const vector up;
		static const vector down;
		static const vector right;
		static const vector left;
		static const vector forward;
		static const vector backward;
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
	struct vector<bool, 3> : vector_base
	{
		using scalar = bool;
		static constexpr size_type size = 3;
		using type = vector<bool, 3>;

		union
		{
			scalar data[3];

			_MATH_SWIZZLE_3_1_(scalar);
			_MATH_SWIZZLE_3_2_(scalar);
			_MATH_SWIZZLE_3_3_(scalar);
			_MATH_SWIZZLE_3_4_(scalar);
		};

		constexpr vector() noexcept
			: vector(static_cast<scalar>(0), static_cast<scalar>(0), static_cast<scalar>(0))
		{
		}

		constexpr vector(const vector&) noexcept = default;

		explicit constexpr vector(scalar s) noexcept
			: vector(static_cast<scalar>(s), static_cast<scalar>(s), static_cast<scalar>(s))
		{
		}

		explicit constexpr vector(const vector<scalar, 2>& v, scalar s = static_cast<scalar>(0)) noexcept;
		constexpr vector(scalar s, const vector<scalar, 2>& v) noexcept;
		constexpr vector(scalar _x, scalar _y, scalar _z = static_cast<scalar>(0)) noexcept
			: x(_x),
			  y(_y),
			  z(_z)
		{
		}

		template <typename _Scal, ::std::enable_if_t<!::std::is_same_v<scalar, _Scal>, bool> = true>
		constexpr vector(const vector<_Scal, size>& other) noexcept
			: vector(static_cast<scalar>(other.x), static_cast<scalar>(other.y), static_cast<scalar>(other.z))
		{
		}

		template <typename vec_type, ::std::enable_if_t<is_vector_v<vec_type> && (size != vec_type::size), bool> = true>
		constexpr vector(const vec_type& other) noexcept;

		static const vector up;
		static const vector down;
		static const vector right;
		static const vector left;
		static const vector forward;
		static const vector backward;
		static const vector one;
		static const vector zero;

		constexpr void set_mask(bitfield8 mask) noexcept
		{
			x = mask & 1;
			y = mask & 2;
			z = mask & 4;
		}
		constexpr bitfield8 mask() const noexcept { return static_cast<bitfield8>(x | y << 1u | z << 2u); }

		constexpr operator bool() const noexcept { return x && y && z; }

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

		constexpr scalar length() const noexcept { return x || y || z; }
		constexpr scalar length2() const noexcept { return this->length(); }
	};

	template <typename Scalar>
	const vector<Scalar, 3> vector<Scalar, 3>::up(static_cast<Scalar>(0), static_cast<Scalar>(1), static_cast<Scalar>(0));
	template <typename Scalar>
	const vector<Scalar, 3> vector<Scalar, 3>::down(static_cast<Scalar>(0), static_cast<Scalar>(-1), static_cast<Scalar>(0));
	template <typename Scalar>
	const vector<Scalar, 3> vector<Scalar, 3>::right(static_cast<Scalar>(1), static_cast<Scalar>(0), static_cast<Scalar>(0));
	template <typename Scalar>
	const vector<Scalar, 3> vector<Scalar, 3>::left(static_cast<Scalar>(-1), static_cast<Scalar>(0), static_cast<Scalar>(0));
	template <typename Scalar>
	const vector<Scalar, 3> vector<Scalar, 3>::forward(static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(1));
	template <typename Scalar>
	const vector<Scalar, 3> vector<Scalar, 3>::backward(static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(-1));
	template <typename Scalar>
	const vector<Scalar, 3> vector<Scalar, 3>::one(static_cast<Scalar>(1));
	template <typename Scalar>
	const vector<Scalar, 3> vector<Scalar, 3>::zero(static_cast<Scalar>(0));

	using float3 = vector<float32, 3>;
	using vec3 = float3;
	using double3 = vector<float64, 3>;
	using dvec3 = double3;
	using int3 = vector<int, 3>;
	using ivec3 = int3;
	using uint3 = vector<uint, 3>;
	using uvec3 = uint3;
	using bool3 = vector<bool, 3>;
	using bvec3 = bool3;
} // namespace rsl::math

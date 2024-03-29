#pragma once
#include "../util/type_util.hpp"
#include "swizzle/swizzle2.hpp"
#include "vector_base.hpp"
#include "vector_base.inl"

namespace rsl::math
{
	template <typename Scalar>
	struct vector<Scalar, 2> : vector_base
	{
		using scalar = Scalar;
		static constexpr size_type size = 2;
		using type = vector<Scalar, 2>;

		union
		{
			scalar data[2];

			_MATH_SWIZZLE_2_1_(scalar);
			_MATH_SWIZZLE_2_2_(scalar);
			_MATH_SWIZZLE_2_3_(scalar);
			_MATH_SWIZZLE_2_4_(scalar);
		};

		constexpr vector() noexcept
			: vector(static_cast<scalar>(0))
		{
		}

		constexpr vector(const vector&) noexcept = default;

		explicit constexpr vector(scalar s) noexcept
			: vector(s, s)
		{
		}

		constexpr vector(scalar _x, scalar _y) noexcept
			: x(_x),
			  y(_y)
		{
		}

		template <typename vec_type, ::std::enable_if_t<is_vector_v<vec_type> && (size != vec_type::size || !std::is_same_v<scalar, typename vec_type::scalar>), bool> = true>
		constexpr vector(const vec_type& other) noexcept
		{
			if constexpr (size > vec_type::size)
			{
				for (size_type i = 0; i < vec_type::size; i++)
					data[i] = static_cast<scalar>(other.data[i]);

				for (size_type i = vec_type::size; i < size; i++)
					data[i] = static_cast<scalar>(0);
			}
			else
			{
				for (size_type i = 0; i < size; i++)
					data[i] = static_cast<scalar>(other.data[i]);
			}
		}

		static const vector up;
		static const vector down;
		static const vector right;
		static const vector left;
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

		rythe_always_inline scalar length() const noexcept { return ::rsl::math::length(*this); }
		constexpr scalar length2() const noexcept { return ::rsl::math::length2(*this); }
		rythe_always_inline scalar angle() const noexcept { return static_cast<scalar>(::std::atan2(y, x)); }
	};

	template <>
	struct vector<bool, 2> : vector_base
	{
		using scalar = bool;
		static constexpr size_type size = 2;
		using type = vector<bool, 2>;

		union
		{
			scalar data[2];

			_MATH_SWIZZLE_2_1_(scalar);
			_MATH_SWIZZLE_2_2_(scalar);
			_MATH_SWIZZLE_2_3_(scalar);
			_MATH_SWIZZLE_2_4_(scalar);
		};

		constexpr vector() noexcept
			: xy(static_cast<scalar>(0), static_cast<scalar>(0))
		{
		}

		constexpr vector(const vector&) noexcept = default;

		explicit constexpr vector(scalar s) noexcept
			: xy(static_cast<scalar>(s), static_cast<scalar>(s))
		{
		}

		constexpr vector(scalar _x, scalar _y) noexcept
			: xy(_x, _y)
		{
		}

		template <typename vec_type, ::std::enable_if_t<is_vector_v<vec_type> && (size != vec_type::size || !std::is_same_v<scalar, typename vec_type::scalar>), bool> = true>
		constexpr vector(const vec_type& other) noexcept
		{
			if constexpr (size > vec_type::size)
			{
				for (size_type i = 0; i < vec_type::size; i++)
					data[i] = static_cast<scalar>(other.data[i]);

				for (size_type i = vec_type::size; i < size; i++)
					data[i] = static_cast<scalar>(0);
			}
			else
			{
				for (size_type i = 0; i < size; i++)
					data[i] = static_cast<scalar>(other.data[i]);
			}
		}

		static const vector up;
		static const vector down;
		static const vector right;
		static const vector left;
		static const vector one;
		static const vector zero;

		constexpr void set_mask(bitfield8 mask) noexcept
		{
			x = mask & 1;
			y = mask & 2;
		}
		constexpr bitfield8 mask() const noexcept { return static_cast<bitfield8>(x | y << 1u); }

		constexpr operator bool() const noexcept { return x && y; }

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

		constexpr scalar length() const noexcept { return x || y; }
		constexpr scalar length2() const noexcept { return this->length(); }
		constexpr scalar angle() const noexcept { return y; }
	};

	template <typename Scalar>
	const vector<Scalar, 2> vector<Scalar, 2>::up(static_cast<Scalar>(0), static_cast<Scalar>(1));
	template <typename Scalar>
	const vector<Scalar, 2> vector<Scalar, 2>::down(static_cast<Scalar>(0), static_cast<Scalar>(-1));
	template <typename Scalar>
	const vector<Scalar, 2> vector<Scalar, 2>::right(static_cast<Scalar>(1), static_cast<Scalar>(0));
	template <typename Scalar>
	const vector<Scalar, 2> vector<Scalar, 2>::left(static_cast<Scalar>(-1), static_cast<Scalar>(0));
	template <typename Scalar>
	const vector<Scalar, 2> vector<Scalar, 2>::one(static_cast<Scalar>(1));
	template <typename Scalar>
	const vector<Scalar, 2> vector<Scalar, 2>::zero(static_cast<Scalar>(0));

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
} // namespace rsl::math

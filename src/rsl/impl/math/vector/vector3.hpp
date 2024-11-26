#pragma once
#include "../util/type_util.hpp"
#include "swizzle/swizzle3.hpp"
#include "vector_base.hpp"
#include "vector_base.inl"

RYTHE_MSVC_SUPPRESS_WARNING_WITH_PUSH(4201)

namespace rsl::math
{
	template <arithmetic_type Scalar, mode Mode>
		requires signed_type<Scalar>
	struct vector<Scalar, 3, Mode>
	{
		using scalar = Scalar;
		static constexpr size_type size = 3;
		static constexpr mode mode = Mode;
		using type = vector<Scalar, 3>;
		using storage_type = storage_t<Scalar, size, Mode>;

		union
		{
			storage_type data;

			_MATH_SWIZZLE_3_1_(scalar, Mode);
			_MATH_SWIZZLE_3_2_(scalar, Mode);
			_MATH_SWIZZLE_3_3_(scalar, Mode);
			_MATH_SWIZZLE_3_4_(scalar, Mode);
		};

		[[rythe_always_inline]] constexpr vector() noexcept;
		[[rythe_always_inline]] constexpr vector(const vector&) noexcept = default;
		[[rythe_always_inline]] explicit constexpr vector(scalar s) noexcept;
		[[rythe_always_inline]] explicit constexpr vector(const vector<scalar, 2>& v, scalar s = static_cast<scalar>(0))
			noexcept;
		[[rythe_always_inline]] constexpr vector(scalar s, const vector<scalar, 2>& v) noexcept;
		[[rythe_always_inline]] constexpr vector(scalar _x, scalar _y, scalar _z = static_cast<scalar>(0)) noexcept;

		template <typename vec_type>
			requires not_same_as<Scalar, typename vec_type::scalar> || (vec_type::size != 3)
		[[rythe_always_inline]] constexpr vector(const vec_type& other) noexcept;

		static const vector up;
		static const vector down;
		static const vector right;
		static const vector left;
		static const vector forward;
		static const vector backward;
		static const vector one;
		static const vector zero;

		[[rythe_always_inline]] constexpr vector& operator=(const vector&) noexcept = default;

		[[nodiscard]] [[rythe_always_inline]] constexpr scalar& operator[](size_type i) noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const scalar& operator[](size_type i) const noexcept;
	};

	template <arithmetic_type Scalar, mode Mode>
		requires unsigned_type<Scalar>
	struct vector<Scalar, 3, Mode>
	{
		using scalar = Scalar;
		static constexpr size_type size = 3;
		static constexpr mode mode = Mode;
		using type = vector<Scalar, 3>;
		using storage_type = storage_t<Scalar, size, Mode>;

		union
		{
			storage_type data;

			_MATH_SWIZZLE_3_1_(scalar, Mode);
			_MATH_SWIZZLE_3_2_(scalar, Mode);
			_MATH_SWIZZLE_3_3_(scalar, Mode);
			_MATH_SWIZZLE_3_4_(scalar, Mode);
		};

		[[rythe_always_inline]] constexpr vector() noexcept;
		[[rythe_always_inline]] constexpr vector(const vector&) noexcept = default;
		[[rythe_always_inline]] explicit constexpr vector(scalar s) noexcept;
		[[rythe_always_inline]] explicit constexpr vector(const vector<scalar, 2>& v, scalar s = static_cast<scalar>(0))
			noexcept;
		[[rythe_always_inline]] constexpr vector(scalar s, const vector<scalar, 2>& v) noexcept;
		[[rythe_always_inline]] constexpr vector(scalar _x, scalar _y, scalar _z = static_cast<scalar>(0)) noexcept;

		template <typename vec_type>
			requires not_same_as<Scalar, typename vec_type::scalar> || (vec_type::size != 3)
		[[rythe_always_inline]] constexpr vector(const vec_type& other) noexcept;

		static const vector up;
		static const vector right;
		static const vector forward;
		static const vector one;
		static const vector zero;

		[[rythe_always_inline]] constexpr vector& operator=(const vector&) noexcept = default;

		[[nodiscard]] [[rythe_always_inline]] constexpr scalar& operator[](size_type i) noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const scalar& operator[](size_type i) const noexcept;
	};

	template <mode Mode>
	struct vector<bool, 3, Mode>
	{
		using scalar = bool;
		static constexpr size_type size = 3;
		static constexpr mode mode = Mode;
		using type = vector<bool, 3>;
		using storage_type = storage_t<scalar, size, Mode>;

		union
		{
			storage_type data;

			_MATH_SWIZZLE_3_1_(scalar, Mode);
			_MATH_SWIZZLE_3_2_(scalar, Mode);
			_MATH_SWIZZLE_3_3_(scalar, Mode);
			_MATH_SWIZZLE_3_4_(scalar, Mode);
		};

		[[rythe_always_inline]] constexpr vector() noexcept;
		[[rythe_always_inline]] constexpr vector(const vector&) noexcept = default;
		[[rythe_always_inline]] explicit constexpr vector(scalar s) noexcept;
		[[rythe_always_inline]] explicit constexpr vector(const vector<scalar, 2>& v, scalar s = static_cast<scalar>(0))
			noexcept;
		[[rythe_always_inline]] constexpr vector(scalar s, const vector<scalar, 2>& v) noexcept;
		[[rythe_always_inline]] constexpr vector(scalar _x, scalar _y, scalar _z = static_cast<scalar>(0)) noexcept;

		template <typename vec_type>
			requires not_same_as<bool, typename vec_type::scalar> || (vec_type::size != 3)
		[[rythe_always_inline]] constexpr vector(const vec_type& other) noexcept;

		static const vector up;
		static const vector down;
		static const vector right;
		static const vector left;
		static const vector forward;
		static const vector backward;
		static const vector one;
		static const vector zero;

		[[rythe_always_inline]] constexpr void set_mask(bitfield8 mask) noexcept
		{
			x = mask & 1;
			y = mask & 2;
			z = mask & 4;
		}

		[[nodiscard]] [[rythe_always_inline]] constexpr bitfield8 mask() const noexcept
		{
			return static_cast<bitfield8>(
				static_cast<uint32>(x) | static_cast<uint32>(y) << 1u | static_cast<uint32>(z) << 2u
			);
		}

		[[nodiscard]] [[rythe_always_inline]] constexpr operator bool() const noexcept { return x && y && z; }

		[[rythe_always_inline]] constexpr vector& operator=(const vector&) noexcept = default;

		[[nodiscard]] [[rythe_always_inline]] constexpr scalar& operator[](size_type i) noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const scalar& operator[](size_type i) const noexcept;
	};

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

#ifdef RYTHE_PCH
	template struct vector<float32, 3>;
	template struct vector<float64, 3>;
	template struct vector<int, 3>;
	template struct vector<uint, 3>;
	template struct vector<bool, 3>;
#endif // RYTHE_PCH
} // namespace rsl::math

RYTHE_MSVC_SUPPRESS_WARNING_POP

#pragma once
#include <immintrin.h>

#include "../../defines.hpp"
#include "predefined.hpp"
#include "vector_additions.hpp"

RYTHE_MSVC_SUPPRESS_WARNING_WITH_PUSH(4201) // anonymous struct

namespace rsl::math
{
	template <arithmetic_type Scalar, mode Mode>
	struct vector<Scalar, 4, Mode> : internal::vector_additions<Scalar, 4, Mode>
	{
		using scalar = Scalar;
		static constexpr size_type size = 4;
		static constexpr mode mode = Mode;
		using storage_type = storage_t<Scalar, size, Mode>;

		union
		{
			storage_type data;
			__m128 intrin[(sizeof(scalar) * size) / sizeof(__m128)];

#include "swizzle/swizzle4.inl"
		};

		static const vector one;
		static const vector zero;

		[[rythe_always_inline]] constexpr vector() noexcept;
		[[rythe_always_inline]] constexpr vector(const vector&) noexcept = default;
		[[rythe_always_inline]] explicit constexpr vector(scalar s) noexcept;

		template <math::mode OtherMode>
		[[rythe_always_inline]] explicit constexpr vector(
			const vector<scalar, 3, OtherMode>& v, scalar s = static_cast<scalar>(0)
		) noexcept;

		template <math::mode OtherMode>
		[[rythe_always_inline]] constexpr vector(scalar s, const vector<scalar, 3, OtherMode>& v) noexcept;

		template <math::mode OtherMode>
		[[rythe_always_inline]] constexpr vector(
			scalar s0, const vector<scalar, 2, OtherMode>& v, scalar s1 = static_cast<scalar>(0)
		) noexcept;

		template <math::mode OtherMode>
		[[rythe_always_inline]] constexpr vector(scalar s0, scalar s1, const vector<scalar, 2, OtherMode>& v) noexcept;

		template <math::mode OtherMode>
		[[rythe_always_inline]] explicit constexpr vector(
			const vector<scalar, 2, OtherMode>& v, scalar s0 = static_cast<scalar>(0),
			scalar s1 = static_cast<scalar>(0)
		) noexcept;

		template <math::mode Mode0, math::mode Mode1>
		[[rythe_always_inline]] constexpr vector(const vector<scalar, 2, Mode0>& v0, const vector<scalar, 2, Mode1>& v1)
			noexcept;

		[[rythe_always_inline]] constexpr vector(
			scalar _x, scalar _y, scalar _z = static_cast<scalar>(0), scalar _w = static_cast<scalar>(0)
		) noexcept;

		template <typename vec_type>
			requires not_same_as<Scalar, typename vec_type::scalar> || (vec_type::size != 4)
		[[rythe_always_inline]] constexpr vector(const vec_type& other) noexcept;

		[[rythe_always_inline]] constexpr vector& operator=(const vector&) noexcept = default;

		[[nodiscard]] [[rythe_always_inline]] constexpr scalar& operator[](size_type i) noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const scalar& operator[](size_type i) const noexcept;
	};

	template <mode Mode>
	struct vector<bool, 4, Mode> : internal::vector_additions<bool, 4, Mode>
	{
		using scalar = bool;
		static constexpr size_type size = 4;
		static constexpr mode mode = Mode;
		using storage_type = storage_t<bool, size, Mode>;

		union
		{
			scalar data[4];

#include "swizzle/swizzle4.inl"
		};

		static const vector one;
		static const vector zero;

		[[rythe_always_inline]] constexpr vector() noexcept;
		[[rythe_always_inline]] constexpr vector(const vector&) noexcept = default;
		[[rythe_always_inline]] explicit constexpr vector(scalar s) noexcept;

		template <math::mode OtherMode>
		[[rythe_always_inline]] explicit constexpr vector(
			const vector<scalar, 3, OtherMode>& v, scalar s = static_cast<scalar>(0)
		) noexcept;

		template <math::mode OtherMode>
		[[rythe_always_inline]] constexpr vector(scalar s, const vector<scalar, 3, OtherMode>& v) noexcept;

		template <math::mode OtherMode>
		[[rythe_always_inline]] constexpr vector(
			scalar s0, const vector<scalar, 2, OtherMode>& v, scalar s1 = static_cast<scalar>(0)
		) noexcept;

		template <math::mode OtherMode>
		[[rythe_always_inline]] constexpr vector(scalar s0, scalar s1, const vector<scalar, 2, OtherMode>& v) noexcept;

		template <math::mode OtherMode>
		[[rythe_always_inline]] explicit constexpr vector(
			const vector<scalar, 2, OtherMode>& v, scalar s0 = static_cast<scalar>(0),
			scalar s1 = static_cast<scalar>(0)
		) noexcept;

		template <math::mode Mode0, math::mode Mode1>
		[[rythe_always_inline]] constexpr vector(const vector<scalar, 2, Mode0>& v0, const vector<scalar, 2, Mode1>& v1)
			noexcept;

		[[rythe_always_inline]] constexpr vector(
			scalar _x, scalar _y, scalar _z = static_cast<scalar>(0), scalar _w = static_cast<scalar>(0)
		) noexcept;

		template <typename vec_type>
			requires not_same_as<bool, typename vec_type::scalar> || (vec_type::size != 4)
		[[rythe_always_inline]] constexpr vector(const vec_type& other) noexcept;

		[[rythe_always_inline]] constexpr void set_mask(bitfield8 mask) noexcept
		{
			x = (mask & 1) != 0;
			y = (mask & 2) != 0;
			z = (mask & 4) != 0;
			w = (mask & 8) != 0;
		}

		[[nodiscard]] [[rythe_always_inline]] constexpr bitfield8 mask() const noexcept
		{
			return static_cast<bitfield8>(
				static_cast<uint32>(x) | static_cast<uint32>(y) << 1u | static_cast<uint32>(z) << 2u |
				static_cast<uint32>(w) << 3u
			);
		}

		[[rythe_always_inline]] constexpr operator bool() const noexcept { return x && y && z && w; }

		[[rythe_always_inline]] constexpr vector& operator=(const vector&) noexcept = default;

		[[nodiscard]] [[rythe_always_inline]] constexpr scalar& operator[](size_type i) noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const scalar& operator[](size_type i) const noexcept;
	};

	using float4 = vector<float32, 4>;
	using double4 = vector<float64, 4>;
	using int4 = vector<int, 4>;
	using uint4 = vector<uint, 4>;
	using bool4 = vector<bool, 4>;
} // namespace rsl::math

RYTHE_MSVC_SUPPRESS_WARNING_POP

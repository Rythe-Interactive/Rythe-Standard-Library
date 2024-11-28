#pragma once
#include "../../defines.hpp"
#include "predefined.hpp"
#include "vector_additions.hpp"

RYTHE_MSVC_SUPPRESS_WARNING_WITH_PUSH(4201) // anonymous struct

namespace rsl::math
{
	template <arithmetic_type Scalar, mode Mode>
	struct vector<Scalar, 3, Mode> : internal::vector_additions<Scalar, 3, Mode>
	{
		using scalar = Scalar;
		static constexpr size_type size = 3;
		static constexpr mode mode = Mode;
		using storage_type = storage_t<Scalar, size, Mode>;

		union
		{
			storage_type data;

#include "swizzle/swizzle3.inl"
		};

		static const vector one;
		static const vector zero;

		[[rythe_always_inline]] constexpr vector() noexcept;
		[[rythe_always_inline]] constexpr vector(const vector&) noexcept = default;
		[[rythe_always_inline]] explicit constexpr vector(scalar s) noexcept;

		template <math::mode OtherMode>
		[[rythe_always_inline]] explicit constexpr vector(
			const vector<scalar, 2, OtherMode>& v, scalar s = static_cast<scalar>(0)
		) noexcept;

		template <math::mode OtherMode>
		[[rythe_always_inline]] constexpr vector(scalar s, const vector<scalar, 2, OtherMode>& v) noexcept;
		[[rythe_always_inline]] constexpr vector(scalar _x, scalar _y, scalar _z = static_cast<scalar>(0)) noexcept;

		template <typename vec_type>
			requires not_same_as<Scalar, typename vec_type::scalar> || (vec_type::size != 3)
		[[rythe_always_inline]] constexpr vector(const vec_type& other) noexcept;

		[[rythe_always_inline]] constexpr vector& operator=(const vector&) noexcept = default;

		[[nodiscard]] [[rythe_always_inline]] constexpr scalar& operator[](size_type i) noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const scalar& operator[](size_type i) const noexcept;
	};

	template <mode Mode>
	struct vector<bool, 3, Mode> : internal::vector_additions<bool, 3, Mode>
	{
		using scalar = bool;
		static constexpr size_type size = 3;
		static constexpr mode mode = Mode;
		using storage_type = storage_t<bool, size, Mode>;

		union
		{
			storage_type data;

#include "swizzle/swizzle3.inl"
		};

		static const vector one;
		static const vector zero;

		[[rythe_always_inline]] constexpr vector() noexcept;
		[[rythe_always_inline]] constexpr vector(const vector&) noexcept = default;
		[[rythe_always_inline]] explicit constexpr vector(scalar s) noexcept;

		template <math::mode OtherMode>
		[[rythe_always_inline]] explicit constexpr vector(
			const vector<scalar, 2, OtherMode>& v, scalar s = static_cast<scalar>(0)
		) noexcept;

		template <math::mode OtherMode>
		[[rythe_always_inline]] constexpr vector(scalar s, const vector<scalar, 2, OtherMode>& v) noexcept;
		[[rythe_always_inline]] constexpr vector(scalar _x, scalar _y, scalar _z = static_cast<scalar>(0)) noexcept;

		template <typename vec_type>
			requires not_same_as<bool, typename vec_type::scalar> || (vec_type::size != 3)
		[[rythe_always_inline]] constexpr vector(const vec_type& other) noexcept;

		[[rythe_always_inline]] constexpr void set_mask(bitfield8 mask) noexcept
		{
			x = (mask & 1) != 0;
			y = (mask & 2) != 0;
			z = (mask & 4) != 0;
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
	using double3 = vector<float64, 3>;
	using int3 = vector<int, 3>;
	using uint3 = vector<uint, 3>;
	using bool3 = vector<bool, 3>;
} // namespace rsl::math

RYTHE_MSVC_SUPPRESS_WARNING_POP

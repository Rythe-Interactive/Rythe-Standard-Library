#pragma once
#include "../../defines.hpp"
#include "predefined.hpp"
#include "vector_additions.hpp"

RYTHE_MSVC_SUPPRESS_WARNING_WITH_PUSH(4201) // anonymous struct

namespace rsl::math
{
	template <arithmetic_type Scalar, mode Mode>
	struct vector<Scalar, 2, Mode> : internal::vector_additions<Scalar, 2, Mode>
	{
		using scalar = Scalar;
		static constexpr size_type size = 2;
		static constexpr mode mode = Mode;
		using storage_type = storage_t<Scalar, size, Mode>;

		union
		{
			storage_type data;

#include "swizzle/swizzle2.inl"
		};

		static const vector one;
		static const vector zero;

		[[rythe_always_inline]] constexpr vector() noexcept;
		[[rythe_always_inline]] constexpr vector(const vector&) noexcept = default;
		[[rythe_always_inline]] explicit constexpr vector(scalar s) noexcept;
		[[rythe_always_inline]] constexpr vector(scalar _x, scalar _y) noexcept;

		template <typename vec_type>
			requires not_same_as<Scalar, typename vec_type::scalar> || (vec_type::size != 2)
		[[rythe_always_inline]] constexpr vector(const vec_type& other) noexcept;

		[[rythe_always_inline]] constexpr vector& operator=(const vector&) noexcept = default;

		[[nodiscard]] [[rythe_always_inline]] constexpr scalar& operator[](size_type i) noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const scalar& operator[](size_type i) const noexcept;
	};

	template <mode Mode>
	struct vector<bool, 2, Mode> : internal::vector_additions<bool, 2, Mode>
	{
		using scalar = bool;
		static constexpr size_type size = 2;
		static constexpr mode mode = Mode;
		using storage_type = storage_t<bool, size, Mode>;

		union
		{
			storage_type data;

#include "swizzle/swizzle2.inl"
		};

		static const vector one;
		static const vector zero;

		[[rythe_always_inline]] constexpr vector() noexcept;
		[[rythe_always_inline]] constexpr vector(const vector&) noexcept = default;
		[[rythe_always_inline]] explicit constexpr vector(scalar s) noexcept;
		[[rythe_always_inline]] constexpr vector(scalar _x, scalar _y) noexcept;

		template <typename vec_type>
			requires not_same_as<bool, typename vec_type::scalar> || (vec_type::size != 2)
		[[rythe_always_inline]] constexpr vector(const vec_type& other) noexcept;

		[[rythe_always_inline]] constexpr void set_mask(bitfield8 mask) noexcept
		{
			data[0] = (mask & 1) != 0;
			data[1] = (mask & 2) != 0;
		}

		[[nodiscard]] [[rythe_always_inline]] constexpr bitfield8 mask() const noexcept
		{
			return static_cast<bitfield8>(static_cast<uint32>(data[0]) | static_cast<uint32>(data[1]) << 1u);
		}

		[[rythe_always_inline]] constexpr operator bool() const noexcept { return data[0] && data[1]; }

		[[rythe_always_inline]] constexpr vector& operator=(const vector&) noexcept = default;

		[[rythe_always_inline]] constexpr scalar& operator[](size_type i) noexcept;
		[[rythe_always_inline]] constexpr const scalar& operator[](size_type i) const noexcept;
	};

	using float2 = vector<float32, 2>;
	using double2 = vector<float64, 2>;
	using int2 = vector<int, 2>;
	using uint2 = vector<uint, 2>;
	using bool2 = vector<bool, 2>;
} // namespace rsl::math

RYTHE_MSVC_SUPPRESS_WARNING_POP

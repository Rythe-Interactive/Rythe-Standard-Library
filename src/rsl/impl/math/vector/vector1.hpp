#pragma once
#include "../../defines.hpp"
#include "predefined.hpp"

RYTHE_MSVC_SUPPRESS_WARNING_WITH_PUSH(4201) // anonymous struct

namespace rsl::math
{
	template <arithmetic_type Scalar, mode Mode>
	struct vector<Scalar, 1, Mode>
	{
		using scalar = Scalar;
		static constexpr size_type size = 1;
		static constexpr mode mode = Mode;
		using storage_type = storage_t<Scalar, size, Mode>;

		union
		{
			storage_type data;

#include "swizzle/swizzle1.inl"
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
		using storage_type = storage_t<bool, size, Mode>;

		union
		{
			storage_type data;

#include "swizzle/swizzle1.inl"
		};

		[[rythe_always_inline]] constexpr vector() noexcept;
		[[rythe_always_inline]] constexpr vector(const vector&) noexcept = default;
		[[rythe_always_inline]] constexpr vector(scalar s) noexcept;

		template <typename vec_type>
			requires not_same_as<bool, typename vec_type::scalar> || (vec_type::size != 1)
		[[rythe_always_inline]] constexpr vector(const vec_type& other) noexcept;

		static const vector one;
		static const vector zero;

		[[rythe_always_inline]] constexpr void set_mask(bitfield8 mask) noexcept { x = (mask & 1) != 0; }
		[[nodiscard]] [[rythe_always_inline]] constexpr bitfield8 mask() const noexcept
		{
			return static_cast<bitfield8>(x);
		}

		[[rythe_always_inline]] constexpr vector& operator=(const vector&) noexcept = default;
		[[nodiscard]] [[rythe_always_inline]] constexpr operator bool() const noexcept { return x; }

		[[nodiscard]] [[rythe_always_inline]] constexpr scalar& operator[](size_type i) noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const scalar& operator[](size_type i) const noexcept;
	};

	using float1 = vector<float32, 1>;
	using double1 = vector<float64, 1>;
	using int1 = vector<int, 1>;
	using uint1 = vector<uint, 1>;
	using bool1 = vector<bool, 1>;
} // namespace rsl::math

RYTHE_MSVC_SUPPRESS_WARNING_POP

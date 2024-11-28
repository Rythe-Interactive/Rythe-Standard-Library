#pragma once
#include "../../defines.hpp"
#include "../constants.hpp"

namespace rsl::math
{
	template <typename precision = float32>
	struct degrees;

	template <typename precision = float32>
	struct angle
	{
		using scalar = precision;

		[[nodiscard]] [[rythe_always_inline]] constexpr scalar degrees() const noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr scalar radians() const noexcept;

		[[rythe_always_inline]] constexpr operator scalar() const noexcept { return value; }
		[[rythe_always_inline]] constexpr operator rsl::math::degrees<precision>() const noexcept;

		// the default is radians!
		scalar value;
	};

	template <typename precision = float32>
	using radians = angle<precision>;

	template <typename p>
	constexpr typename angle<p>::scalar angle<p>::degrees() const noexcept
	{
		return rad2deg(value);
	}

	template <typename p>
	constexpr typename angle<p>::scalar angle<p>::radians() const noexcept
	{
		return value;
	}

	template <typename precision>
	struct degrees
	{
		using scalar = precision;

		scalar value;

		[[rythe_always_inline]] constexpr operator radians<precision>() const noexcept { return {deg2rad(value)}; }
	};

	template <typename precision>
	constexpr angle<precision>::operator rsl::math::degrees<precision>() const noexcept
	{
		return {rad2deg(value)};
	}

	using angle32 = angle<float32>;
	using angle64 = angle<float64>;
	using angle_max = angle<float_max>;

	using radians32 = radians<float32>;
	using radians64 = radians<float64>;
	using radians_max = radians<float_max>;

	using degrees32 = degrees<float32>;
	using degrees64 = degrees<float64>;
	using degrees_max = degrees<float_max>;

#ifdef RYTHE_PCH
	template struct angle<float32>;
	template struct angle<float64>;
	template struct angle<float_max>;

	template struct degrees<float32>;
	template struct degrees<float64>;
	template struct degrees<float_max>;
#endif
} // namespace rsl::math

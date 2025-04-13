#pragma once
#include "../../defines.hpp"
#include "../constants.hpp"

namespace rsl::math
{
	template <typename Precision = float32>
	struct degrees;

	template <typename Precision = float32>
	struct angle
	{
		using scalar = Precision;

		[[nodiscard]] [[rythe_always_inline]] constexpr scalar degrees() const noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr scalar radians() const noexcept;

		[[rythe_always_inline]] constexpr operator scalar() const noexcept { return value; }
		[[rythe_always_inline]] constexpr operator rsl::math::degrees<Precision>() const noexcept;

		// the default is radians!
		scalar value;

		const static angle zero;
		const static angle deg45;
		const static angle deg60;
		const static angle deg90;
		const static angle deg120;
		const static angle deg180;
	};

	template <typename Precision = float32>
	using radians = angle<Precision>;

	template <typename Precision>
	constexpr typename angle<Precision>::scalar angle<Precision>::degrees() const noexcept
	{
		return rad2deg(value);
	}

	template <typename Precision>
	constexpr typename angle<Precision>::scalar angle<Precision>::radians() const noexcept
	{
		return value;
	}

	template <typename Precision>
	struct degrees
	{
		using scalar = Precision;

		scalar value;

		[[rythe_always_inline]] constexpr operator radians<Precision>() const noexcept { return {deg2rad(value)}; }
	};

	template <typename Precision>
	constexpr angle<Precision>::operator rsl::math::degrees<Precision>() const noexcept
	{
		return {rad2deg(value)};
	}

	template <typename Precision>
	const angle<Precision> angle<Precision>::zero = rsl::math::degrees<Precision>{0};
	template <typename Precision>
	const angle<Precision> angle<Precision>::deg45 = rsl::math::degrees<Precision>{45};
	template <typename Precision>
	const angle<Precision> angle<Precision>::deg60 = rsl::math::degrees<Precision>{60};
	template <typename Precision>
	const angle<Precision> angle<Precision>::deg90 = rsl::math::degrees<Precision>{90};
	template <typename Precision>
	const angle<Precision> angle<Precision>::deg120 = rsl::math::degrees<Precision>{120};
	template <typename Precision>
	const angle<Precision> angle<Precision>::deg180 = rsl::math::degrees<Precision>{180};

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

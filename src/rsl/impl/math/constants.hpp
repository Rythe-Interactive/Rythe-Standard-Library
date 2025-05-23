#pragma once
#include "../util/concepts.hpp"
#include "../util/primitives.hpp"

namespace rsl::math
{
	namespace internal
	{
		// note that this is in fact way more digits than even float_max can usually handle

		constexpr static float_max very_precise_e = 2.71828182845904523536028747135266249775724709369995957496l;
		constexpr static float_max very_precise_log2e = 1.69314718055994530941723212145817656807550013436025525412l;
		constexpr static float_max very_precise_log10e = 3.30258509299404568401799145468436420760110148862877297603l;

		constexpr static float_max very_precise_pi = 3.14159265358979323846264338327950288419716939937510582097l;
		constexpr static float_max very_precise_tau = 6.28318530717958647692528676655900576839433879875021164194l;
		constexpr static float_max very_precise_sqrt2 = 1.41421356237309504880168872420969807856967187537694807317l;
		constexpr static float_max very_precise_sqrt3 = 1.73205080756887729352744634150587236694280525381038062805l;

		constexpr static float_max very_precise_egamma = 0.57721566490153286060651209008240243104215933593992359880l;
		constexpr static float_max very_precise_phi = 1.61803398874989484820458683436563811772030917980576286213l;

		// I have honestly no idea what you would use this for,
		// but it is a neat constant so have fun with it anyways
		constexpr static float_max vp_feigenbaum = 4.66920160910299067185320382046620161725818557747576863274l;
	} // namespace internal

	template <arithmetic_type T = float32>
	constexpr T euler = static_cast<T>(internal::very_precise_e);

	template <arithmetic_type T = float32>
	constexpr T inv_euler = static_cast<T>(1) / euler<T>();

	template <arithmetic_type T = float32>
	constexpr T log2e = static_cast<T>(internal::very_precise_log2e);

	template <arithmetic_type T = float32>
	constexpr T log10e = static_cast<T>(internal::very_precise_log10e);

	template <arithmetic_type T = float32>
	constexpr T pi = static_cast<T>(internal::very_precise_pi);

	template <arithmetic_type T = float32>
	constexpr T half_pi = pi<T> * static_cast<T>(0.5);

	template <arithmetic_type T = float32>
	constexpr T quarter_pi = pi<T> * static_cast<T>(0.25);

	template <arithmetic_type T = float32>
	constexpr T tau = static_cast<T>(internal::very_precise_tau);

	template <arithmetic_type T = float32>
	constexpr T pau = pi<T> * static_cast<T>(1.5);

	template <arithmetic_type T = float32>
	constexpr T inv_pi = static_cast<T>(1) / pi<T>;

	template <arithmetic_type T = float32>
	constexpr T sqrt2 = static_cast<T>(internal::very_precise_sqrt2);

	template <arithmetic_type T = float32>
	constexpr T inv_sqrt2 = T(1) / sqrt2<T>();

	template <arithmetic_type T = float32>
	constexpr T sqrt3 = static_cast<T>(internal::very_precise_sqrt3);

	template <arithmetic_type T = float32>
	constexpr T inv_sqrt3 = static_cast<T>(1) / sqrt3<T>;

	template <arithmetic_type T = float32>
	constexpr T egamma = static_cast<T>(internal::very_precise_egamma);

	template <arithmetic_type T = float32>
	constexpr T phi = static_cast<T>(internal::very_precise_phi);

	template <arithmetic_type T = float32>
	constexpr T feigenbaum = static_cast<T>(internal::vp_feigenbaum);

	template <arithmetic_type T>
	constexpr T rad2deg = static_cast<T>(180) / pi<T>;

	template <arithmetic_type T>
	constexpr T deg2rad = pi<T> / static_cast<T>(180);

} // namespace rsl::math

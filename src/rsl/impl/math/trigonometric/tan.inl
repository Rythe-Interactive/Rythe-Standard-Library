#pragma once
#include "../basic/abs.hpp"
#include "../basic/is_nan.hpp"
#include "../basic/round.hpp"
#include "../util/limits.hpp"
#include "tan.hpp"

#include "tan_vector.inl"
// #include "tan_matrix.inl"
// #include "tan_quaternion.inl"

namespace rsl::math
{
	namespace internal
	{
		// https://en.wikipedia.org/wiki/Exponentiation_by_squaring
		template <floating_point_type T1, integral_type T2>
		constexpr T1 exp_by_squaring(T1 value, T2 exponent) noexcept
		{
			if (exponent == static_cast<T2>(3))
			{
				return value * value * value;
			}
			else if (exponent == static_cast<T2>(2))
			{
				return value * value;
			}
			else if (exponent == static_cast<T2>(1) || value == static_cast<T1>(1))
			{
				return value;
			}
			else if (exponent == static_cast<T2>(0))
			{
				return static_cast<T1>(1);
			}
			else if (exponent == limits<T2>::min)
			{
				return static_cast<T1>(0);
			}
			else if (exponent == limits<T2>::max)
			{
				if (value < static_cast<T1>(0))
				{
					return -limits<T1>::infinity;
				}
				else if (value > static_cast<T1>(1))
				{
					return limits<T1>::infinity;
				}

				return static_cast<T1>(0);
			}

			if constexpr (is_signed_v<T2>)
			{
				if (exponent < static_cast<T2>(0))
				{
					value = static_cast<T1>(1) / value;
					exponent = -exponent;
				}
			}

			T1 tmp = static_cast<T1>(1);
			while (exponent > static_cast<T2>(1))
			{
				if (exponent % static_cast<T2>(2) != static_cast<T2>(0))
				{
					tmp *= value;
					--exponent;
				}

				value *= value;
				exponent /= static_cast<T2>(2);
			}

			return value * tmp;
		}

		template <floating_point_type Scalar>
		constexpr Scalar tan_series_exp_long(const Scalar z) noexcept
		{
			// this is valued on a fourth-order expansion of tan(z) using Bernoulli numbers
			return static_cast<Scalar>(-1) / z +
				   (z / static_cast<Scalar>(3) +
					(exp_by_squaring(z, 3) / static_cast<Scalar>(45) +
					 (static_cast<Scalar>(2) * exp_by_squaring(z, 5) / static_cast<Scalar>(945) +
					  exp_by_squaring(z, 7) / static_cast<Scalar>(4725))));
		}

		template <floating_point_type Scalar>
		constexpr Scalar tan_series_exp(const Scalar x) noexcept
		{
			if (limits<Scalar>::min > abs(x - half_pi<Scalar>))
			{
				// the value tan(pi/2) is somewhat of a convention;
				// technically the function is not defined at EXACTLY pi/2,
				// but this is floating point pi/2
				return static_cast<Scalar>(1.633124e+16);
			}

			return tan_series_exp_long(x - half_pi<Scalar>);
		}

		template <floating_point_type Scalar>
		constexpr Scalar tan_cf_recur(const Scalar xx, const size_type depth, const size_type maxDepth) noexcept
		{
			if (depth < maxDepth)
			{
				return static_cast<Scalar>(2 * depth - 1) - xx / tan_cf_recur(xx, depth + 1ull, maxDepth);
			}

			return static_cast<Scalar>(2 * depth - 1);
		}

		template <floating_point_type Scalar>
		constexpr Scalar tan_cf_main(const Scalar x) noexcept
		{
			// deals with a singularity at tan(pi/2)
			if (x > static_cast<Scalar>(1.55) && x < static_cast<Scalar>(1.60))
			{
				return tan_series_exp(x);
			}

			if (x > static_cast<Scalar>(1.4))
			{
				return x / tan_cf_recur(x * x, 1ull, 45ull);
			}

			if (x > static_cast<Scalar>(1))
			{
				return x / tan_cf_recur(x * x, 1ull, 35ull);
			}

			return x / tan_cf_recur(x * x, 1ull, 25ull);
		}

		template <floating_point_type Scalar>
		constexpr Scalar tan_begin(const Scalar x, const size_type count = 0) noexcept
		{
			// tan(x) = tan(x + pi)
			if (x > pi<Scalar>)
			{
				if (count > 1ull)
				{
					return limits<Scalar>::quiet_nan;
				}

				return tan_begin(x - pi<Scalar> * floor(x / pi<Scalar>), count + 1ull);
			}

			return tan_cf_main(x);
		}

		template <floating_point_type Scalar>
		constexpr Scalar constexpr_tan(const Scalar x) noexcept
		{
			// NaN check
			if (is_nan(x))
			{
				return limits<Scalar>::quiet_nan;
			}

			// indistinguishable from zero
			if (limits<Scalar>::min > abs(x))
			{
				return Scalar(0);
			}

			if (x < Scalar(0))
			{
				return -tan_begin(-x);
			}

			return tan_begin(x);
		}

		template <floating_point_type Scalar>
		constexpr Scalar constexpr_atan(const Scalar x) noexcept
		{
			// NaN check
			if (is_nan(x))
			{
				return limits<Scalar>::quiet_nan;
			}

			return x;
		}
	} // namespace internal

	// tan
	template <typename T>
		requires floating_point_type<T> || linear_algebraic_construct<T>
	constexpr auto tan(T&& value) noexcept
	{
		if constexpr (is_linear_algebraic_construct_v<T>)
		{
			return internal::compute_tan<remove_cvr_t<T>>::compute(forward<T>(value));
		}
		else
		{
			if (is_constant_evaluated())
			{
				return internal::constexpr_tan<remove_cvr_t<T>>(value);
			}
			else
			{
				return ::std::tan(forward<T>(value));
			}
		}
	}

	// atan
	template <typename T>
		requires floating_point_type<T> || linear_algebraic_construct<T>
	constexpr auto atan(T&& value) noexcept
	{
		if constexpr (is_linear_algebraic_construct_v<T>)
		{
			return internal::compute_tan<remove_cvr_t<T>>::compute_inverse(forward<T>(value));
		}
		else
		{
			if (is_constant_evaluated())
			{
				return internal::constexpr_atan<remove_cvr_t<T>>(value);
			}
			else
			{
				return ::std::atan(forward<T>(value));
			}
		}
	}

	template <typename X, typename Y>
		requires same_as<remove_cvr_t<X>, remove_cvr_t<Y>> && (floating_point_type<X> || linear_algebraic_construct<X>)
	constexpr auto atan2(X&& x, Y&& y) noexcept
	{
		if constexpr (is_linear_algebraic_construct_v<X>)
		{
			return internal::compute_tan<elevated_t<X, Y>>::compute_inverse2(forward<X>(x), forward<Y>(y));
		}
		else
		{
			if (is_constant_evaluated())
			{
				return internal::constexpr_atan<remove_cvr_t<X>>(y / x);
			}
			else
			{
				return ::std::atan2(forward<X>(x), forward<Y>(y));
			}
		}
	}
} // namespace rsl::math

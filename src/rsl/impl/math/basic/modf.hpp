#pragma once
#include <cmath>
#include <limits>

#include "../../util/primitives.hpp"
#include "../util/type_util.hpp"

namespace rsl::math
{
	namespace internal
	{
		template <typename Scalar, size_type Size>
		struct compute_modf
		{
			static constexpr size_type size = Size;
			using value_type = vector<Scalar, size>;

			[[rythe_always_inline]] static value_type compute(const value_type& val, value_type& integer) noexcept
			{
				value_type result;
				for (size_type i = 0; i < size; i++)
				{
					result[i] = ::std::modf(val[i], &integer[i]);
				}
				return result;
			}
		};

		template <typename Scalar>
		struct compute_modf<Scalar, 1u>
		{
			static constexpr size_type size = 1u;
			using value_type = vector<Scalar, size>;

			template <typename RET>
			[[rythe_always_inline]] static value_type compute(Scalar val, RET& integer) noexcept
			{
				if constexpr (is_vector_v<RET>)
				{
					return ::std::modf(val, &integer[0]);
				}
				else
				{
					return ::std::modf(val, &integer);
				}
			}
		};
	} // namespace internal

	template <typename T, typename RET>
	[[rythe_always_inline]] static auto modf(const T& val, RET& integer)
	{
		if constexpr (is_vector_v<T>)
		{
			static_assert(
				::std::is_floating_point_v<typename T::scalar>,
				"Value must be floating point in order to use modf. (Did you mean fmod?)"
			);
			return internal::compute_modf<typename T::scalar, T::size>::compute(val, integer);
		}
		else
		{
			static_assert(
				::std::is_floating_point_v<T>, "Value must be floating point in order to use modf. (Did you mean fmod?)"
			);
			return ::std::modf(val, &integer);
		}
	}
} // namespace rsl::math

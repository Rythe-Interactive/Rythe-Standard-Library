#pragma once
#include "../../util/primitives.hpp"
#include "../util/close_enough.hpp"
#include "../vector/vector.hpp"
#include "invert.hpp"

namespace rsl::math
{
	namespace internal
	{
		template <vector_type vec_type>
		struct compute_equality
		{
			using result_type = vector<bool, vec_type::size, vec_type::mode>;

			[[nodiscard]] [[rythe_always_inline]] constexpr static result_type
			compute(const vec_type& a, const vec_type& b) noexcept
			{
				result_type result;
				for (size_type i = 0; i < vec_type::size; i++)
				{
					result[i] = close_enough(a[i], b[i]);
				}
				return result;
			}

			[[nodiscard]] [[rythe_always_inline]] constexpr static result_type
			compute(const vec_type& a, typename vec_type::scalar b) noexcept
			{
				result_type result;
				for (size_type i = 0; i < vec_type::size; i++)
				{
					result[i] = close_enough(a[i], b);
				}
				return result;
			}
		};

		template <arithmetic_type Scalar, mode Mode>
		struct compute_equality<vector<Scalar, 1, Mode>>
		{
			[[nodiscard]] [[rythe_always_inline]] constexpr static bool compute(Scalar a, Scalar b) noexcept
			{
				return close_enough(a, b);
			}
		};
	} // namespace internal

	template <vector_type vec_type0, vector_type vec_type1>
	[[nodiscard]] constexpr auto equals(const vec_type0& a, const vec_type1& b) noexcept
	{
		return internal::compute_equality<elevated_t<vec_type0, vec_type1>>::compute(a, b);
	}

	template <vector_type vec_type0, vector_type vec_type1>
	[[nodiscard]] constexpr auto operator==(const vec_type0& a, const vec_type1& b) noexcept
	{
		return internal::compute_equality<elevated_t<vec_type0, vec_type1>>::compute(a, b);
	}

	template <vector_type vec_type0, vector_type vec_type1>
	[[nodiscard]] constexpr auto operator!=(const vec_type0& a, const vec_type1& b) noexcept
	{
		return !internal::compute_equality<elevated_t<vec_type0, vec_type1>>::compute(a, b);
	}

	template <vector_type vec_type>
	[[nodiscard]] constexpr auto equals(const vec_type& a, typename vec_type::scalar b) noexcept
	{
		return internal::compute_equality<vec_type>::compute(a, b);
	}

	template <vector_type vec_type>
	[[nodiscard]] constexpr vector<bool, vec_type::size, vec_type::mode>
	operator!=(const vec_type& a, typename vec_type::scalar b) noexcept
	{
		auto result = internal::compute_equality<vec_type>::compute(a, b);
		return !result;
	}
} // namespace rsl::math

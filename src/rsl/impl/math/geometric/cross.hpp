#pragma once
#include "../../util/primitives.hpp"
#include "../vector/vector.hpp"

namespace rsl::math
{
	namespace internal
	{
		template <typename Scalar, size_type Size>
		struct compute_cross
		{
			static constexpr size_type size = Size;
			using value_type = vector<Scalar, size>;

			constexpr static size_type index_at(size_type index) { return index % size; }

			constexpr static value_type compute(const value_type& a, const value_type& b) noexcept
			{
				value_type result;
				for (size_type i = 0; i < size; i++)
				{
					result[i] = a[index_at(i + 1)] * b[index_at(i + 2)] - b[index_at(i + 1)] * a[index_at(i + 2)];
				}
				return result;
			}
		};
	} // namespace internal

	template <
		typename vec_type0, typename vec_type1,
		std::enable_if_t<is_vector_v<vec_type0> && is_vector_v<vec_type1>, bool> = true>
	constexpr auto cross(const vec_type0& a, const vec_type1& b) noexcept
	{
		return internal::compute_cross<typename vec_type0::scalar, vec_type0::size>::compute(a, b);
	}
} // namespace rsl::math

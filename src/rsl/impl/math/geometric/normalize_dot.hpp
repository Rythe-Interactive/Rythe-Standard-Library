#pragma once
#include "../exponential/sqrt.hpp"
#include "dot.hpp"

namespace rsl::math
{
	template <
		typename vec_type0, typename vec_type1,
		std::enable_if_t<is_vector_v<vec_type0> && is_vector_v<vec_type1>, bool> = true>
	[[nodiscard]] [[rythe_always_inline]] constexpr auto normalize_dot(const vec_type0& a, const vec_type1& b) noexcept
	{
		return dot(a, b) * rcp_sqrt(dot(a, a) * dot(b, b));
	}
} // namespace rsl::math

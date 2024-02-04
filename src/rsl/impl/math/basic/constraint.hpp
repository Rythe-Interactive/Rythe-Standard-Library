#pragma once
#include "../../defines.hpp"

#include "../util/type_util.hpp"
#include "../vector/vector.hpp"

namespace rsl::math
{
	template <typename A, typename B>
	[[nodiscard]] rythe_always_inline constexpr auto min(A&& a, B&& b);

	template <typename A, typename B>
	[[nodiscard]] rythe_always_inline constexpr auto max(A&& a, B&& b);

	template <typename In, typename Min, typename Max>
	[[nodiscard]] rythe_always_inline constexpr auto clamp(In&& in, Min&& min, Max&& max);

	template <typename In>
	[[nodiscard]] rythe_always_inline constexpr auto saturate(In&& in);
} // namespace rsl::math

#include "constraint.inl"

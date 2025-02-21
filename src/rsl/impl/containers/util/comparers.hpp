#pragma once
#include "../../defines.hpp"

namespace rsl
{
	template <typename T>
	struct equal
	{
		[[nodiscard]] [[rythe_always_inline]] constexpr bool operator()(const T& lhs, const T& rhs) const noexcept
		{
			return lhs == rhs;
		}
	};
} // namespace rsl

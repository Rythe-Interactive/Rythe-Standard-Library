#pragma once
#include "../../util/primitives.hpp"

#include "../util/type_util.hpp"
#include "matrix_base.hpp"

namespace rsl::math
{
	template <typename mat_type, ::std::enable_if_t<is_matrix_v<mat_type>, bool> = true>
	[[nodiscard]] rythe_always_inline constexpr auto determinant(const mat_type& mat) noexcept;
}

#include "determinant.inl"

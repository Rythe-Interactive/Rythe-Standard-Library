#include "length.hpp"
#pragma once

#include "length_quat.inl"
#include "length_vector.inl"

namespace rsl::math
{
	template <typename vec_type, ::std::enable_if_t<is_vector_v<vec_type>, bool>>
	[[nodiscard]] auto length(const vec_type& v) noexcept
	{
		return internal::compute_length<vec_type>::compute(v);
	}

	template <typename vec_type, ::std::enable_if_t<is_vector_v<vec_type>, bool>>
	[[nodiscard]] constexpr auto length2(const vec_type& v) noexcept
	{
		return internal::compute_length<vec_type>::compute2(v);
	}

	template <typename quat_type, ::std::enable_if_t<is_quat_v<quat_type>, bool>>
	[[nodiscard]] auto length(const quat_type& q) noexcept
	{
		return internal::compute_length<quat_type>::compute(q);
	}

	template <typename quat_type, ::std::enable_if_t<is_quat_v<quat_type>, bool>>
	[[nodiscard]] constexpr auto length2(const quat_type& q) noexcept
	{
		return internal::compute_length<quat_type>::compute2(q);
	}

	template <typename mat_type, ::std::enable_if_t<is_matrix_v<mat_type>, bool>>
	[[nodiscard]] auto length(const mat_type& m) noexcept
	{
		return determinant(m);
	}

	template <typename mat_type, ::std::enable_if_t<is_matrix_v<mat_type>, bool>>
	[[nodiscard]] constexpr auto length2(const mat_type& m) noexcept
	{
		auto det = determinant(m);
		return det * det;
	}
} // namespace rsl::math

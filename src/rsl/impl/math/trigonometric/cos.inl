#include "cos.hpp"
#pragma once

#include "cos_vector.inl"
// #include "cos_matrix.inl"
// #include "cos_quaternion.inl"

namespace rsl::math
{
	// cos
	template <typename T>
	[[nodiscard]] auto cos(T&& value) noexcept
	{
		using Type = ::std::remove_cvref_t<T>;

		if constexpr (is_quat_v<Type>)
		{
			return internal::compute_cos<quaternion<typename Type::scalar>>::compute(forward<T>(value));
		}
		if constexpr (is_matrix_v<Type>)
		{
			return internal::compute_cos<matrix<typename Type::scalar, Type::row_count, Type::col_count>>::compute(
				forward<T>(value)
			);
		}
		if constexpr (is_vector_v<Type>)
		{
			return internal::compute_cos<vector<typename Type::scalar, Type::size>>::compute(forward<T>(value));
		}
		else
		{
			return ::std::cos(forward<T>(value));
		}
	}

	// acos
	template <typename T>
	[[nodiscard]] auto acos(T&& value) noexcept
	{
		using Type = ::std::remove_cvref_t<T>;

		if constexpr (is_quat_v<Type>)
		{
			return internal::compute_cos<quaternion<typename Type::scalar>>::compute_inverse(forward<T>(value));
		}
		if constexpr (is_matrix_v<Type>)
		{
			return internal::compute_cos<
				matrix<typename Type::scalar, Type::row_count, Type::col_count>>::compute_inverse(forward<T>(value));
		}
		if constexpr (is_vector_v<Type>)
		{
			return internal::compute_cos<vector<typename Type::scalar, Type::size>>::compute_inverse(forward<T>(value));
		}
		else
		{
			return ::std::acos(forward<T>(value));
		}
	}
} // namespace rsl::math

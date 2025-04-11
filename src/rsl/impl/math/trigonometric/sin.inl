#include "sin.hpp"
#pragma once

#include "sin_vector.inl"
// #include "sin_matrix.inl"
// #include "sin_quaternion.inl"

namespace rsl::math
{
	// sin
	template <typename T>
	[[nodiscard]] auto sin(T&& value) noexcept
	{
		using Type = ::std::remove_cvref_t<T>;

		if constexpr (is_quat_v<Type>)
		{
			return internal::compute_sin<quaternion<typename Type::scalar>>::compute(forward<T>(value));
		}
		if constexpr (is_matrix_v<Type>)
		{
			return internal::compute_sin<matrix<typename Type::scalar, Type::row_count, Type::col_count>>::compute(
				forward<T>(value)
			);
		}
		if constexpr (is_vector_v<Type>)
		{
			return internal::compute_sin<vector<typename Type::scalar, Type::size>>::compute(forward<T>(value));
		}
		else
		{
			return ::std::sin(forward<T>(value));
		}
	}

	// asin
	template <typename T>
	[[nodiscard]] auto asin(T&& value) noexcept
	{
		using Type = ::std::remove_cvref_t<T>;

		if constexpr (is_quat_v<Type>)
		{
			return internal::compute_sin<quaternion<typename Type::scalar>>::compute_inverse(forward<T>(value));
		}
		if constexpr (is_matrix_v<Type>)
		{
			return internal::compute_sin<
				matrix<typename Type::scalar, Type::row_count, Type::col_count>>::compute_inverse(forward<T>(value));
		}
		if constexpr (is_vector_v<Type>)
		{
			return internal::compute_sin<vector<typename Type::scalar, Type::size>>::compute_inverse(forward<T>(value));
		}
		else
		{
			return ::std::asin(forward<T>(value));
		}
	}
} // namespace rsl::math

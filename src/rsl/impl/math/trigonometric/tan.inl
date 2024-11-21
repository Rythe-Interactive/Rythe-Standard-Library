#include "tan.hpp"
#pragma once

#include "tan_vector.inl"
// #include "tan_matrix.inl"
// #include "tan_quaternion.inl"

namespace rsl::math
{
	// tan
	template <typename T>
	[[nodiscard]] auto tan(T&& value) noexcept
	{
		using Type = remove_cvr_t<T>;

		if constexpr (is_quat_v<Type>)
		{
			return detail::compute_tan<quaternion<typename Type::scalar>>::compute(forward<T>(value));
		}
		if constexpr (is_matrix_v<Type>)
		{
			return detail::compute_tan<matrix<typename Type::scalar, Type::row_count, Type::col_count>>::compute(forward<T>(value));
		}
		if constexpr (is_vector_v<Type>)
		{
			return detail::compute_tan<vector<typename Type::scalar, Type::size>>::compute(forward<T>(value));
		}
		else
		{
			return ::std::tan(forward<T>(value));
		}
	}

	// atan
	template <typename T>
	[[nodiscard]] auto atan(T&& value) noexcept
	{
		using Type = remove_cvr_t<T>;

		if constexpr (is_quat_v<Type>)
		{
			return detail::compute_tan<quaternion<typename Type::scalar>>::compute_inverse(forward<T>(value));
		}
		if constexpr (is_matrix_v<Type>)
		{
			return detail::compute_tan<matrix<typename Type::scalar, Type::row_count, Type::col_count>>::compute_inverse(forward<T>(value));
		}
		if constexpr (is_vector_v<Type>)
		{
			return detail::compute_tan<vector<typename Type::scalar, Type::size>>::compute_inverse(forward<T>(value));
		}
		else
		{
			return ::std::atan(forward<T>(value));
		}
	}

	template <typename X, typename Y>
		requires same_as<remove_cvr_t<X>, remove_cvr_t<Y>>
	auto atan2(X&& x, Y&& y) noexcept
	{
		using Type = remove_cvr_t<X>;

		if constexpr (is_quat_v<Type>)
		{
			return detail::compute_tan<quaternion<typename Type::scalar>>::compute_inverse2(
				forward<X>(x), forward<Y>(y)
			);
		}
		if constexpr (is_matrix_v<Type>)
		{
			return detail::compute_tan<matrix<typename Type::scalar, Type::row_count, Type::col_count>>::
				compute_inverse2(forward<X>(x), forward<Y>(y));
		}
		if constexpr (is_vector_v<Type>)
		{
			return detail::compute_tan<vector<typename Type::scalar, Type::size>>::compute_inverse2(
				forward<X>(x), forward<Y>(y)
			);
		}
		else
		{
			return ::std::atan2(forward<X>(x), forward<Y>(y));
		}
	}
} // namespace rsl::math

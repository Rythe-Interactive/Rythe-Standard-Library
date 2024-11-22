#include "multiplication.hpp"
#pragma once
#include <immintrin.h>

#include "multiplication_matrix.inl"
#include "multiplication_quaternion.inl"
#include "multiplication_vector.inl"

namespace rsl::math
{
	// mul
	template <typename TypeA, typename TypeB>
	[[nodiscard]] constexpr auto mul(TypeA&& a, TypeB&& b) noexcept
	{
		using A = remove_cvr_t<TypeA>;
		using B = remove_cvr_t<TypeB>;

		if constexpr (is_quat_v<A> && is_quat_v<B>)
		{
			using scalar = elevated_t<typename A::scalar, typename B::scalar>;
			return detail::compute_multiplication<quaternion<scalar>>::compute(forward<TypeA>(a), forward<TypeB>(b));
		}
		else if constexpr (is_vector_v<A> && is_quat_v<B>)
		{
			using scalar = elevated_t<typename A::scalar, typename B::scalar>;
			return detail::compute_multiplication<quaternion<scalar>>::compute(forward<TypeA>(a), forward<TypeB>(b));
		}
		else if constexpr (is_quat_v<A> && is_vector_v<B>)
		{
			using scalar = elevated_t<typename A::scalar, typename B::scalar>;
			return detail::compute_multiplication<quaternion<scalar>>::compute(forward<TypeB>(b), forward<TypeA>(a));
		}
		else if constexpr (is_matrix_v<A> && is_matrix_v<B>)
		{
			using scalar = elevated_t<typename A::scalar, typename B::scalar>;
			return detail::compute_multiplication<matrix<scalar, A::row_count, A::col_count>>::compute(
				forward<TypeA>(a), forward<TypeB>(b)
			);
		}
		else if constexpr (is_matrix_v<A> && is_vector_v<B>)
		{
			using scalar = elevated_t<typename A::scalar, typename B::scalar>;
			return detail::compute_multiplication<matrix<scalar, A::row_count, A::col_count>>::compute(
				forward<TypeB>(b), forward<TypeA>(a)
			);
		}
		else if constexpr (is_vector_v<A> && is_matrix_v<B>)
		{
			using scalar = elevated_t<typename A::scalar, typename B::scalar>;
			return detail::compute_multiplication<matrix<scalar, B::row_count, B::col_count>>::compute(
				forward<TypeA>(a), forward<TypeB>(b)
			);
		}
		else if constexpr (is_vector_v<A> && is_vector_v<B>)
		{
			using scalar = elevated_t<typename A::scalar, typename B::scalar>;
			constexpr size_type size = min(A::size, B::size);
			return detail::compute_multiplication<vector<scalar, size>>::compute(forward<TypeA>(a), forward<TypeB>(b));
		}
		else if constexpr (is_vector_v<A> || is_vector_v<B>)
		{
			using vec_type = select_vector_type_t<A, B>;
			using scalar = elevated_t<typename vec_type::scalar, select_floating_type_t<A, B>>;
			return detail::compute_multiplication<vector<scalar, vec_type::size>>::compute(
				forward<TypeA>(a), forward<TypeB>(b)
			);
		}
		else if constexpr (is_quat_v<A> && is_scalar_v<B>)
		{
			return detail::compute_multiplication<quaternion<elevated_t<typename A::scalar, B>>>::compute(
				std::forward<TypeA>(a), std::forward<TypeB>(b)
			);
		}
		else if constexpr (is_scalar_v<A> && is_quat_v<B>)
		{
			return detail::compute_multiplication<quaternion<elevated_t<A, typename B::scalar>>>::compute(
				std::forward<TypeB>(b), std::forward<TypeA>(a)
			);
		}
		else
		{
			return a * b;
		}
	}

	template <linear_algebraic_construct TypeA, linear_algebraic_construct TypeB>
	[[nodiscard]] constexpr auto operator*(TypeA&& a, TypeB&& b) noexcept
	{
		return mul(forward<TypeA>(a), forward<TypeB>(b));
	}

	template <typename TypeA, typename TypeB>
		requires(linear_algebraic_construct<TypeA> || scalar_type<TypeA>) &&
				(linear_algebraic_construct<TypeB> || scalar_type<TypeB>)
	constexpr TypeA& mul_assign(TypeA& a, TypeB&& b) noexcept
	{
		return a = a * b;
	}

	template <linear_algebraic_construct TypeA, linear_algebraic_construct TypeB>
	constexpr TypeA& operator*=(TypeA& a, TypeB&& b) noexcept
	{
		return a = a * b;
	}

} // namespace rsl::math

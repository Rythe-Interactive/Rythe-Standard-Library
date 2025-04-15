#pragma once
#include "multiplication.hpp"

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
			return internal::compute_multiplication<elevated_t<A, B>>::compute(forward<TypeA>(a), forward<TypeB>(b));
		}
		else if constexpr (is_vector_v<A> && is_quat_v<B>)
		{
			using scalar = elevated_t<typename A::scalar, typename B::scalar>;
			constexpr storage_mode mode = elevated_storage_mode_v<A::mode, B::mode>;
			return internal::compute_multiplication<quaternion<scalar, mode>>::compute(forward<TypeA>(a), forward<TypeB>(b));
		}
		else if constexpr (is_quat_v<A> && is_vector_v<B>)
		{
			using scalar = elevated_t<typename A::scalar, typename B::scalar>;
			constexpr storage_mode mode = elevated_storage_mode_v<A::mode, B::mode>;
			return internal::compute_multiplication<quaternion<scalar, mode>>::compute(forward<TypeB>(b), forward<TypeA>(a));
		}
		else if constexpr (is_matrix_v<A> && is_matrix_v<B>)
		{
			return internal::compute_multiplication<elevated_t<A, B>>::compute(
				forward<TypeA>(a), forward<TypeB>(b)
			);
		}
		else if constexpr (is_matrix_v<A> && is_vector_v<B>)
		{
			using scalar = elevated_t<typename A::scalar, typename B::scalar>;
			constexpr storage_mode mode = elevated_storage_mode_v<A::mode, B::mode>;
			return internal::compute_multiplication<matrix<scalar, A::row_count, A::col_count, mode>>::compute(
				forward<TypeB>(b), forward<TypeA>(a)
			);
		}
		else if constexpr (is_vector_v<A> && is_matrix_v<B>)
		{
			using scalar = elevated_t<typename A::scalar, typename B::scalar>;
			constexpr storage_mode mode = elevated_storage_mode_v<A::mode, B::mode>;
			return internal::compute_multiplication<matrix<scalar, B::row_count, B::col_count, mode>>::compute(
				forward<TypeA>(a), forward<TypeB>(b)
			);
		}
		else if constexpr (is_vector_v<A> && is_vector_v<B>)
		{
			return internal::compute_multiplication<elevated_t<A, B>>::compute(
				forward<TypeA>(a), forward<TypeB>(b)
			);
		}
		else if constexpr (is_vector_v<A> || is_vector_v<B>)
		{
			using vec_type = select_vector_type_t<A, B>;
			using scalar = elevated_t<typename vec_type::scalar, select_floating_type_t<A, B>>;
			return internal::compute_multiplication<vector<scalar, vec_type::size, vec_type::mode>>::compute(
				forward<TypeA>(a), forward<TypeB>(b)
			);
		}
		else if constexpr (is_quat_v<A> && is_arithmetic_v<B>)
		{
			return internal::compute_multiplication<quaternion<elevated_t<typename A::scalar, B>, A::mode>>::compute(
				forward<TypeA>(a), forward<TypeB>(b)
			);
		}
		else if constexpr (is_arithmetic_v<A> && is_quat_v<B>)
		{
			return internal::compute_multiplication<quaternion<elevated_t<A, typename B::scalar>, B::mode>>::compute(
				forward<TypeB>(b), forward<TypeA>(a)
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
		requires(linear_algebraic_construct<TypeA> && arithmetic_type<TypeB>) ||
				(linear_algebraic_construct<TypeB> && arithmetic_type<TypeA>)
	constexpr auto operator*(TypeA&& a, TypeB&& b) noexcept
	{
		return mul(forward<TypeA>(a), forward<TypeB>(b));
	}

	template <linear_algebraic_construct TypeA, linear_algebraic_construct TypeB>
	constexpr TypeA& mul_assign(TypeA& a, TypeB&& b) noexcept
	{
		return a = a * b;
	}

	template <typename TypeA, typename TypeB>
		requires(linear_algebraic_construct<TypeA> && arithmetic_type<TypeB>) ||
				(linear_algebraic_construct<TypeB> && arithmetic_type<TypeA>)
	constexpr TypeA& mul_assign(TypeA& a, TypeB&& b) noexcept
	{
		return a = a * b;
	}

	template <linear_algebraic_construct TypeA, linear_algebraic_construct TypeB>
	constexpr TypeA& operator*=(TypeA& a, TypeB&& b) noexcept
	{
		return a = a * b;
	}

	template <typename TypeA, typename TypeB>
		requires(linear_algebraic_construct<TypeA> && arithmetic_type<TypeB>) ||
				(linear_algebraic_construct<TypeB> && arithmetic_type<TypeA>)
	constexpr TypeA& operator*=(TypeA& a, TypeB&& b) noexcept
	{
		return a = a * b;
	}

} // namespace rsl::math

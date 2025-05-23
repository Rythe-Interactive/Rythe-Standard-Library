#include "quintic.hpp"
#pragma once
#include <array>

namespace rsl::math::internal
{
	template <typename T>
	[[nodiscard]] [[rythe_always_inline]] constexpr auto _quintic_impl_(T&& v) noexcept
	{
		using scalar = ::std::remove_cvref_t<T>;
		return v * v * v * (v * (v * static_cast<scalar>(6) - static_cast<scalar>(15)) + static_cast<scalar>(10));
	}

	template <typename T>
	[[nodiscard]] [[rythe_always_inline]] constexpr auto _quintic_derivative_impl_(T&& v) noexcept
	{
		using scalar = ::std::remove_cvref_t<T>;
		const auto sqr = v - static_cast<scalar>(1);
		return static_cast<scalar>(30) * sqr * sqr * v * v;
	}

	template <typename T>
	[[nodiscard]] [[rythe_always_inline]] auto _inverse_quintic_impl_(T&& v) noexcept
	{
		using scalar = ::std::remove_cvref_t<T>;

		// TODO: find an inverse with a smaller average error.
		return static_cast<scalar>(0.5) -
			   sin(asin(
					   sin(asin(static_cast<scalar>(1) - static_cast<scalar>(2) * v) * static_cast<scalar>(0.032)) *
					   static_cast<scalar>(7.62)
				   ) *
				   static_cast<scalar>(0.63)) *
				   static_cast<scalar>(1.78);

		// Numerical integration
		constexpr size_type stepCount = 10;

		scalar pos = static_cast<scalar>(0.5);
		auto offsetFromCenter = v - pos;
		auto stepSize = offsetFromCenter / static_cast<scalar>(stepCount);

		scalar result = 0;
		for (size_type i = 0; i < stepCount; i++)
		{
			result += _inverse_quintic_derivative_impl_(pos) *
					  static_cast<scalar>(stepSize < static_cast<scalar>(0) ? -1 : 1);
			pos += stepSize;
		}
		return result;
	}

	template <typename T>
	[[nodiscard]] [[rythe_always_inline]] constexpr auto _inverse_quintic_derivative_impl_(T&& v) noexcept
	{
		using scalar = ::std::remove_cvref_t<T>;
		constexpr auto sqr = static_cast<scalar>(1) - v;
		return static_cast<scalar>(1) / (static_cast<scalar>(30) * sqr * sqr * v * v);
	}
} // namespace rsl::math::internal

#include "quintic_vector.inl"
// #include "quintic_matrix.inl"
// #include "quintic_quaternion.inl"

namespace rsl::math
{
	// quintic
	template <typename TypeMin, typename TypeMax, typename ValueType>
	[[nodiscard]] constexpr auto quintic(TypeMin&& edgeMin, TypeMax&& edgeMax, ValueType&& value) noexcept
	{
		using A = ::std::remove_cvref_t<TypeMin>;
		using B = ::std::remove_cvref_t<TypeMax>;
		using T = ::std::remove_cvref_t<ValueType>;

		if constexpr (is_quat_v<A> && is_quat_v<B> && is_quat_v<T>)
		{
			using scalar = elevated_t<typename A::scalar, elevated_t<typename B::scalar, typename T::scalar>>;
			return internal::compute_quintic<quaternion<scalar>>::compute(
				forward<TypeMin>(edgeMin), forward<TypeMax>(edgeMax), forward<ValueType>(value)
			);
		}
		if constexpr (is_matrix_v<A> && is_matrix_v<B> && is_matrix_v<T>)
		{
			using scalar = elevated_t<typename A::scalar, elevated_t<typename B::scalar, typename T::scalar>>;
			return internal::compute_quintic<matrix<scalar, A::row_count, A::col_count>>::compute(
				forward<TypeMin>(edgeMin), forward<TypeMax>(edgeMax), forward<ValueType>(value)
			);
		}
		if constexpr (is_vector_v<A> && is_vector_v<B> && is_vector_v<T>)
		{
			using scalar = elevated_t<typename A::scalar, elevated_t<typename B::scalar, typename T::scalar>>;
			constexpr size_type size = min(A::size, min(B::size, T::size))));
			return internal::compute_quintic<vector<scalar, size>>::compute(
				forward<TypeMin>(edgeMin), forward<TypeMax>(edgeMax), forward<ValueType>(value)
			);
		}
		else
		{
			return internal::_quintic_impl_(
				saturate(map01(forward<ValueType>(value), forward<TypeMin>(edgeMin), forward<TypeMax>(edgeMax)))
			);
		}
	}

	// quintic
	template <typename ValueType>
	[[nodiscard]] constexpr auto quintic(ValueType&& value) noexcept
	{
		using T = ::std::remove_cvref_t<ValueType>;

		if constexpr (is_quat_v<T>)
		{
			return internal::compute_quintic<quaternion<typename T::scalar>>::compute(forward<ValueType>(value));
		}
		if constexpr (is_matrix_v<T>)
		{
			return internal::compute_quintic<matrix<typename T::scalar, T::row_count, T::col_count>>::compute(
				forward<ValueType>(value)
			);
		}
		if constexpr (is_vector_v<T>)
		{
			return internal::compute_quintic<vector<typename T::scalar, T::size>>::compute(forward<ValueType>(value));
		}
		else
		{
			return internal::_quintic_impl_(forward<ValueType>(value));
		}
	}

	// quintic_derivative
	template <typename TypeMin, typename TypeMax, typename ValueType>
	[[nodiscard]] constexpr auto quintic_derivative(TypeMin&& edgeMin, TypeMax&& edgeMax, ValueType&& value) noexcept
	{
		using A = ::std::remove_cvref_t<TypeMin>;
		using B = ::std::remove_cvref_t<TypeMax>;
		using T = ::std::remove_cvref_t<ValueType>;

		if constexpr (is_quat_v<A> && is_quat_v<B> && is_quat_v<T>)
		{
			using scalar = elevated_t<typename A::scalar, elevated_t<typename B::scalar, typename T::scalar>>;
			return internal::compute_quintic<quaternion<scalar>>::compute_derivative(
				forward<TypeMin>(edgeMin), forward<TypeMax>(edgeMax), forward<ValueType>(value)
			);
		}
		if constexpr (is_matrix_v<A> && is_matrix_v<B> && is_matrix_v<T>)
		{
			using scalar = elevated_t<typename A::scalar, elevated_t<typename B::scalar, typename T::scalar>>;
			return internal::compute_quintic<matrix<scalar, A::row_count, A::col_count>>::compute_derivative(
				forward<TypeMin>(edgeMin), forward<TypeMax>(edgeMax), forward<ValueType>(value)
			);
		}
		if constexpr (is_vector_v<A> && is_vector_v<B> && is_vector_v<T>)
		{
			using scalar = elevated_t<typename A::scalar, elevated_t<typename B::scalar, typename T::scalar>>;
			constexpr size_type size = min(A::size, min(B::size, T::size))));
			return internal::compute_quintic<vector<scalar, size>>::compute_derivative(
				forward<TypeMin>(edgeMin), forward<TypeMax>(edgeMax), forward<ValueType>(value)
			);
		}
		else
		{
			return internal::_quintic_derivative_impl_(
				saturate(map01(forward<ValueType>(value), forward<TypeMin>(edgeMin), forward<TypeMax>(edgeMax)))
			);
		}
	}

	// quintic_derivative
	template <typename ValueType>
	[[nodiscard]] constexpr auto quintic_derivative(ValueType&& value) noexcept
	{
		using T = ::std::remove_cvref_t<ValueType>;

		if constexpr (is_quat_v<T>)
		{
			return internal::compute_quintic<quaternion<typename T::scalar>>::compute_derivative(
				forward<ValueType>(value)
			);
		}
		if constexpr (is_matrix_v<T>)
		{
			return internal::compute_quintic<
				matrix<typename T::scalar, T::row_count, T::col_count>>::compute_derivative(forward<ValueType>(value));
		}
		if constexpr (is_vector_v<T>)
		{
			return internal::compute_quintic<vector<typename T::scalar, T::size>>::compute_derivative(
				forward<ValueType>(value)
			);
		}
		else
		{
			return internal::_quintic_derivative_impl_(forward<ValueType>(value));
		}
	}

	// inverse_quintic
	template <typename TypeMin, typename TypeMax, typename ValueType>
	[[nodiscard]] auto inverse_quintic(TypeMin&& edgeMin, TypeMax&& edgeMax, ValueType&& value) noexcept
	{
		using A = ::std::remove_cvref_t<TypeMin>;
		using B = ::std::remove_cvref_t<TypeMax>;
		using T = ::std::remove_cvref_t<ValueType>;

		if constexpr (is_quat_v<A> && is_quat_v<B> && is_quat_v<T>)
		{
			using scalar = elevated_t<typename A::scalar, elevated_t<typename B::scalar, typename T::scalar>>;
			return internal::compute_quintic<quaternion<scalar>>::compute_inverse(
				forward<TypeMin>(edgeMin), forward<TypeMax>(edgeMax), forward<ValueType>(value)
			);
		}
		if constexpr (is_matrix_v<A> && is_matrix_v<B> && is_matrix_v<T>)
		{
			using scalar = elevated_t<typename A::scalar, elevated_t<typename B::scalar, typename T::scalar>>;
			return internal::compute_quintic<matrix<scalar, A::row_count, A::col_count>>::compute_inverse(
				forward<TypeMin>(edgeMin), forward<TypeMax>(edgeMax), forward<ValueType>(value)
			);
		}
		if constexpr (is_vector_v<A> && is_vector_v<B> && is_vector_v<T>)
		{
			using scalar = elevated_t<typename A::scalar, elevated_t<typename B::scalar, typename T::scalar>>;
			constexpr size_type size = min(A::size, min(B::size, T::size))));
			return internal::compute_quintic<vector<scalar, size>>::compute_inverse(
				forward<TypeMin>(edgeMin), forward<TypeMax>(edgeMax), forward<ValueType>(value)
			);
		}
		else
		{
			return internal::_inverse_quintic_impl_(
				saturate(map01(forward<ValueType>(value), forward<TypeMin>(edgeMin), forward<TypeMax>(edgeMax)))
			);
		}
	}

	// inverse_quintic
	template <typename ValueType>
	[[nodiscard]] auto inverse_quintic(ValueType&& value) noexcept
	{
		using T = ::std::remove_cvref_t<ValueType>;

		if constexpr (is_quat_v<T>)
		{
			return internal::compute_quintic<quaternion<typename T::scalar>>::compute_inverse(forward<ValueType>(value)
			);
		}
		if constexpr (is_matrix_v<T>)
		{
			return internal::compute_quintic<matrix<typename T::scalar, T::row_count, T::col_count>>::compute_inverse(
				forward<ValueType>(value)
			);
		}
		if constexpr (is_vector_v<T>)
		{
			return internal::compute_quintic<vector<typename T::scalar, T::size>>::compute_inverse(
				forward<ValueType>(value)
			);
		}
		else
		{
			return internal::_inverse_quintic_impl_(forward<ValueType>(value));
		}
	}

	// inverse_quintic_derivative
	template <typename TypeMin, typename TypeMax, typename ValueType>
	[[nodiscard]] auto inverse_quintic_derivative(TypeMin&& edgeMin, TypeMax&& edgeMax, ValueType&& value) noexcept
	{
		using A = ::std::remove_cvref_t<TypeMin>;
		using B = ::std::remove_cvref_t<TypeMax>;
		using T = ::std::remove_cvref_t<ValueType>;

		if constexpr (is_quat_v<A> && is_quat_v<B> && is_quat_v<T>)
		{
			using scalar = elevated_t<typename A::scalar, elevated_t<typename B::scalar, typename T::scalar>>;
			return internal::compute_quintic<quaternion<scalar>>::compute_inverse_derivative(
				forward<TypeMin>(edgeMin), forward<TypeMax>(edgeMax), forward<ValueType>(value)
			);
		}
		if constexpr (is_matrix_v<A> && is_matrix_v<B> && is_matrix_v<T>)
		{
			using scalar = elevated_t<typename A::scalar, elevated_t<typename B::scalar, typename T::scalar>>;
			return internal::compute_quintic<matrix<scalar, A::row_count, A::col_count>>::compute_inverse_derivative(
				forward<TypeMin>(edgeMin), forward<TypeMax>(edgeMax), forward<ValueType>(value)
			);
		}
		if constexpr (is_vector_v<A> && is_vector_v<B> && is_vector_v<T>)
		{
			using scalar = elevated_t<typename A::scalar, elevated_t<typename B::scalar, typename T::scalar>>;
			constexpr size_type size = min(A::size, min(B::size, T::size))));
			return internal::compute_quintic<vector<scalar, size>>::compute_inverse_derivative(
				forward<TypeMin>(edgeMin), forward<TypeMax>(edgeMax), forward<ValueType>(value)
			);
		}
		else
		{
			return internal::_inverse_quintic_derivative_impl_(
				saturate(map01(forward<ValueType>(value), forward<TypeMin>(edgeMin), forward<TypeMax>(edgeMax)))
			);
		}
	}

	// inverse_quintic_derivative
	template <typename ValueType>
	[[nodiscard]] auto inverse_quintic_derivative(ValueType&& value) noexcept
	{
		using T = ::std::remove_cvref_t<ValueType>;

		if constexpr (is_quat_v<T>)
		{
			return internal::compute_quintic<quaternion<typename T::scalar>>::compute_inverse_derivative(
				forward<ValueType>(value)
			);
		}
		if constexpr (is_matrix_v<T>)
		{
			return internal::compute_quintic<matrix<typename T::scalar, T::row_count, T::col_count>>::
				compute_inverse_derivative(forward<ValueType>(value));
		}
		if constexpr (is_vector_v<T>)
		{
			return internal::compute_quintic<vector<typename T::scalar, T::size>>::compute_inverse_derivative(
				forward<ValueType>(value)
			);
		}
		else
		{
			return internal::_inverse_quintic_derivative_impl_(forward<ValueType>(value));
		}
	}
} // namespace rsl::math

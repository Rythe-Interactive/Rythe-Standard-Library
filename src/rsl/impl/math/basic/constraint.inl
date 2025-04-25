#pragma once
#include "constraint.hpp"

#include "constraint_vector.inl"

namespace rsl::math
{
	template <typename A, typename B>
	[[nodiscard]] constexpr auto min(A&& a, B&& b) noexcept
	{
		if constexpr (is_vector_v<A> && is_vector_v<B>)
		{
			using AType = remove_cvr_t<A>;
			using BType = remove_cvr_t<B>;
			using scalar = elevated_t<typename AType::scalar, typename BType::scalar>;
			constexpr size_type size = AType::size < BType::size ? AType::size : BType::size;
			return internal::compute_constraint<vector<scalar, size>>::compute_min(forward<A>(a), forward<B>(b));
		}
		else if constexpr (is_vector_v<A> || is_vector_v<B>)
		{
			return internal::compute_constraint<select_vector_type_t<A, B>>::compute_min(forward<A>(a), forward<B>(b));
		}
		else
		{
			using value_type = elevated_t<decay_vector_t<A>, decay_vector_t<B>>;
			value_type decayedA = static_cast<value_type>(static_cast<decay_vector_t<A>>(a));
			value_type decayedB = static_cast<value_type>(static_cast<decay_vector_t<B>>(b));
			return decayedA < decayedB ? decayedA : decayedB;
		}
	}

	template <typename A, typename B>
	[[nodiscard]] constexpr auto max(A&& a, B&& b) noexcept
	{
		if constexpr (is_vector_v<A> && is_vector_v<B>)
		{
			using AType = remove_cvr_t<A>;
			using BType = remove_cvr_t<B>;
			using scalar = elevated_t<typename AType::scalar, typename BType::scalar>;
			constexpr size_type size = AType::size < BType::size ? AType::size : BType::size;
			return internal::compute_constraint<vector<scalar, size>>::compute_max(forward<A>(a), forward<B>(b));
		}
		else if constexpr (is_vector_v<A> || is_vector_v<B>)
		{
			return internal::compute_constraint<select_vector_type_t<A, B>>::compute_max(forward<A>(a), forward<B>(b));
		}
		else
		{
			using value_type = elevated_t<decay_vector_t<A>, decay_vector_t<B>>;
			value_type decayedA = static_cast<value_type>(static_cast<decay_vector_t<A>>(a));
			value_type decayedB = static_cast<value_type>(static_cast<decay_vector_t<B>>(b));
			return decayedA > decayedB ? decayedA : decayedB;
		}
	}

	template <typename In, typename Min, typename Max>
	[[nodiscard]] constexpr auto clamp(In&& in, Min&& min, Max&& max) noexcept
	{
		if constexpr (is_vector_v<In> && is_vector_v<Min> && is_vector_v<Max>)
		{
			using InType = remove_cvr_t<In>;
			using MinType = remove_cvr_t<Min>;
			using MaxType = remove_cvr_t<Max>;
			using scalar =
				elevated_t<typename InType::scalar, elevated_t<typename MinType::scalar, typename MaxType::scalar>>;
			constexpr size_type inMinSize = InType::size < MinType::size ? InType::size : MinType::size;
			constexpr size_type size = MaxType::size < inMinSize ? MaxType::size : inMinSize;
			return internal::compute_constraint<vector<scalar, size>>::compute_clamp(
				forward<In>(in), forward<Min>(min), forward<Max>(max)
			);
		}
		else if constexpr (is_vector_v<In> && is_vector_v<Min>)
		{
			using InType = remove_cvr_t<In>;
			using MinType = remove_cvr_t<Min>;
			using scalar = elevated_t<typename InType::scalar, elevated_t<typename MinType::scalar, Max>>;
			constexpr size_type size = InType::size < MinType::size ? InType::size : MinType::size;
			return internal::compute_constraint<vector<scalar, size>>::compute_clamp(
				forward<In>(in), forward<Min>(min), forward<Max>(max)
			);
		}
		else if constexpr (is_vector_v<In> && is_vector_v<Max>)
		{
			using InType = remove_cvr_t<In>;
			using MaxType = remove_cvr_t<Max>;
			using scalar = elevated_t<typename InType::scalar, elevated_t<Min, typename MaxType::scalar>>;
			constexpr size_type size = InType::size < MaxType::size ? InType::size : MaxType::size;
			return internal::compute_constraint<vector<scalar, size>>::compute_clamp(
				forward<In>(in), forward<Min>(min), forward<Max>(max)
			);
		}
		else if constexpr (is_vector_v<Min> && is_vector_v<Max>)
		{
			using MinType = remove_cvr_t<Min>;
			using MaxType = remove_cvr_t<Max>;
			using scalar = elevated_t<In, elevated_t<typename MinType::scalar, typename MaxType::scalar>>;
			constexpr size_type size = MinType::size < MaxType::size ? MinType::size : MaxType::size;
			return internal::compute_constraint<vector<scalar, size>>::compute_clamp(
				forward<In>(in), forward<Min>(min), forward<Max>(max)
			);
		}
		else if constexpr (is_vector_v<In> || is_vector_v<Min> || is_vector_v<Max>)
		{
			return internal::compute_constraint<select_vector_type_t<In, select_vector_type_t<Min, Max>>>::
				compute_clamp(forward<In>(in), forward<Min>(min), forward<Max>(max));
		}
		else
		{
			using value_type = elevated_t<decay_vector_t<In>, elevated_t<decay_vector_t<Min>, decay_vector_t<Max>>>;
			value_type decayedIn = static_cast<value_type>(static_cast<decay_vector_t<In>>(in));
			value_type decayedMin = static_cast<value_type>(static_cast<decay_vector_t<Min>>(min));
			value_type decayedMax = static_cast<value_type>(static_cast<decay_vector_t<Max>>(max));
			return decayedIn < decayedMin ? decayedMin : (decayedIn > decayedMax ? decayedMax : decayedIn);
		}
	}

	template <typename In>
	[[nodiscard]] constexpr auto saturate(In&& in) noexcept
	{
		using value_type = remove_cvr_t<In>;
		if constexpr (is_vector_v<value_type>)
		{
			using scalar = typename value_type::scalar;
			return internal::compute_constraint<make_vector_t<value_type>>::compute_clamp(
				in, static_cast<scalar>(0), static_cast<scalar>(1)
			);
		}
		else
		{
			return in < static_cast<value_type>(0)
					   ? static_cast<value_type>(0)
					   : (in > static_cast<value_type>(1) ? static_cast<value_type>(1) : static_cast<value_type>(in));
		}
	}
} // namespace rsl::math

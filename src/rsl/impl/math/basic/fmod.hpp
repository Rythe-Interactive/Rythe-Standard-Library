#pragma once
#include <cmath>
#include <limits>

#include "../../util/primitives.hpp"
#include "../util/type_util.hpp"

namespace rsl::math
{
	namespace internal
	{
		template <typename Scalar, size_type Size>
		struct compute_mod
		{
			static constexpr size_type size = Size;
			using value_type = vector<Scalar, size>;

			[[nodiscard]] [[rythe_always_inline]] static value_type computef(const value_type& val, Scalar m) noexcept
			{
				value_type result;
				for (size_type i = 0; i < size; i++)
				{
					result[i] = ::std::fmod(val[i], m);
				}
				return result;
			}

			[[nodiscard]] [[rythe_always_inline]] static value_type
			computef(const value_type& val, const value_type& m) noexcept
			{
				value_type result;
				for (size_type i = 0; i < size; i++)
				{
					result[i] = ::std::fmod(val[i], m[i]);
				}
				return result;
			}

			[[nodiscard]] constexpr static value_type compute(const value_type& val, Scalar m) noexcept
			{
				value_type result;
				for (size_type i = 0; i < size; i++)
				{
					result[i] = val[i] % m;
				}
				return result;
			}

			[[nodiscard]] constexpr static value_type compute(const value_type& val, const value_type& m) noexcept
			{
				value_type result;
				for (size_type i = 0; i < size; i++)
				{
					result[i] = val[i] % m[i];
				}
				return result;
			}
		};

		template <typename Scalar>
		struct compute_mod<Scalar, 1u>
		{
			static constexpr size_type size = 1u;
			using value_type = vector<Scalar, size>;

			[[nodiscard]] [[rythe_always_inline]] static Scalar computef(Scalar val, Scalar m) noexcept
			{
				return ::std::fmod(val, m);
			}

			[[nodiscard]] constexpr static Scalar compute(Scalar val, Scalar m) noexcept { return val % m; }
		};
	} // namespace internal

	template <typename T>
	[[nodiscard]] [[rythe_always_inline]] static auto fmod(const T& val, const T& m)
	{
		if constexpr (is_vector_v<T>)
		{
			static_assert(
				::std::is_floating_point_v<typename T::scalar>,
				"Value must be floating point in order to use fmod, use mod instead."
			);
			return internal::compute_mod<typename T::scalar, T::size>::computef(val, m);
		}
		else
		{
			static_assert(
				::std::is_floating_point_v<T>, "Value must be floating point in order to use fmod, use mod instead."
			);
			return ::std::fmod(val, m);
		}
	}

	template <typename vec_type, std::enable_if_t<is_vector_v<vec_type>, bool> = true>
	[[nodiscard]] [[rythe_always_inline]] static auto fmod(const vec_type& val, typename vec_type::scalar m)
	{
		static_assert(
			::std::is_floating_point_v<typename vec_type::scalar>,
			"Value must be floating point in order to use fmod, use mod instead."
		);
		return internal::compute_mod<typename vec_type::scalar, vec_type::size>::computef(val, m);
	}

	template <typename T>
	[[nodiscard]] [[rythe_always_inline]] static auto mod(const T& val, const T& m)
	{
		if constexpr (is_vector_v<T>)
		{
			if constexpr (::std::is_floating_point_v<typename T::scalar>)
			{
				return internal::compute_mod<typename T::scalar, T::size>::computef(val, m);
			}
			else
			{
				return internal::compute_mod<typename T::scalar, T::size>::compute(val, m);
			}
		}
		else if constexpr (::std::is_floating_point_v<T>)
		{
			return ::std::fmod(val, m);
		}
		else
		{
			return val % m;
		}
	}

	template <typename vec_type, std::enable_if_t<is_vector_v<vec_type>, bool> = true>
	[[nodiscard]] [[rythe_always_inline]] static auto mod(const vec_type& val, typename vec_type::scalar m)
	{
		if constexpr (::std::is_floating_point_v<typename vec_type::scalar>)
		{
			return internal::compute_mod<typename vec_type::scalar, vec_type::size>::computef(val, m);
		}
		else
		{
			return internal::compute_mod<typename vec_type::scalar, vec_type::size>::compute(val, m);
		}
	}

	template <typename vec_type, std::enable_if_t<is_vector_v<vec_type>, bool> = true>
	[[nodiscard]] [[rythe_always_inline]] auto operator%(const vec_type& val, typename vec_type::scalar m)
	{
		return mod(val, m);
	}

	template <typename vec_type, std::enable_if_t<is_vector_v<vec_type>, bool> = true>
	[[nodiscard]] [[rythe_always_inline]] auto operator%(const vec_type& val, const vec_type& m)
	{
		return mod(val, m);
	}
} // namespace rsl::math

#pragma once
#include <cmath>

#include "../../util/primitives.hpp"
#include "../vector/vector.hpp"

namespace rsl::math
{
	namespace internal
	{
		template <typename Scalar, size_type Size>
		struct compute_sqrt
		{
			static constexpr size_type size = Size;
			using value_type = vector<Scalar, size>;

			[[rythe_always_inline]] static value_type compute(const value_type& v) noexcept
			{
				value_type result;
				for (size_type i; i < size; i++)
				{
					result[i] = ::std::sqrt(v[i]);
				}
				return result;
			}
		};

		template <typename Scalar>
		struct compute_sqrt<Scalar, 1u>
		{
			static constexpr size_type size = 1u;
			using value_type = vector<Scalar, size>;

			[[rythe_always_inline]] static Scalar compute(Scalar v) noexcept { return ::std::sqrt(v); }
		};

		template <typename Scalar>
		struct compute_sqrt<Scalar, 2u>
		{
			static constexpr size_type size = 2u;
			using value_type = vector<Scalar, size>;

			[[rythe_always_inline]] static value_type compute(const value_type& v) noexcept
			{
				return value_type{::std::sqrt(v[0]), ::std::sqrt(v[1])};
			}
		};

		template <typename Scalar>
		struct compute_sqrt<Scalar, 3u>
		{
			static constexpr size_type size = 3u;
			using value_type = vector<Scalar, size>;

			[[rythe_always_inline]] static value_type compute(const value_type& v) noexcept
			{
				return value_type{::std::sqrt(v[0]), ::std::sqrt(v[1]), ::std::sqrt(v[2])};
			}
		};

		template <typename Scalar>
		struct compute_sqrt<Scalar, 4u>
		{
			static constexpr size_type size = 4u;
			using value_type = vector<Scalar, size>;

			[[rythe_always_inline]] static value_type compute(const value_type& v) noexcept
			{
				return value_type{::std::sqrt(v[0]), ::std::sqrt(v[1]), ::std::sqrt(v[2]), ::std::sqrt(v[3])};
			}
		};
	} // namespace internal

	template <typename vec_type, ::std::enable_if_t<is_vector_v<vec_type>, bool> = true>
	[[rythe_always_inline]] static auto sqrt(const vec_type& v) noexcept
	{
		return internal::compute_sqrt<typename vec_type::scalar, vec_type::size>::compute(v);
	}

	template <typename Scalar>
	[[rythe_always_inline]] static auto sqrt(Scalar v) noexcept
	{
		return internal::compute_sqrt<Scalar, 1>::compute(v);
	}
} // namespace rsl::math

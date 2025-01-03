#pragma once
#include <cmath>

#include "../../util/primitives.hpp"
#include "../vector/vector.hpp"

namespace rsl::math
{
	namespace internal
	{
		template <typename Scalar, size_type Size>
		struct compute_inv_sqrt
		{
			static constexpr size_type size = Size;
			using value_type = vector<Scalar, size>;

			[[rythe_always_inline]] static value_type compute(const value_type& v) noexcept
			{
				value_type result;
				for (size_type i; i < size; i++)
				{
					result[i] = static_cast<Scalar>(1) / ::std::sqrt(v[i]);
				}
				return result;
			}
		};

		template <typename Scalar>
		struct compute_inv_sqrt<Scalar, 1u>
		{
			static constexpr size_type size = 1u;
			using value_type = vector<Scalar, size>;

			[[rythe_always_inline]] static Scalar compute(Scalar v) noexcept
			{
				return static_cast<Scalar>(1) / ::std::sqrt(v);
			}
		};

		template <typename Scalar>
		struct compute_inv_sqrt<Scalar, 2u>
		{
			static constexpr size_type size = 2u;
			using value_type = vector<Scalar, size>;

			[[rythe_always_inline]] static value_type compute(const value_type& v) noexcept
			{
				return value_type{
					static_cast<Scalar>(1) / ::std::sqrt(v[0]), static_cast<Scalar>(1) / ::std::sqrt(v[1])
				};
			}
		};

		template <typename Scalar>
		struct compute_inv_sqrt<Scalar, 3u>
		{
			static constexpr size_type size = 3u;
			using value_type = vector<Scalar, size>;

			[[rythe_always_inline]] static value_type compute(const value_type& v) noexcept
			{
				return value_type{
					static_cast<Scalar>(1) / ::std::sqrt(v[0]), static_cast<Scalar>(1) / ::std::sqrt(v[1]),
					static_cast<Scalar>(1) / ::std::sqrt(v[2])
				};
			}
		};

		template <typename Scalar>
		struct compute_inv_sqrt<Scalar, 4u>
		{
			static constexpr size_type size = 4u;
			using value_type = vector<Scalar, size>;

			[[rythe_always_inline]] static value_type compute(const value_type& v) noexcept
			{
				return value_type{
					static_cast<Scalar>(1) / ::std::sqrt(v[0]), static_cast<Scalar>(1) / ::std::sqrt(v[1]),
					static_cast<Scalar>(1) / ::std::sqrt(v[2]), static_cast<Scalar>(1) / ::std::sqrt(v[3])
				};
			}
		};
	} // namespace internal

	template <typename vec_type, ::std::enable_if_t<is_vector_v<vec_type>, bool> = true>
	[[rythe_always_inline]] static auto inverse_sqrt(const vec_type& v) noexcept
	{
		return internal::compute_inv_sqrt<typename vec_type::scalar, vec_type::size>::compute(v);
	}

	template <typename Scalar>
	[[rythe_always_inline]] static Scalar inverse_sqrt(Scalar v) noexcept
	{
		return internal::compute_inv_sqrt<Scalar, 1>::compute(v);
	}
} // namespace rsl::math

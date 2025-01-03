#pragma once
#include "../../util/primitives.hpp"
#include "../util/type_util.hpp"
#include "../vector/vector.hpp"

namespace rsl::math
{
	namespace internal
	{
		template <arithmetic_type Scalar, size_type Size, mode Mode>
		struct compute_addition
		{
			static constexpr size_type size = Size;
			using value_type = vector<Scalar, size, Mode>;

			[[nodiscard]] [[rythe_always_inline]] constexpr static value_type
			compute(const value_type& a, const value_type& b) noexcept
			{
				value_type result;
				for (size_type i = 0; i < size; i++)
				{
					result[i] = a[i] + b[i];
				}
				return result;
			}

			[[nodiscard]] [[rythe_always_inline]] constexpr static value_type
			compute(const value_type& a, Scalar b) noexcept
			{
				value_type result;
				for (size_type i = 0; i < size; i++)
				{
					result[i] = a[i] + b;
				}
				return result;
			}
		};

		template <arithmetic_type Scalar, mode Mode>
		struct compute_addition<Scalar, 1u, Mode>
		{
			static constexpr size_type size = 1u;
			using value_type = vector<Scalar, size, Mode>;

			[[nodiscard]] [[rythe_always_inline]] constexpr static Scalar compute(Scalar a, Scalar b) noexcept
			{
				return a + b;
			}
		};

		template <typename T>
		struct compute_quat_addition;

		template <arithmetic_type Scalar, mode Mode>
		struct compute_quat_addition<quaternion<Scalar, Mode>>
		{
			using value_type = quaternion<Scalar>;

			[[nodiscard]] [[rythe_always_inline]] constexpr static auto
			compute(const value_type& a, const value_type& b) noexcept
			{
				value_type result;
				result.i = a.i + b.i;
				result.j = a.j + b.j;
				result.k = a.k + b.k;
				result.w = a.w + b.w;
				return result;
			}
		};
	} // namespace internal

	template <vector_type vec_type0, vector_type vec_type1>
	[[nodiscard]] [[rythe_always_inline]] constexpr auto add(const vec_type0& a, const vec_type1& b) noexcept
	{
		return internal::compute_addition<typename vec_type0::scalar, vec_type0::size, vec_type0::mode>::compute(a, b);
	}

	template <vector_type vec_type0, vector_type vec_type1>
	[[nodiscard]] [[rythe_always_inline]] constexpr auto operator+(const vec_type0& a, const vec_type1& b) noexcept
	{
		return internal::compute_addition<typename vec_type0::scalar, vec_type0::size, vec_type0::mode>::compute(a, b);
	}

	template <vector_type vec_type>
	[[nodiscard]] [[rythe_always_inline]] constexpr auto add(const vec_type& a, typename vec_type::scalar b) noexcept
	{
		return internal::compute_addition<typename vec_type::scalar, vec_type::size, vec_type::mode>::compute(a, b);
	}

	template <vector_type vec_type>
	[[nodiscard]] [[rythe_always_inline]] constexpr auto
	operator+(const vec_type& a, typename vec_type::scalar b) noexcept
	{
		return internal::compute_addition<typename vec_type::scalar, vec_type::size, vec_type::mode>::compute(a, b);
	}

	template <quat_type quat_type0, quat_type quat_type1>
	[[nodiscard]] [[rythe_always_inline]] constexpr auto add(const quat_type0& a, const quat_type1& b) noexcept
	{
		return internal::compute_quat_addition<quat_type0>::compute(a, b);
	}
	template <quat_type quat_type0, quat_type quat_type1>
	[[nodiscard]] [[rythe_always_inline]] constexpr auto operator+(const quat_type0& a, const quat_type1& b) noexcept
	{
		return internal::compute_quat_addition<quat_type0>::compute(a, b);
	}

	template <vector_type vec_type0, vector_type vec_type1>
	[[rythe_always_inline]] constexpr vec_type0& add_assign(vec_type0& a, const vec_type1& b) noexcept
	{
		return a = internal::compute_addition<typename vec_type0::scalar, vec_type0::size, vec_type0::mode>::compute(
				   a, b
			   );
	}

	template <vector_type vec_type0, vector_type vec_type1>
	[[rythe_always_inline]] constexpr vec_type0& operator+=(vec_type0& a, const vec_type1& b) noexcept
	{
		return a = internal::compute_addition<typename vec_type0::scalar, vec_type0::size, vec_type0::mode>::compute(
				   a, b
			   );
	}

	template <vector_type vec_type>
	[[rythe_always_inline]] constexpr vec_type& add_assign(vec_type& a, typename vec_type::scalar b) noexcept
	{
		return a = internal::compute_addition<typename vec_type::scalar, vec_type::size, vec_type::mode>::compute(a, b);
	}

	template <vector_type vec_type>
	[[rythe_always_inline]] constexpr vec_type& operator+=(vec_type& a, typename vec_type::scalar b) noexcept
	{
		return a = internal::compute_addition<typename vec_type::scalar, vec_type::size, vec_type::mode>::compute(a, b);
	}
} // namespace rsl::math

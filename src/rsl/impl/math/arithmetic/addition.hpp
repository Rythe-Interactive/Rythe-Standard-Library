#pragma once
#include "../../util/primitives.hpp"
#include "../vector/vector.hpp"
#include "../util/type_util.hpp"

namespace rsl::math
{
	namespace detail
	{
		template<typename Scalar, size_type Size>
		struct compute_addition
		{
			static constexpr size_type size = Size;
			using value_type = vector<Scalar, size>;

			[[nodiscard]] constexpr static value_type compute(const value_type& a, const value_type& b) noexcept
			{
				value_type result;
				for (size_type i = 0; i < size; i++)
					result[i] = a[i] + b[i];
				return result;
			}

			[[nodiscard]] constexpr static value_type compute(const value_type& a, Scalar b) noexcept
			{
				value_type result;
				for (size_type i = 0; i < size; i++)
					result[i] = a[i] + b;
				return result;
			}
		};

		template<typename Scalar>
		struct compute_addition<Scalar, 1u>
		{
			static constexpr size_type size = 1u;
			using value_type = vector<Scalar, size>;

			[[nodiscard]] constexpr static Scalar compute(Scalar a, Scalar b) noexcept
			{
				return a * b;
			}
		};


		template<typename T>
		struct compute_quat_addition;

		template<typename Scalar>
		struct compute_quat_addition<quaternion<Scalar>>
		{
			using quat_type = quaternion<Scalar>;

			template<typename AType, typename BType, ::std::enable_if_t<is_quat_v<AType>&& is_quat_v<BType>, bool> = true>
			[[nodiscard]] constexpr static auto compute(AType&& a, BType&& b) noexcept
			{
				quat_type result;
				result.i = a.i + b.i;
				result.j = a.j + b.j;
				result.k = a.k + b.k;
				result.w = a.w + b.w;
				return result;
			}
		};
	}

	template<typename vec_type0, typename vec_type1, std::enable_if_t<is_vector_v<vec_type0>&& is_vector_v<vec_type1>, bool> = true>
	[[nodiscard]] constexpr auto add(const vec_type0& a, const vec_type1& b) noexcept
	{
		return detail::compute_addition<typename vec_type0::scalar, vec_type0::size>::compute(a, b);
	}

	template<typename vec_type0, typename vec_type1, std::enable_if_t<is_vector_v<vec_type0>&& is_vector_v<vec_type1>, bool> = true>
	[[nodiscard]] constexpr auto operator+(const vec_type0& a, const vec_type1& b) noexcept
	{
		return detail::compute_addition<typename vec_type0::scalar, vec_type0::size>::compute(a, b);
	}

	template<typename vec_type, std::enable_if_t<is_vector_v<vec_type>, bool> = true>
	[[nodiscard]] constexpr auto add(const vec_type& a, typename vec_type::scalar b) noexcept
	{
		return detail::compute_addition<typename vec_type::scalar, vec_type::size>::compute(a, b);
	}

	template<typename vec_type, std::enable_if_t<is_vector_v<vec_type>, bool> = true>
	[[nodiscard]] constexpr auto operator+(const vec_type& a, typename vec_type::scalar b) noexcept
	{
		return detail::compute_addition<typename vec_type::scalar, vec_type::size>::compute(a, b);
	}

	template<typename quat_type0, typename quat_type1, std::enable_if_t<is_quat_v<quat_type0>&& is_quat_v<quat_type1>, bool> = true>
	[[nodiscard]] constexpr auto add(const quat_type0& a, const quat_type1& b) noexcept
	{
		return detail::compute_quat_addition<quaternion<typename quat_type0::scalar>>::compute(a, b);
	}
	template<typename quat_type0, typename quat_type1, std::enable_if_t<is_quat_v<quat_type0>&& is_quat_v<quat_type1>, bool> = true>
	[[nodiscard]] constexpr auto operator+(const quat_type0& a, const quat_type1& b) noexcept
	{
		return detail::compute_quat_addition<quaternion<typename quat_type0::scalar>>::compute(a, b);
	}


	template<typename vec_type0, typename vec_type1, std::enable_if_t<is_vector_v<vec_type0>&& is_vector_v<vec_type1>, bool> = true>
	constexpr vec_type0& add_assign(vec_type0& a, const vec_type1& b) noexcept
	{
		return a = detail::compute_addition<typename vec_type0::scalar, vec_type0::size>::compute(a, b);
	}

	template<typename vec_type0, typename vec_type1, std::enable_if_t<is_vector_v<vec_type0>&& is_vector_v<vec_type1>, bool> = true>
	constexpr vec_type0& operator+=(vec_type0& a, const vec_type1& b) noexcept
	{
		return a = detail::compute_addition<typename vec_type0::scalar, vec_type0::size>::compute(a, b);
	}

	template<typename vec_type, std::enable_if_t<is_vector_v<vec_type>, bool> = true>
	constexpr vec_type& add_assign(vec_type& a, typename vec_type::scalar b) noexcept
	{
		return a = detail::compute_addition<typename vec_type::scalar, vec_type::size>::compute(a, b);
	}

	template<typename vec_type, std::enable_if_t<is_vector_v<vec_type>, bool> = true>
	constexpr vec_type& operator+=(vec_type& a, typename vec_type::scalar b) noexcept
	{
		return a = detail::compute_addition<typename vec_type::scalar, vec_type::size>::compute(a, b);
	}

	template<typename quat_type0, typename quat_type1, std::enable_if_t<is_quat_v<quat_type0>&& is_quat_v<quat_type1>, bool> = true>
	constexpr quat_type0& add_assign(quat_type0& a, const quat_type1& b) noexcept
	{
		return a = detail::compute_quat_addition<quaternion<typename quat_type0::scalar>>::compute(a, b);
	}

	template<typename quat_type0, typename quat_type1, std::enable_if_t<is_quat_v<quat_type0>&& is_quat_v<quat_type1>, bool> = true>
	constexpr quat_type0& operator+=(quat_type0& a, const quat_type1& b) noexcept
	{
		return a = detail::compute_quat_addition<quaternion<typename quat_type0::scalar>>::compute(a, b);
	}
}

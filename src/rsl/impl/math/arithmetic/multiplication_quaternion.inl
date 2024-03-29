#pragma once
#include "../geometric/cross.hpp"
#include "../geometric/dot.hpp"
#include "multiplication.hpp"

namespace rsl::math::detail
{
	template <typename T>
	struct compute_multiplication;

	template <typename Scalar>
	struct compute_multiplication<quaternion<Scalar>>
	{
		using quat_type = quaternion<Scalar>;

		template <typename AType, typename BType, ::std::enable_if_t<is_quat_v<AType> && is_quat_v<BType>, bool> = true>
		[[nodiscard]] constexpr static auto compute(AType&& a, BType&& b) noexcept
		{
			quat_type result;
			result.vec = a.w * b.vec + static_cast<Scalar>(b.w) * a.vec + cross(a.vec, b.vec);
			result.w = a.w * static_cast<Scalar>(b.w) - dot(a.vec, b.vec);
			return result;
		}

		template <typename AType, typename BType, ::std::enable_if_t<is_vector_v<AType> && is_quat_v<BType>, bool> = true>
		[[nodiscard]] constexpr static auto compute(AType&& a, BType&& b) noexcept
		{
			using vec_type = make_vector_t<AType>;
			using scalar = typename vec_type::scalar;
			if constexpr (vec_type::size == 3)
			{
				return static_cast<scalar>(2) * dot(b.vec, a) * b.vec + (static_cast<scalar>(b.w) * static_cast<scalar>(b.w) - dot(b.vec, b.vec)) * a + static_cast<scalar>(2) * static_cast<scalar>(b.w) * cross(b.vec, a);
			}
			else if constexpr (vec_type::size == 4)
			{
				return vec_type(static_cast<scalar>(2) * dot(b.vec, a.xyz) * b.vec + (static_cast<scalar>(b.w) * static_cast<scalar>(b.w) - dot(b.vec, b.vec)) * a.xyz + static_cast<scalar>(2) * static_cast<scalar>(b.w) * cross(b.vec, a.xyz));
			}
			else
			{
				vector<scalar, 3> a3 = a;
				return vec_type(static_cast<scalar>(2) * dot(b.vec, a3) * b.vec + (static_cast<scalar>(b.w) * static_cast<scalar>(b.w) - dot(b.vec, b.vec)) * a3 + static_cast<scalar>(2) * static_cast<scalar>(b.w) * cross(b.vec, a3));
			}
		}
	};
} // namespace rsl::math::detail

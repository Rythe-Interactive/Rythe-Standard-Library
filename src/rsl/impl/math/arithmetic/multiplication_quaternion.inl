#pragma once
#include "../geometric/cross.hpp"
#include "../geometric/dot.hpp"
#include "addition.hpp"

namespace rsl::math::internal
{
	template <typename T>
	struct compute_multiplication;

	template <arithmetic_type Scalar, storage_mode Mode>
	struct compute_multiplication<quaternion<Scalar, Mode>>
	{
		using value_type = quaternion<Scalar, Mode>;

		[[nodiscard]] constexpr static auto compute(const value_type& a, const value_type& b) noexcept
		{
			value_type result;
			result.vec = a.w * b.vec + static_cast<Scalar>(b.w) * a.vec + cross(a.vec, b.vec);
			result.w = a.w * static_cast<Scalar>(b.w) - dot(a.vec, b.vec);
			return result;
		}

		template <vector_type VecType>
		[[nodiscard]] constexpr static auto compute(const VecType& a, const value_type& b) noexcept
		{
			if constexpr (VecType::size == 3)
			{
				return static_cast<Scalar>(2) * dot(b.vec, a) * b.vec +
					   (static_cast<Scalar>(b.w) * static_cast<Scalar>(b.w) - dot(b.vec, b.vec)) * a +
					   static_cast<Scalar>(2) * static_cast<Scalar>(b.w) * cross(b.vec, a);
			}
			else if constexpr (VecType::size == 4)
			{
				return VecType(
					static_cast<Scalar>(2) * dot(b.vec, a.xyz) * b.vec +
					(static_cast<Scalar>(b.w) * static_cast<Scalar>(b.w) - dot(b.vec, b.vec)) * a.xyz +
					static_cast<Scalar>(2) * static_cast<Scalar>(b.w) * cross(b.vec, a.xyz)
				);
			}
			else
			{
				vector<Scalar, 3, Mode> a3 = a;
				return vector<Scalar, 3, Mode>(
					static_cast<Scalar>(2) * dot(b.vec, a3) * b.vec +
					(static_cast<Scalar>(b.w) * static_cast<Scalar>(b.w) - dot(b.vec, b.vec)) * a3 +
					static_cast<Scalar>(2) * static_cast<Scalar>(b.w) * cross(b.vec, a3)
				);
			}
		}

		template <arithmetic_type ScalarType>
		[[nodiscard]] constexpr static auto compute(const value_type& a, ScalarType b) noexcept
		{
			value_type result;
			result.i = a.i * b;
			result.j = a.j * b;
			result.k = a.k * b;
			result.w = a.w * b;
			return result;
		}
	};
} // namespace rsl::math::internal

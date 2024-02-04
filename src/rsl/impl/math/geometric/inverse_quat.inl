#pragma once
#include "inverse.hpp"

namespace rsl::math::detail
{
	template <typename T>
	struct compute_inverse;

	template <typename Scalar>
	struct compute_inverse<quaternion<Scalar>>
	{
		using quat_type = quaternion<Scalar>;

		[[nodiscard]] rythe_always_inline static quat_type compute(const quat_type& q) noexcept
		{
			// Assuming we never work with non-normalized quaternions.
			return quat_type(q.w, -q.vec);
		}
	};
} // namespace rsl::math::detail

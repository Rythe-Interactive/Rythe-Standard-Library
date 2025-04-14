#pragma once

namespace rsl::math::internal
{
	template <typename T>
	struct compute_length;

	template <typename Scalar>
	struct compute_length<quaternion<Scalar>>
	{
		using quat_type = quaternion<Scalar>;
		using vec_type = vector<Scalar, 4>;

		[[nodiscard]] [[rythe_always_inline]] static Scalar compute(const quat_type& q) noexcept
		{
			vec_type v(q.w, q.i, q.j, q.k);
			return ::std::sqrt(dot(v, v));
		}

		[[nodiscard]] [[rythe_always_inline]] constexpr static Scalar compute2(const quat_type& q) noexcept
		{
			vec_type v(q.w, q.i, q.j, q.k);
			return dot(v, v);
		}
	};
} // namespace rsl::math::internal

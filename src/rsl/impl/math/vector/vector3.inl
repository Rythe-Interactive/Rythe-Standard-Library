#include "vector3.hpp"
#pragma once

namespace rsl::math
{
	template <typename Scalar>
	inline rythe_always_inline constexpr vector<Scalar, 3>::vector(const vector<scalar, 2>& v, scalar s) noexcept
		: vector(v.x, v.y, s)
	{
	}

	template <typename Scalar>
	inline rythe_always_inline constexpr vector<Scalar, 3>::vector(scalar s, const vector<scalar, 2>& v) noexcept
		: vector(s, v.x, v.y)
	{
	}

	template <typename Scalar>
	inline rythe_always_inline constexpr vector<Scalar, 3>::vector(scalar _x, scalar _y, scalar _z) noexcept
		: x(_x),
		  y(_y),
		  z(_z)
	{
	}

	template <typename Scalar>
	template <typename vec_type, ::std::enable_if_t<is_vector_v<vec_type> && (vec_type::size != 3 || !std::is_same_v<Scalar, typename vec_type::scalar>), bool>>
	inline rythe_always_inline constexpr vector<Scalar, 3>::vector(const vec_type& other) noexcept
	{
		if constexpr (size > vec_type::size)
		{
			for (size_type i = 0; i < vec_type::size; i++)
				data[i] = static_cast<scalar>(other.data[i]);

			for (size_type i = vec_type::size; i < size; i++)
				data[i] = static_cast<scalar>(0);
		}
		else
		{
			for (size_type i = 0; i < size; i++)
				data[i] = static_cast<scalar>(other.data[i]);
		}
	}
} // namespace rsl::math

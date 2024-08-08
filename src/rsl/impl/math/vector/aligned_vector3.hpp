#pragma once
#include "../util/type_util.hpp"
#include "swizzle/swizzle3.hpp"
#include "vector_base.hpp"

namespace rsl::math
{
	template <typename Scalar, size_type alignment = sizeof(Scalar) * 4>
	struct alignas(alignment) aligned_vector3 : vector_base
	{
		using scalar = Scalar;
		static constexpr size_type size = 3;
		using type = aligned_vector3<Scalar, alignment>;

		union
		{
			scalar data[3];

			_MATH_SWIZZLE_3_1_(scalar);
			_MATH_SWIZZLE_3_2_(scalar);
			_MATH_SWIZZLE_3_3_(scalar);
			_MATH_SWIZZLE_3_4_(scalar);
		};

		constexpr aligned_vector3() noexcept
			: xyz(static_cast<scalar>(0), static_cast<scalar>(0), static_cast<scalar>(0))
		{
		}

		constexpr aligned_vector3(const aligned_vector3&) noexcept = default;

		explicit constexpr aligned_vector3(scalar s) noexcept
			: xyz(static_cast<scalar>(s), static_cast<scalar>(s), static_cast<scalar>(s))
		{
		}

		constexpr aligned_vector3(scalar _x, scalar _y, scalar _z) noexcept
			: xyz(_x, _y, _z)
		{
		}

		constexpr aligned_vector3(const vector<scalar, 3>& other) noexcept
			: xyz(other.x, other.y, other.z)
		{
		}

		template <typename vec_type, ::std::enable_if_t<is_vector_v<vec_type> && (size != vec_type::size || !std::is_same_v<scalar, typename vec_type::scalar>), bool> = true>
		constexpr aligned_vector3(const vec_type& other) noexcept
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

		static const aligned_vector3 up;
		static const aligned_vector3 down;
		static const aligned_vector3 right;
		static const aligned_vector3 left;
		static const aligned_vector3 forward;
		static const aligned_vector3 backward;
		static const aligned_vector3 one;
		static const aligned_vector3 zero;

		constexpr aligned_vector3& operator=(const aligned_vector3&) noexcept = default;

		constexpr scalar& operator[](size_type i) noexcept
		{
			rsl_assert_msg((i >= 0) && (i < size), "vector subscript out of range");
			return data[i];
		}
		constexpr const scalar& operator[](size_type i) const noexcept
		{
			rsl_assert_msg((i >= 0) && (i < size), "vector subscript out of range");
			return data[i];
		}

		rythe_always_inline scalar length() const noexcept;
		constexpr scalar length2() const noexcept;
	};

	template <typename Scalar, size_type alignment>
	const aligned_vector3<Scalar, alignment> aligned_vector3<Scalar, alignment>::up(static_cast<Scalar>(0), static_cast<Scalar>(1), static_cast<Scalar>(0));
	template <typename Scalar, size_type alignment>
	const aligned_vector3<Scalar, alignment> aligned_vector3<Scalar, alignment>::down(static_cast<Scalar>(0), static_cast<Scalar>(-1), static_cast<Scalar>(0));
	template <typename Scalar, size_type alignment>
	const aligned_vector3<Scalar, alignment> aligned_vector3<Scalar, alignment>::right(static_cast<Scalar>(1), static_cast<Scalar>(0), static_cast<Scalar>(0));
	template <typename Scalar, size_type alignment>
	const aligned_vector3<Scalar, alignment> aligned_vector3<Scalar, alignment>::left(static_cast<Scalar>(-1), static_cast<Scalar>(0), static_cast<Scalar>(0));
	template <typename Scalar, size_type alignment>
	const aligned_vector3<Scalar, alignment> aligned_vector3<Scalar, alignment>::forward(static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(1));
	template <typename Scalar, size_type alignment>
	const aligned_vector3<Scalar, alignment> aligned_vector3<Scalar, alignment>::backward(static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(-1));
	template <typename Scalar, size_type alignment>
	const aligned_vector3<Scalar, alignment> aligned_vector3<Scalar, alignment>::one(static_cast<Scalar>(1));
	template <typename Scalar, size_type alignment>
	const aligned_vector3<Scalar, alignment> aligned_vector3<Scalar, alignment>::zero(static_cast<Scalar>(0));

	using aligned_float3 = aligned_vector3<float32>;
	using aligned_vec3 = aligned_float3;
	using aligned_double3 = aligned_vector3<float64>;
	using aligned_dvec3 = aligned_double3;
	using aligned_int3 = aligned_vector3<int>;
	using aligned_ivec3 = aligned_int3;
	using aligned_uint3 = aligned_vector3<uint>;
	using aligned_uvec3 = aligned_uint3;
	using aligned_bool3 = aligned_vector3<bool>;
	using aligned_bvec3 = aligned_bool3;
} // namespace rsl::math

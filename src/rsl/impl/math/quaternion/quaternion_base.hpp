#pragma once
#include <immintrin.h>

#include "../../util/assert.hpp"
#include "../../util/primitives.hpp"

#include "../util/type_util.hpp"
#include "../vector/vector.hpp"

namespace rsl::math
{
	template <typename Scalar, size_type RowCount, size_type ColCount>
	struct matrix;

	struct quaternion_base
	{
	};

	template <typename Scalar>
	struct quaternion : quaternion_base
	{
		using scalar = Scalar;
		static constexpr size_type size = 4;
		using type = quaternion<Scalar>;
		using vec_type = vector<scalar, 3>;

		union
		{
			struct
			{
				scalar scal;
				vec_type vec;
			};
			struct
			{
				scalar w, i, j, k;
			};
			__m128 intrin[(sizeof(scalar) * size) / sizeof(__m128)];

			scalar data[size];
		};

		constexpr quaternion() noexcept
			: w(static_cast<scalar>(1)),
			  i(static_cast<scalar>(0)),
			  j(static_cast<scalar>(0)),
			  k(static_cast<scalar>(0))
		{
		}

		constexpr quaternion(const quaternion&) noexcept = default;

		constexpr quaternion(scalar _w, scalar _i, scalar _j, scalar _k) noexcept
			: w(static_cast<scalar>(_w)),
			  i(static_cast<scalar>(_i)),
			  j(static_cast<scalar>(_j)),
			  k(static_cast<scalar>(_k))
		{
		}

		template <typename VecType, ::std::enable_if_t<is_vector_v<VecType> && VecType::size == 3, bool> = true>
		constexpr quaternion(typename VecType::scalar s, const VecType& v) noexcept
			: w(static_cast<scalar>(s)),
			  i(static_cast<scalar>(v.x)),
			  j(static_cast<scalar>(v.y)),
			  k(static_cast<scalar>(v.z))
		{
		}

		template <typename VecType, ::std::enable_if_t<is_vector_v<VecType> && VecType::size == 4, bool> = true>
		explicit constexpr quaternion(const VecType& other) noexcept
			: w(static_cast<scalar>(other.w)),
			  i(static_cast<scalar>(other.x)),
			  j(static_cast<scalar>(other.y)),
			  k(static_cast<scalar>(other.z))
		{
		}

		explicit constexpr quaternion(const matrix<scalar, 3, 3>& m) noexcept;
		explicit constexpr quaternion(const matrix<scalar, 4, 4>& m) noexcept;

		static const quaternion identity;
		static const quaternion rotate_x_90;
		static const quaternion rotate_y_90;
		static const quaternion rotate_z_90;
		static const quaternion rotate_x_180;
		static const quaternion rotate_y_180;
		static const quaternion rotate_z_180;

		constexpr quaternion& operator=(const quaternion&) noexcept = default;

		[[nodiscard]] constexpr scalar& operator[](size_type i) noexcept
		{
			rsl_assert_msg((i >= 0) && (i < size), "quaternion subscript out of range");
			return data[i];
		}
		[[nodiscard]] constexpr const scalar& operator[](size_type i) const noexcept
		{
			rsl_assert_msg((i >= 0) && (i < size), "quaternion subscript out of range");
			return data[i];
		}

		[[nodiscard]] vec_type right() noexcept;
		[[nodiscard]] vec_type forward() noexcept;
		[[nodiscard]] vec_type up() noexcept;
		[[nodiscard]] static quaternion angle_axis(scalar angle, const vec_type& vec) noexcept;
		[[nodiscard]] static quaternion look_at(const vec_type& _pos, const vec_type& _center, const vec_type& _up = vec_type::up) noexcept;
		[[nodiscard]] static quaternion conjugate(const quaternion& _quat) noexcept;
	};

#define sin45 0.70710678118654752440084436210485L
#define cos45 0.70710678118654752440084436210485L

	template <typename Scalar>
	const quaternion<Scalar> quaternion<Scalar>::identity(static_cast<Scalar>(1), static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(0));
	template <typename Scalar>
	const quaternion<Scalar> quaternion<Scalar>::rotate_x_90(static_cast<Scalar>(cos45), static_cast<Scalar>(sin45), static_cast<Scalar>(0), static_cast<Scalar>(0));
	template <typename Scalar>
	const quaternion<Scalar> quaternion<Scalar>::rotate_y_90(static_cast<Scalar>(cos45), static_cast<Scalar>(0), static_cast<Scalar>(sin45), static_cast<Scalar>(0));
	template <typename Scalar>
	const quaternion<Scalar> quaternion<Scalar>::rotate_z_90(static_cast<Scalar>(cos45), static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(sin45));
	template <typename Scalar>
	const quaternion<Scalar> quaternion<Scalar>::rotate_x_180(static_cast<Scalar>(0), static_cast<Scalar>(1), static_cast<Scalar>(0), static_cast<Scalar>(0));
	template <typename Scalar>
	const quaternion<Scalar> quaternion<Scalar>::rotate_y_180(static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(1), static_cast<Scalar>(0));
	template <typename Scalar>
	const quaternion<Scalar> quaternion<Scalar>::rotate_z_180(static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(1));

#undef sin45
#undef cos45

	using quat = quaternion<float32>;
	using quat64 = quaternion<float64>;
	using quat_max = quaternion<float_max>;
} // namespace rsl::math

#include "matrix_quat_conv.inl"

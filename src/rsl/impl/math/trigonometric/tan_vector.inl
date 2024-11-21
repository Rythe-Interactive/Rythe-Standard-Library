#pragma once
#include "../interpolation/map.hpp"

namespace rsl::math::detail
{
	template <typename T>
	struct compute_tan;

	template <typename Scalar, size_type Size>
	struct compute_tan<vector<Scalar, Size>>
	{
		using vec_type = vector<Scalar, Size>;

		template <typename ValueType>
		[[nodiscard]] static auto compute(ValueType&& value) noexcept
		{
			vec_type result;
			for (size_type i = 0; i < Size; i++) result[i] = ::std::tan(value[i]);

			return result;
		}

		template <typename ValueType>
		[[nodiscard]] static auto compute_inverse(ValueType&& value) noexcept
		{
			vec_type result;
			for (size_type i = 0; i < Size; i++) result[i] = ::std::atan(value[i]);

			return result;
		}

		template <typename X, typename Y>
			requires same_as<remove_cvr_t<X>, remove_cvr_t<Y>>
		[[nodiscard]] static auto compute_inverse2(X&& x, Y&& y) noexcept
		{
			vec_type result;
			for (size_type i = 0; i < Size; i++) result[i] = ::std::atan2(value[i]);

			return result;
		}
	};

	template <typename Scalar>
	struct compute_tan<vector<Scalar, 1u>>
	{
		using vec_type = vector<Scalar, 1u>;

		template <typename ValueType>
		[[nodiscard]] static auto compute(ValueType&& value) noexcept
		{
			return ::std::tan(value[0]);
		}

		template <typename ValueType>
		[[nodiscard]] static auto compute_inverse(ValueType&& value) noexcept
		{
			return ::std::atan(value[0]);
		}

		template <typename X, typename Y>
			requires same_as<remove_cvr_t<X>, remove_cvr_t<Y>>
		[[nodiscard]] static auto compute_inverse2(X&& x, Y&& y) noexcept
		{
			return ::std::atan2(x[0], y[0]);
		}
	};
} // namespace rsl::math::detail

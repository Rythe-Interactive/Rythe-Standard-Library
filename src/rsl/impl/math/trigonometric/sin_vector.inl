#pragma once
#include "../interpolation/map.hpp"

namespace rsl::math::internal
{
	template <typename T>
	struct compute_sin;

	template <typename Scalar, size_type Size>
	struct compute_sin<vector<Scalar, Size>>
	{
		using vec_type = vector<Scalar, Size>;

		template <typename ValueType>
		[[nodiscard]] static auto compute(ValueType&& value) noexcept
		{
			vec_type result;
			for (size_type i = 0; i < Size; i++)
			{
				result[i] = ::std::sin(value[i]);
			}

			return result;
		}

		template <typename ValueType>
		[[nodiscard]] static auto compute_inverse(ValueType&& value) noexcept
		{
			vec_type result;
			for (size_type i = 0; i < Size; i++)
			{
				result[i] = ::std::asin(value[i]);
			}

			return result;
		}
	};

	template <typename Scalar>
	struct compute_sin<vector<Scalar, 1u>>
	{
		using vec_type = vector<Scalar, 1u>;

		template <typename ValueType>
		[[nodiscard]] static auto compute(ValueType&& value) noexcept
		{
			return ::std::sin(value[0]);
		}

		template <typename ValueType>
		[[nodiscard]] static auto compute_inverse(ValueType&& value) noexcept
		{
			return ::std::asin(value[0]);
		}
	};
} // namespace rsl::math::internal

#pragma once
/*********************************************************************
 * @file   core/math/util/hash.hpp
 * @date   05-12-2021
 * @brief  Hashing functions and utilities for math structures.
 *
 * Copyright (c) 2021 Rythe-Interactive All rights reserved
 *********************************************************************/

#include "../../util/hash.hpp"
#include "../vector/vector.hpp"

namespace std
{
	template <typename Scalar, std::size_t Size, rsl::math::storage_mode Mode, std::size_t... args>
	struct std::hash<rsl::math::swizzle<Scalar, Size, Mode, args...>>
	{
		std::size_t operator()(const rsl::math::swizzle<Scalar, Size, Mode, args...>& vec) const noexcept
		{
			return rsl::hash_value(typename rsl::math::swizzle<Scalar, Size, Mode, args...>::conv_type(vec));
		}
	};
} // namespace std

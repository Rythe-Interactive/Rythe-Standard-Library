#pragma once
/*********************************************************************
 * @file   core/math/util/hash.hpp
 * @date   05-12-2021
 * @brief  Hashing functions and utilities for math structures.
 *
 * Copyright (c) 2021 Rythe-Interactive All rights reserved
 *********************************************************************/

#include "../../util/hash.hpp"
#include "../../util/primitives.hpp"
#include "../vector/vector.hpp"

template <typename Scalar, ::std::size_t Size, ::std::size_t... args>
struct ::std::hash<::rsl::math::swizzle<Scalar, Size, args...>>
{
	inline rythe_always_inline::std::size_t operator()(const ::rsl::math::swizzle<Scalar, Size, args...>& vec) const noexcept
	{
		return ::rsl::hash_value(typename ::rsl::math::swizzle<Scalar, Size, args...>::conv_type(vec));
	}
};

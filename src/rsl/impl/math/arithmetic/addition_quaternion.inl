#pragma once
#include "addition.hpp"
#include "../geometric/dot.hpp"
#include "../geometric/cross.hpp"

namespace rsl::math::detail
{
	template<typename T>
	struct compute_addition;

	template<typename Scalar>
	struct compute_addition<quaternion<Scalar>>
	{


	};
}
#pragma once
#include "../vector/vector.hpp"
#include "../quaternion/quaternion.hpp"
#include "../trigonometric/trigonometric.hpp"

namespace rsl::math
{
	template<typename Scalar>
	quaternion<Scalar> angleAxis(Scalar angle, float3 axis)
	{
		Scalar const a(angle * static_cast<Scalar>(0.5));

		return quaternion<Scalar>(cos(a), sin(a) * axis);
	}
}

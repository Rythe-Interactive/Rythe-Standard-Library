#pragma once
#include "../vector/vector.hpp"
#include "../quaternion/quaternion.hpp"
#include "../trigonometric/trigonometric.hpp"

namespace rsl::math
{
    template<typename Scalar>
    quaternion<Scalar> angleAxis(Scalar angle, float3 axis)
    {
        Scalar const a(angle);
        Scalar const s = sin(a * static_cast<T>(0.5));

        return quaternion<Scalar>(cos(a * static_cast<T>(0.5)), v * s);
    }
}

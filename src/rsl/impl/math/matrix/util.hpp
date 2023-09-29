#pragma once
#include "../../util/primitives.hpp"

#include "matrix4.hpp"
#include "../quaternion/quaternion.hpp"
#include "../vector/vector.hpp"

#include "../geometric/geometric.hpp"
#include "../trigonometric/trigonometric.hpp"
#include "../util/close_enough.hpp"

namespace rsl::math
{
    template<typename Scalar>
    [[nodiscard]] matrix<Scalar, 4, 4> perspective(Scalar rads, Scalar fov, Scalar nearZ, Scalar farZ) noexcept;
    template<typename Scalar>
    [[nodiscard]] matrix<Scalar, 4, 4> lookAt(vector<Scalar, 3> pos, vector<Scalar, 3> center, vector<Scalar, 3> up) noexcept;

    template<typename Scalar>
    [[nodiscard]] matrix<Scalar, 4, 4> transpose(matrix<Scalar, 4, 4> mat) noexcept;

    template<typename Scalar>
    [[nodiscard]] matrix<Scalar, 4, 4> translate(matrix<Scalar, 4, 4> mat, vector<Scalar, 3> pos) noexcept;
    /*template<typename Scalar>
    [[nodiscard]] matrix<Scalar, 4, 4> rotate(matrix<Scalar, 4, 4> matrix, quaternion<Scalar> orientation) noexcept;*/
    template<typename Scalar>
    [[nodiscard]] matrix<Scalar, 4, 4> rotate(matrix<Scalar, 4, 4> mat, Scalar rad, vector<Scalar, 3> axis) noexcept;
    template<typename Scalar>
    [[nodiscard]] matrix<Scalar, 4, 4> scale(matrix<Scalar, 4, 4> mat, vector<Scalar, 3> scale) noexcept;

    //Functions to sort out later
    template<typename Scalar>
    [[nodiscard]] matrix<Scalar, 3, 3> toMat3(quaternion<Scalar> rot)
    {
        matrix<Scalar, 3, 3> result(rot);
        return result;
    }

    template<typename Scalar>
    [[nodiscard]] Scalar orientedAngle(vector<Scalar, 3> x, vector<Scalar, 3> y, vector<Scalar, 3> z)
    {
        //Scalar const Angle(acos(clamp(dot(x, y), static_cast<Scalar>(-1), static_cast<Scalar>(1))));

        //if (close_enough(y, rotate(x, Angle)))
        //    return Angle;
        //else
        //    return -Angle;
    }

    template<typename Scalar>
    [[nodiscard]] matrix<Scalar, 4, 4> axisAngleMatrix(vector<Scalar, 3> axis, Scalar angle)
    {
        Scalar c = cos(angle);
        Scalar s = sin(angle);
        Scalar t = static_cast<Scalar>(1) - c;
        vector<Scalar, 3> n = normalize(axis);

        return matrix<Scalar, 4, 4>(
            t * n.x * n.x + c, t * n.x * n.y + n.z * s, t * n.x * n.z - n.y * s, static_cast<Scalar>(0.0),
            t * n.x * n.y - n.z * s, t * n.y * n.y + c, t * n.y * n.z + n.x * s, static_cast<Scalar>(0.0),
            t * n.x * n.z + n.y * s, t * n.y * n.z - n.x * s, t * n.z * n.z + c, static_cast<Scalar>(0.0),
            static_cast<Scalar>(0.0), static_cast<Scalar>(0.0), static_cast<Scalar>(0.0), static_cast<Scalar>(1.0));
    }

    template<typename Scalar>
   [[nodiscard]] quaternion<Scalar> conjugate(quaternion<Scalar> quat)
    {
        return quaternion<Scalar>(quat.w, -quat.i, -quat.j, -quat.k);
    }

   template<typename Scalar>
   [[nodiscard]] quaternion<Scalar> toQuat(matrix<Scalar,3,3> mat)
   {
       quaternion<Scalar> result(mat);
       return result;
   }

   template<typename Scalar>
   [[nodiscard]] quaternion<Scalar> toQuat(matrix<Scalar, 4, 4> mat)
   {
       quaternion<Scalar> result(mat);
       return result;
   }

}

#pragma once
#include "../../util/primitives.hpp"
#include "../util/type_util.hpp"
#include "../basic/constraint.hpp"

#include <cmath>

namespace rsl::math
{
    // cos
    template<typename T>
    [[nodiscard]] rythe_always_inline auto cos(T&& v) noexcept;

    // acos
    template<typename T>
    [[nodiscard]] rythe_always_inline auto acos(T&& v) noexcept;
}

#include "cos.inl"

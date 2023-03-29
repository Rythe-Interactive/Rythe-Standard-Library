#pragma once

#include "../defines.hpp"

namespace rsl {
    template<typename To, typename From>
    always_inline To force_cast(From& f) { return *reinterpret_cast<To*>(&f); }

    template<typename To, typename From>
    always_inline const To force_cast(const From& f) { return *reinterpret_cast<const To*>(&f); }
}

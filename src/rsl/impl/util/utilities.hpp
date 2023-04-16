#pragma once

#include "../defines.hpp"

namespace rsl {
    enum class endian
    {
#ifdef RYTHE_WINDOWS
        little = 0,
        big = 1,
        native = little
#else
        little = __ORDER_LITTLE_ENDIAN__,
        big = __ORDER_BIG_ENDIAN__,
        native = __BYTE_ORDER__
#endif
    };

    template<typename To, typename From>
    always_inline To force_cast(From& f) { return *reinterpret_cast<To*>(&f); }

    template<typename To, typename From>
    always_inline const To force_cast(const From& f) { return *reinterpret_cast<const To*>(&f); }
}

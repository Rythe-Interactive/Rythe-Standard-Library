#pragma once

namespace rsl {
    template<typename To, typename From>
    To force_cast(From& f) { return *reinterpret_cast<To*>(&f); }

    template<typename To, typename From>
    const To force_cast(const From& f) { return *reinterpret_cast<const To*>(&f); }
}

#pragma once

#include "../primitives"

namespace rsl{
    constexpr static size_type hash_combine(size_type seed, size_type hash)
    {
        return seed ^ (hash + (0x9e3779b9 + (seed << 6) + (seed >> 2)));
    }

    template<typename hash_type, typename... hash_types>
    constexpr static size_type hash_combine_multiple(size_type seed, hash_type hash, hash_types... hashes)
    {
        static_assert(std::is_same_v<size_type, hash_type>, "Hashes need to be of type: size_type");

        seed ^= (hash + (0x9e3779b9 + (seed << 6) + (seed >> 2)));
        if constexpr (sizeof...(hash_types) == 1)
        {
            return hash_combine(seed, hashes...);
        }
        else
        {
            return hash_combine_multiple(seed, hashes...);
        }
    }

    template<typename T>
    constexpr static size_type hash(const T& val)
    {
        constexpr ::std::hash<T> hasher{};
        return hasher(val);
    }

    template<typename T>
    constexpr static size_type typeHash()
    {
        T t;
        return typeid(t).hash_code();
    }
}

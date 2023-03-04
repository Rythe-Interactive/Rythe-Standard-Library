#pragma once

#include <string_view>

#include "../util/primitives.hpp"

namespace rsl {
#if !defined(RSL_ERR_MESSAGE_MAX_LENGTH)
#define RSL_ERR_MESSAGE_MAX_LENGTH 512
#endif

    template<typename...>
    struct result;

    using errc = int64;

    struct error_context {
        thread_local static errc errorCode;
        thread_local static char errorMessage[RSL_ERR_MESSAGE_MAX_LENGTH];
    };

    template<typename T>
    struct result<T>
    {
    private:
        union {
            T m_value;
            byte m_dummy;
        };

    public:

    };

    template<typename ErrorType>
    constexpr auto make_error(ErrorType&& errorType, std::string_view message) noexcept
    {
        error_context::errorCode = static_cast<errc>(errorType);
    }
}

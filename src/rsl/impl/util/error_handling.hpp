#pragma once

#include <string_view>

#include "../util/primitives.hpp"
#include "../util/concepts.hpp"
#include "../util/assert.hpp"
#include "../containers/buffered_string.hpp"
#include "../containers/buffered_list.hpp"

namespace rsl
{
#if !defined(RSL_ERR_MESSAGE_MAX_LENGTH)
#   define RSL_ERR_MESSAGE_MAX_LENGTH 512
#endif

#if !defined(RSL_WARN_MAX_COUNT)
#   define RSL_WARN_MAX_COUNT 512
#endif

#if !defined(RSL_ERR_UNDERLYING)
#   define RSL_ERR_UNDERLYING int64
#endif

    using errc = RSL_ERR_UNDERLYING;

    template<typename ERRC>
    concept error_type = std::is_enum_v<ERRC> && std::is_same_v<std::underlying_type_t<ERRC>, errc>;

    template<typename...>
    struct result;

    struct success_type {
        constexpr operator errc() const noexcept { return 0; }
    };

    constexpr static success_type success;

    struct warning {
        errc code;
        buffered_string<RSL_ERR_MESSAGE_MAX_LENGTH> warningMessage;
    };

    struct error_context {
        thread_local static errc errorCode;
        thread_local static buffered_string<RSL_ERR_MESSAGE_MAX_LENGTH> errorMessage;
        thread_local static buffered_list<warning, RSL_WARN_MAX_COUNT> warnings;
    };

    struct error_signal {};

    template<typename T>
    struct result<T> final {
    private:
        union {
            T m_value;
            byte m_dummy;
        };

    public:
        constexpr result(error_signal) noexcept {}

        template<typename... Args>
        constexpr result(Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>) : m_value(std::forward<Args>(args)...) {}

        ~result() {
#if RYTHE_VALIDATION_LEVEL >= RYTHE_WARNING_VALIDATION_LEVEL

#endif

            rsl_assert_msg(error_context::errorCode == success, error_context::errorMessage);
        }
    };

    template<error_type ErrorType>
    constexpr auto make_error(ErrorType&& errorType, std::string_view message) noexcept {
        error_context::errorCode = static_cast<errc>(errorType);
        error_context::errorMessage = message;
        return error_signal{};
    }

    template<typename T, typename... Args>
    constexpr result<T> make_result(Args&&... args) noexcept(std::is_nothrow_constructible_v<result<T>, Args...>) {
        return result<T>(std::forward<Args>(args)...);
    }
}

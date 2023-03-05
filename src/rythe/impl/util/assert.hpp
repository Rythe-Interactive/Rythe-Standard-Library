#pragma once

#include <iostream>
#include <cstdlib>

#include "../defines.hpp"

#if defined(RYTHE_VALIDATE)

namespace rsl
{
    always_inline static bool __assert_impl(const char* expr_str, const char* file, int line, std::string_view msg)
    {
        std::cerr << "Assertion failed:\t" << msg << std::endl
            << "Expected:\t" << expr_str << std::endl
            << "Source:\t\t" << file << ", line " << line << std::endl;
        std::abort();
    }
}

#define rsl_assert(expr) { [[maybe_unused]] bool ANONYMOUS_NAME(assert_) = (!!(expr)) || rsl::__assert_impl(#expr, __FILE__, __LINE__, ""); }
#define rsl_assert_msg(expr, msg) { [[maybe_unused]] bool ANONYMOUS_NAME(assert_) = (!!(expr)) || rsl::__assert_impl(#expr, __FILE__, __LINE__, msg); }

#else

#define rsl_assert(expr) if constexpr (false) { [[maybe_unused]] bool b = static_cast<bool>(expr); }
#define rsl_assert_msg(expr, msg) if constexpr (false) { [[maybe_unused]] bool b = static_cast<bool>(expr); [[maybe_unused]] const char* m = static_cast<const char*>(msg); }

#endif

#pragma once

#include <cstdlib>
#include <iostream>

#include "../defines.hpp"

#if defined(RYTHE_VALIDATE)

	#define __rsl_assert_impl(expr, file, line, msg, soft)                                                                                   \
		{                                                                                                                                    \
			std::cerr << "Assertion failed:\t" << msg << "\nExpected:\t\t" expr "\nSource:\t\t\t" file ", line " RYTHE_STRINGIFY(line) "\n"; \
			__debugbreak();                                                                                                                  \
			if constexpr (!soft)                                                                                                             \
			{                                                                                                                                \
				std::abort();                                                                                                                \
			}                                                                                                                                \
		}

	#define rsl_assert(expr)                                                            \
		{                                                                               \
			if (!!!(expr))                                                              \
				__rsl_assert_impl(RYTHE_STRINGIFY(expr), __FILE__, __LINE__, "", false) \
		}
	#define rsl_assert_msg(expr, msg)                                                    \
		{                                                                                \
			if (!!!(expr))                                                               \
				__rsl_assert_impl(RYTHE_STRINGIFY(expr), __FILE__, __LINE__, msg, false) \
		}

	#define rsl_soft_assert(expr)                                                      \
		{                                                                              \
			if (!!!(expr))                                                             \
				__rsl_assert_impl(RYTHE_STRINGIFY(expr), __FILE__, __LINE__, "", true) \
		}
	#define rsl_soft_assert_msg(expr, msg)                                              \
		{                                                                               \
			if (!!!(expr))                                                              \
				__rsl_assert_impl(RYTHE_STRINGIFY(expr), __FILE__, __LINE__, msg, true) \
		}

#else

	#define rsl_assert(expr)                                   \
		if constexpr (false)                                   \
		{                                                      \
			[[maybe_unused]] bool b = static_cast<bool>(expr); \
		}
	#define rsl_assert_msg(expr, msg)                                       \
		if constexpr (false)                                                \
		{                                                                   \
			[[maybe_unused]] bool b = static_cast<bool>(expr);              \
			[[maybe_unused]] const char* m = static_cast<const char*>(msg); \
		}

	#define rsl_soft_assert(expr)                              \
		if constexpr (false)                                   \
		{                                                      \
			[[maybe_unused]] bool b = static_cast<bool>(expr); \
		}
	#define rsl_soft_assert_msg(expr, msg)                                  \
		if constexpr (false)                                                \
		{                                                                   \
			[[maybe_unused]] bool b = static_cast<bool>(expr);              \
			[[maybe_unused]] const char* m = static_cast<const char*>(msg); \
		}

#endif

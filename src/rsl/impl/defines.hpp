#pragma once

#define _CONCAT_IMPL_(A, B) A ## B

#define CONCAT(A, B) _CONCAT_IMPL_(A, B)

#define ANONYMOUS_NAME(x) CONCAT(x, __LINE__)

#define STRINGIFY(x) #x

#define RYTHE_DEBUG_VALUE 1
#define RYTHE_RELEASE_VALUE 2

#if defined(_DEBUG) || defined(DEBUG)
/**@def RYTHE_DEBUG
 * @brief Defined in debug mode.
 */
#   define RYTHE_DEBUG
#   define RYTHE_CONFIGURATION RYTHE_DEBUG_VALUE

#   if !defined(RYTHE_VALIDATE)
#       define RYTHE_VALIDATE
#   endif
#else
/**@def RYTHE_RELEASE
 * @brief Defined in release mode.
 */
#   define RYTHE_RELEASE 
#   define RYTHE_CONFIGURATION RYTHE_RELEASE_VALUE
#endif

#if !defined(RYTHE_VALIDATION_LEVEL)
#   if defined(RYTHE_VALIDATE)
#       define RYTHE_VALIDATION_LEVEL 1
#   else
#       define RYTHE_VALIDATION_LEVEL 0
#   endif
#endif

#pragma region /////////////////////////////////// Operating system /////////////////////////////////////

#if defined(_WIN64)
#   define RYTHE_WINDOWS
#elif defined(__linux__)
#   define RYTHE_LINUX
#endif

#pragma endregion

#pragma region //////////////////////////////////// Detect compiler /////////////////////////////////////

#if defined(__clang__)
 // clang
#   define RYTHE_CLANG
#   if defined(__GNUG__) || (defined(__GNUC__) && defined(__cplusplus))
#       define RYTHE_CLANG_GCC
#   elif defined(_MSC_VER)
#       define RYTHE_CLANG_MSVC
#   endif
#elif defined(__GNUG__) || (defined(__GNUC__) && defined(__cplusplus))
 // gcc
#   define RYTHE_GCC
#elif defined(_MSC_VER)
 // msvc
#   define RYTHE_MSVC
#endif

#pragma endregion

#pragma region ////////////////////////////////// Compiler specifics ////////////////////////////////////

#if defined(RYTHE_CLANG) || defined(RYTHE_GCC)
#   define nullop() asm volatile("nop");
#elif defined(RYTHE_MSVC)
#   include <intrin.h>
#   define nullop() __nop();
#else
#   define nullop() ;
#endif

#if defined(RYTHE_CLANG) || defined(RYTHE_GCC)
#   define pause_instruction __builtin_ia32_pause
#elif defined(RYTHE_MSVC)
#   define pause_instruction _mm_pause
#else
#   define pause_instruction nullop
#endif

#if !defined(__FULL_FUNC__)
#   if defined(RYTHE_CLANG) || defined(RYTHE_GCC)
#       define __FULL_FUNC__ __PRETTY_FUNCTION__
#   elif defined(RYTHE_MSVC)
#       define __FULL_FUNC__ __FUNCSIG__
#   else
#       define __FULL_FUNC__ __func__
#   endif
#endif

#if defined(RYTHE_CLANG)
#   define RYTHE_PRAGMA_TO_STR(x) _Pragma(#x)
#   define RYTHE_CLANG_SUPPRESS_WARNING_PUSH _Pragma("clang diagnostic push")
#   define RYTHE_CLANG_SUPPRESS_WARNING(w) RYTHE_PRAGMA_TO_STR(clang diagnostic ignored w)
#   define RYTHE_CLANG_SUPPRESS_WARNING_POP _Pragma("clang diagnostic pop")
#   define RYTHE_CLANG_SUPPRESS_WARNING_WITH_PUSH(w) RYTHE_CLANG_SUPPRESS_WARNING_PUSH RYTHE_CLANG_SUPPRESS_WARNING(w)
#else
#   define RYTHE_CLANG_SUPPRESS_WARNING_PUSH
#   define RYTHE_CLANG_SUPPRESS_WARNING(w)
#   define RYTHE_CLANG_SUPPRESS_WARNING_POP
#   define RYTHE_CLANG_SUPPRESS_WARNING_WITH_PUSH(w)
#endif

#if defined(RYTHE_GCC)
#   define RYTHE_PRAGMA_TO_STR(x) _Pragma(#x)
#   define RYTHE_GCC_SUPPRESS_WARNING_PUSH _Pragma("GCC diagnostic push")
#   define RYTHE_GCC_SUPPRESS_WARNING(w) RYTHE_PRAGMA_TO_STR(GCC diagnostic ignored w)
#   define RYTHE_GCC_SUPPRESS_WARNING_POP _Pragma("GCC diagnostic pop")
#   define RYTHE_GCC_SUPPRESS_WARNING_WITH_PUSH(w) RYTHE_GCC_SUPPRESS_WARNING_PUSH RYTHE_GCC_SUPPRESS_WARNING(w)
#else
#   define RYTHE_GCC_SUPPRESS_WARNING_PUSH
#   define RYTHE_GCC_SUPPRESS_WARNING(w)
#   define RYTHE_GCC_SUPPRESS_WARNING_POP
#   define RYTHE_GCC_SUPPRESS_WARNING_WITH_PUSH(w)
#endif

#if defined(RYTHE_MSVC)
#   define RYTHE_MSVC_SUPPRESS_WARNING_PUSH __pragma(warning(push))
#   define RYTHE_MSVC_SUPPRESS_WARNING(w) __pragma(warning(disable : w))
#   define RYTHE_MSVC_SUPPRESS_WARNING_POP __pragma(warning(pop))
#   define RYTHE_MSVC_SUPPRESS_WARNING_WITH_PUSH(w) RYTHE_MSVC_SUPPRESS_WARNING_PUSH RYTHE_MSVC_SUPPRESS_WARNING(w)
#else
#   define RYTHE_MSVC_SUPPRESS_WARNING_PUSH
#   define RYTHE_MSVC_SUPPRESS_WARNING(w)
#   define RYTHE_MSVC_SUPPRESS_WARNING_POP
#   define RYTHE_MSVC_SUPPRESS_WARNING_WITH_PUSH(w)
#endif

#if defined (RYTHE_MSVC)
#   define COMPILER_WARNING(desc) __pragma(message(__FILE__ "(" STRINGIFY(__LINE__) ") : warning: " #desc))
#   define COMPILER_ERROR(desc) __pragma(message(__FILE__ "(" STRINGIFY(__LINE__) ") : error: " #desc))
#elif defined(RYTHE_GCC) || defined(RYTHE_CLANG)
#   define COMPILER_WARNING(desc) _Pragma(STRINGIFY(GCC warning desc))
#   define COMPILER_ERROR(desc) _Pragma(STRINGIFY(GCC error desc))
#endif


#if defined(RYTHE_GCC) || defined(RYTHE_CLANG)
#   define always_inline __attribute__((always_inline))
#elif defined(RYTHE_MSVC)
#   define always_inline __forceinline
#else
#   define always_inline inline
#endif


#define COPY_FUNCS(type)\
type(const type&) = default;\
type& operator=(const type&) = default;

#define COPY_FUNCS_DECO(pre, type, post)\
pre type(const type&) post = default;\
pre type& operator=(const type&) post = default;

#define MOVE_FUNCS(type)\
type(type&&) = default;\
type& operator=(type&&) = default;

#define MOVE_FUNCS_DECO(pre, type, post)\
pre type(type&&) post = default;\
pre type& operator=(type&&) post = default;

#pragma endregion

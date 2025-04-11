#pragma once

#include "../defines.hpp"
#include "primitives.hpp"

#define rsl_mock_assert(expr)                                                                                          \
	if constexpr (false)                                                                                               \
	{                                                                                                                  \
		[[maybe_unused]] bool b = static_cast<bool>(expr);                                                             \
	}
#define rsl_mock_assert_msg(expr, msg)                                                                                 \
	if constexpr (false)                                                                                               \
	{                                                                                                                  \
		[[maybe_unused]] bool b = static_cast<bool>(expr);                                                             \
		[[maybe_unused]] const char* m = static_cast<const char*>(msg);                                                \
	}

#define rsl_mock_soft_assert(expr)                                                                                     \
	if constexpr (false)                                                                                               \
	{                                                                                                                  \
		[[maybe_unused]] bool b = static_cast<bool>(expr);                                                             \
	}
#define rsl_mock_soft_assert_msg(expr, msg)                                                                            \
	if constexpr (false)                                                                                               \
	{                                                                                                                  \
		[[maybe_unused]] bool b = static_cast<bool>(expr);                                                             \
		[[maybe_unused]] const char* m = static_cast<const char*>(msg);                                                \
	}

namespace rsl::asserts
{
	namespace internal
	{
		void default_assert_handler(
			std::string_view expression, std::string_view file, size_type line, std::string_view message, bool soft
		);
	}

	using assert_handler_function = void (*)(
		std::string_view expression, std::string_view file, size_type line, std::string_view message, bool soft
	);

	extern assert_handler_function assert_handler;
} // namespace rsl::asserts

#define __rsl_assert_impl(expr, file, line, msg, soft)                                                                 \
	{                                                                                                                  \
		if (rsl::asserts::assert_handler)                                                                              \
		{                                                                                                              \
			rsl::asserts::assert_handler(expr, file, line, msg, soft);                                                 \
		}                                                                                                              \
		else                                                                                                           \
		{                                                                                                              \
			rsl::asserts::internal::default_assert_handler(expr, file, line, msg, soft);                               \
		}                                                                                                              \
	}

#define rsl_always_assert(expr)                                                                                        \
	{                                                                                                                  \
		if (!!!(expr))                                                                                                 \
			__rsl_assert_impl(RYTHE_STRINGIFY(expr), __FILE__, __LINE__, "", false)                                    \
	}
#define rsl_always_assert_msg(expr, msg)                                                                               \
	{                                                                                                                  \
		if (!!!(expr))                                                                                                 \
			__rsl_assert_impl(RYTHE_STRINGIFY(expr), __FILE__, __LINE__, msg, false)                                   \
	}

#define rsl_always_soft_assert(expr)                                                                                   \
	{                                                                                                                  \
		if (!!!(expr))                                                                                                 \
			__rsl_assert_impl(RYTHE_STRINGIFY(expr), __FILE__, __LINE__, "", true)                                     \
	}
#define rsl_always_soft_assert_msg(expr, msg)                                                                          \
	{                                                                                                                  \
		if (!!!(expr))                                                                                                 \
			__rsl_assert_impl(RYTHE_STRINGIFY(expr), __FILE__, __LINE__, msg, true)                                    \
	}

#ifdef RYTHE_VALIDATE
	#define rsl_hard_assert(expr) rsl_always_assert(expr)
	#define rsl_hard_assert_msg(expr, msg) rsl_always_assert_msg(expr, msg)

	#define rsl_soft_assert(expr) rsl_always_soft_assert(expr)
	#define rsl_soft_assert_msg(expr, msg) rsl_always_soft_assert_msg(expr, msg)

	#if RYTHE_VALIDATION_LEVEL >= RYTHE_HIGH_IMPACT_VALIDATION_LEVEL
		#define rsl_assert_frequent(expr) rsl_hard_assert(expr)
		#define rsl_assert_consistent(expr) rsl_hard_assert(expr)
		#define rsl_assert_rarely(expr) rsl_hard_assert(expr)

		#define rsl_assert_msg_frequent(expr, msg) rsl_hard_assert_msg(expr, msg)
		#define rsl_assert_msg_consistent(expr, msg) rsl_hard_assert_msg(expr, msg)
		#define rsl_assert_msg_rarely(expr, msg) rsl_hard_assert_msg(expr, msg)

		#define rsl_soft_assert_frequent(expr) rsl_soft_assert(expr)
		#define rsl_soft_assert_consistent(expr) rsl_soft_assert(expr)
		#define rsl_soft_assert_rarely(expr) rsl_soft_assert(expr)

		#define rsl_soft_assert_msg_frequent(expr, msg) rsl_soft_assert_msg(expr, msg)
		#define rsl_soft_assert_msg_consistent(expr, msg) rsl_soft_assert_msg(expr, msg)
		#define rsl_soft_assert_msg_rarely(expr, msg) rsl_soft_assert_msg(expr, msg)
	#elif RYTHE_VALIDATION_LEVEL == RYTHE_MEDIUM_IMPACT_VALIDATION_LEVEL
		#define rsl_assert_frequent(expr) rsl_mock_assert(expr)
		#define rsl_assert_consistent(expr) rsl_hard_assert(expr)
		#define rsl_assert_rarely(expr) rsl_hard_assert(expr)

		#define rsl_assert_msg_frequent(expr, msg) rsl_mock_assert_msg(expr, msg)
		#define rsl_assert_msg_consistent(expr, msg) rsl_hard_assert_msg(expr, msg)
		#define rsl_assert_msg_rarely(expr, msg) rsl_hard_assert_msg(expr, msg)

		#define rsl_soft_assert_frequent(expr) rsl_mock_soft_assert(expr)
		#define rsl_soft_assert_consistent(expr) rsl_soft_assert(expr)
		#define rsl_soft_assert_rarely(expr) rsl_soft_assert(expr)

		#define rsl_soft_assert_msg_frequent(expr, msg) rsl_mock_soft_assert_msg(expr, msg)
		#define rsl_soft_assert_msg_consistent(expr, msg) rsl_soft_assert_msg(expr, msg)
		#define rsl_soft_assert_msg_rarely(expr, msg) rsl_soft_assert_msg(expr, msg)
	#elif RYTHE_VALIDATION_LEVEL == RYTHE_LOW_IMPACT_VALIDATION_LEVEL
		#define rsl_assert_frequent(expr) rsl_mock_assert(expr)
		#define rsl_assert_consistent(expr) rsl_mock_assert(expr)
		#define rsl_assert_rarely(expr) rsl_hard_assert(expr)

		#define rsl_assert_msg_frequent(expr, msg) rsl_mock_assert_msg(expr, msg)
		#define rsl_assert_msg_consistent(expr, msg) rsl_mock_assert_msg(expr, msg)
		#define rsl_assert_msg_rarely(expr, msg) rsl_hard_assert_msg(expr, msg)

		#define rsl_soft_assert_frequent(expr) rsl_mock_soft_assert(expr)
		#define rsl_soft_assert_consistent(expr) rsl_mock_soft_assert(expr)
		#define rsl_soft_assert_rarely(expr) rsl_soft_assert(expr)

		#define rsl_soft_assert_msg_frequent(expr, msg) rsl_mock_soft_assert_msg(expr, msg)
		#define rsl_soft_assert_msg_consistent(expr, msg) rsl_mock_soft_assert_msg(expr, msg)
		#define rsl_soft_assert_msg_rarely(expr, msg) rsl_soft_assert_msg(expr, msg)
	#endif
#else
	#define rsl_assert_frequent(expr) rsl_mock_assert(expr)
	#define rsl_assert_consistent(expr) rsl_mock_assert(expr)
	#define rsl_assert_rarely(expr) rsl_mock_assert(expr)

	#define rsl_assert_msg_frequent(expr, msg) rsl_mock_assert_msg(expr, msg)
	#define rsl_assert_msg_consistent(expr, msg) rsl_mock_assert_msg(expr, msg)
	#define rsl_assert_msg_rarely(expr, msg) rsl_mock_assert_msg(expr, msg)

	#define rsl_soft_assert_frequent(expr) rsl_mock_soft_assert(expr)
	#define rsl_soft_assert_consistent(expr) rsl_mock_soft_assert(expr)
	#define rsl_soft_assert_rarely(expr) rsl_mock_soft_assert(expr)

	#define rsl_soft_assert_msg_frequent(expr, msg) rsl_mock_soft_assert_msg(expr, msg)
	#define rsl_soft_assert_msg_consistent(expr, msg) rsl_mock_soft_assert_msg(expr, msg)
	#define rsl_soft_assert_msg_rarely(expr, msg) rsl_mock_soft_assert_msg(expr, msg)

	#define rsl_hard_assert(expr) rsl_mock_assert(expr)
	#define rsl_hard_assert_msg(expr, msg) rsl_mock_assert_msg(expr, msg)
	#define rsl_soft_assert(expr) rsl_mock_soft_assert(expr)
	#define rsl_soft_assert_msg(expr, msg) rsl_mock_soft_assert_msg(expr, msg)

#endif // RYTHE_VALIDATE

#define rsl_assert_unreachable() rsl_always_assert_msg(false, "reached unreachable code")
#define rsl_assert_out_of_range(expr) rsl_assert_msg_frequent(expr, "out of range")
#define rsl_assert_out_of_range_msg(expr, msg) rsl_assert_msg_frequent(expr, msg)
#define rsl_assert_invalid_object(expr) rsl_assert_msg_frequent(expr, "invalid object")
#define rsl_assert_duplicate_object(expr) rsl_assert_msg_frequent(expr, "duplicate object")
#define rsl_assert_borrow_release_mismatch(expr) rsl_assert_msg_frequent(expr, "borrow release mismatch")
#define rsl_assert_invalid_cast(expr) rsl_assert_msg_frequent(expr, "invalid cast")
#define rsl_assert_alignment(ptr, alignment)                                                                           \
	rsl_assert_msg_frequent((reinterpret_cast<size_type>(ptr) & ((alignment) - 1)) == 0, "wrong alignment")

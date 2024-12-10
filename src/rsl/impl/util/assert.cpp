#include "assert.hpp"
#include "../logging/logging.hpp"

namespace rsl::asserts
{
	namespace internal
	{
		void default_assert_handler(
			std::string_view expression, std::string_view file, size_type line, std::string_view message, bool soft
		)
		{
			log::undecoratedln(
				soft ? log::severity::warn : log::severity::fatal,
				"Assertion failed:\t{}\nExpected:\t\t{}\nSource:\t\t\t{}, line {}", message, expression, file, line
			);

			__debugbreak();

			if (!soft)
			{
				std::abort();
			}
		}
	} // namespace internal

	assert_handler_function assert_handler = nullptr;
} // namespace rsl::asserts

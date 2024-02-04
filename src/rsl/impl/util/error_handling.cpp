#include "error_handling.hpp"

namespace rsl
{
	thread_local buffered_list<error_info, RSL_ERR_MAX_COUNT> error_context::errors;
	thread_local errid error_context::currentError;
} // namespace rsl

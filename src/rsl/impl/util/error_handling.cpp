#include "error_handling.hpp"

namespace rsl
{
	thread_local error_list error_context::errors;
	thread_local errid error_context::currentError = invalid_err_id;

	namespace internal
	{
		errid& get_errid(result_base& r) noexcept
		{
			return r.m_errid;
		}
	} // namespace internal
} // namespace rsl

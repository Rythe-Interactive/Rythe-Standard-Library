#include "memory_resource_base.hpp"
#pragma once

namespace rsl
{
#define MRB_TEMPLATE_ARGS                                                                                              \
	typename T, typename Alloc, bool AllowRawDataAccess, typename AllocMessage, typename ConstructMessage,             \
		typename FreeMessage, typename DestroyMessage

#define MRB_VALUE_TYPE                                                                                                 \
	typename memory_resource_base<                                                                                     \
		T, Alloc, AllowRawDataAccess, AllocMessage, ConstructMessage, FreeMessage, DestroyMessage>::value_type

#define MRB_TYPE                                                                                                       \
	memory_resource_base<T, Alloc, AllowRawDataAccess, AllocMessage, ConstructMessage, FreeMessage, DestroyMessage>

	template <MRB_TEMPLATE_ARGS>
	inline constexpr MRB_VALUE_TYPE* MRB_TYPE::data() noexcept
	{
		return m_data;
	}

	template <MRB_TEMPLATE_ARGS>
	inline constexpr const MRB_VALUE_TYPE* MRB_TYPE::data() const noexcept
	{
		return m_data;
	}

#undef MRB_TEMPLATE_ARGS
#undef MRB_VALUE_TYPE
#undef MRB_TYPE
} // namespace rsl

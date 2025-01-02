#pragma once
#include "multicast_delegate.hpp"

namespace rsl
{
	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	inline constexpr multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>&
	multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::push_back(invocation_element&& elem)
	{
		return push_back(value_type(m_invocationList.get_allocator_storage(), move(elem)));
	}
}

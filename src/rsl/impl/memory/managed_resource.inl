#pragma once
#include "managed_resource.hpp"

namespace rsl
{
	template <typename T, allocator_type Alloc, factory_type Factory>
	inline constexpr managed_resource<T, Alloc, Factory>::managed_resource(nullptr_type) noexcept
		: ref_counter()
	{
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	template <typename Deleter, typename... Args>
	inline constexpr managed_resource<T, Alloc, Factory>::managed_resource(Deleter deleter, Args&&... args) noexcept
		: ref_counter(),
		  m_value(forward<Args>(args)...)
	{
		using alloc_type = typename mem_rsc::typed_alloc_type::template retarget<internal::managed_payload<T, Deleter>>;
		alloc_type alloc{this->m_alloc};
		auto* ptr = alloc.allocate();
		ptr->deleter = deleter;
		ref_counter::arm(ptr);
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	inline constexpr managed_resource<T, Alloc, Factory>::~managed_resource() noexcept
	{
		if (ref_counter::is_armed() && ref_counter::free())
		{
			this->m_ptr->destroy(&m_value);
		}
	}
} // namespace rsl

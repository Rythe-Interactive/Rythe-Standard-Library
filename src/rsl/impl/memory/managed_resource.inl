#pragma once
#include "managed_resource.hpp"

namespace rsl
{
	template <typename T, allocator_type Alloc, untyped_factory_type Factory>
	inline constexpr managed_resource<T, Alloc, Factory>::managed_resource(nullptr_type) noexcept
		: ref_counter()
	{
	}

	template <typename T, allocator_type Alloc, untyped_factory_type Factory>
	template <typename Deleter, typename... Args>
	inline constexpr managed_resource<T, Alloc, Factory>::managed_resource(Deleter deleter, Args&&... args) noexcept
		: ref_counter(),
		  m_value(forward<Args>(args)...)
	{
		ref_counter::set_factory(Factory(construct_type_signal<internal::managed_payload<T, Deleter>>));
		ref_counter::arm();
		bit_cast<internal::managed_payload<T, Deleter>*>(mem_rsc::get_ptr())->deleter = deleter;
	}

	template <typename T, allocator_type Alloc, untyped_factory_type Factory>
	inline constexpr managed_resource<T, Alloc, Factory>::~managed_resource() noexcept
	{
		if (ref_counter::is_armed() && ref_counter::free())
		{
			mem_rsc::get_ptr()->destroy(&m_value);
		}
	}
} // namespace rsl

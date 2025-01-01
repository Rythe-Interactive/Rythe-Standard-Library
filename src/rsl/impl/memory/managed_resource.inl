#pragma once
#include "managed_resource.hpp"

namespace rsl
{
	template <typename T, allocator_type Alloc, untyped_factory_type Factory>
	inline constexpr managed_resource<T, Alloc, Factory>::managed_resource(nullptr_type)
		noexcept(is_nothrow_constructible_v<ref_counter>)
		: ref_counter()
	{
	}

	template <typename T, allocator_type Alloc, untyped_factory_type Factory>
	inline managed_resource<T, Alloc, Factory>::managed_resource(const allocator_storage_type& allocStorage)
		noexcept(is_nothrow_constructible_v<ref_counter, const allocator_storage_type&>)
		: ref_counter(allocStorage)
	{
	}

	template <typename T, allocator_type Alloc, untyped_factory_type Factory>
	template <typename Deleter, typename... Args>
	inline constexpr managed_resource<T, Alloc, Factory>::managed_resource(Deleter deleter, Args&&... args)
		noexcept(is_nothrow_constructible_v<ref_counter> && is_nothrow_constructible_v<T, Args...>)
		: ref_counter()
	{
		arm(deleter, forward<Args>(args)...);
	}

	template <typename T, allocator_type Alloc, untyped_factory_type Factory>
	template <typename Deleter, typename... Args>
	inline managed_resource<T, Alloc, Factory>::managed_resource(
		const allocator_storage_type& allocStorage, Deleter deleter, Args&&... args
	) noexcept(is_nothrow_constructible_v<ref_counter, const allocator_storage_type&> && is_nothrow_constructible_v<T, Args...>)
		: ref_counter(allocStorage)
	{
		arm(deleter, forward<Args>(args)...);
	}

	template <typename T, allocator_type Alloc, untyped_factory_type Factory>
	template <typename Deleter, typename... Args>
	inline constexpr void managed_resource<T, Alloc, Factory>::arm(Deleter deleter, Args&&... args)
		noexcept(is_nothrow_constructible_v<T, Args...>)
	{
		default_factory<T>{}.construct(&m_value, 1, forward<Args>(args)...);

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

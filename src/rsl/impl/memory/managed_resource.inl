#pragma once
#include "managed_resource.hpp"

namespace rsl
{
	template <typename T, internal::managed_deleter_type<T> Deleter>
	void internal::managed_payload<T, Deleter>::destroy(void* value) noexcept
	{
		if (deleter)
		{
			deleter(*static_cast<T*>(value));
		}
	}

	template <typename T, allocator_type Alloc, untyped_factory_type Factory>
	constexpr managed_resource<T, Alloc, Factory>::managed_resource(nullptr_type)
		noexcept(is_nothrow_constructible_v<ref_counter>)
		: ref_counter() {}

	template <typename T, allocator_type Alloc, untyped_factory_type Factory>
	managed_resource<T, Alloc, Factory>::managed_resource(const allocator_storage_type& allocStorage)
		noexcept(is_nothrow_constructible_v<ref_counter, const allocator_storage_type&>)
		: ref_counter(allocStorage) {}

	template <typename T, allocator_type Alloc, untyped_factory_type Factory>
	template <internal::managed_deleter_type<T> Deleter, typename... Args>
	constexpr managed_resource<T, Alloc, Factory>::managed_resource(Deleter deleter, Args&&... args)
		noexcept(is_nothrow_constructible_v<ref_counter> && is_nothrow_constructible_v<T, Args...>)
		: ref_counter()
	{
		arm(deleter, forward<Args>(args)...);
	}

	template <typename T, allocator_type Alloc, untyped_factory_type Factory>
	template <internal::managed_deleter_type<T> Deleter, typename... Args>
	managed_resource<T, Alloc, Factory>::managed_resource(const allocator_storage_type& allocStorage,
	                                                             Deleter deleter, Args&&... args)
		noexcept(is_nothrow_constructible_v<ref_counter, const allocator_storage_type&> && is_nothrow_constructible_v<
			         T, Args...>)
		: ref_counter(allocStorage)
	{
		arm(deleter, forward<Args>(args)...);
	}

	template <typename T, allocator_type Alloc, untyped_factory_type Factory>
	template <internal::managed_deleter_type<T> Deleter, typename... Args>
	constexpr void managed_resource<T, Alloc, Factory>::arm(Deleter deleter, Args&&... args)
		noexcept(is_nothrow_constructible_v<T, Args...>)
	{
		m_value.emplace(forward<Args>(args)...);

		ref_counter::set_factory(Factory(construct_type_signal<typed_payload<Deleter>>));
		ref_counter::arm();

		typed_payload<Deleter>* payload = bit_cast<typed_payload<Deleter>*>(mem_rsc::get_ptr());
		payload->deleter.deleter = deleter;
		payload->deleter.thisObject = this;
	}

	template <typename T, allocator_type Alloc, untyped_factory_type Factory>
	constexpr managed_resource<T, Alloc, Factory>::~managed_resource() noexcept
	{
		if (ref_counter::is_armed() && ref_counter::free())
		{
			mem_rsc::get_ptr()->destroy(get());
			m_value.reset();
		}
	}

	template <typename T, allocator_type Alloc, untyped_factory_type Factory>
	template <internal::managed_deleter_type<T> Deleter>
	constexpr void managed_resource<T, Alloc, Factory>::deleter_wrapper<Deleter>::operator()(T& value) const noexcept
	{
		deleter(value);
		thisObject->m_value.reset();
	}

} // namespace rsl

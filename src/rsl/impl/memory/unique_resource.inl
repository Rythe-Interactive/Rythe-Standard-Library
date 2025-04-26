#pragma once
#include "unique_resource.hpp"

namespace rsl
{
	template <typename T, allocator_type Alloc, untyped_factory_type Factory>
	unique_resource<T, Alloc, Factory>::unique_resource(const allocator_storage_type& allocStorage)
		noexcept(is_nothrow_constructible_v<mem_rsc, const allocator_storage_type&>)
		: mem_rsc(allocStorage) {}

	template <typename T, allocator_type Alloc, untyped_factory_type Factory>
	template <typename Deleter, typename... Args>
	unique_resource<T, Alloc, Factory>::unique_resource(const allocator_storage_type& allocStorage, Deleter deleter, Args&&... args)
		noexcept( is_nothrow_constructible_v<mem_rsc, const allocator_storage_type&> && is_nothrow_constructible_v<T, Args...>)
	{

	}

	template <typename T, allocator_type Alloc, untyped_factory_type Factory>
	constexpr unique_resource<T, Alloc, Factory>::unique_resource(nullptr_type)
		noexcept(is_nothrow_constructible_v<mem_rsc>)
	{

	}

	template <typename T, allocator_type Alloc, untyped_factory_type Factory>
	template <typename Deleter, typename... Args>
	constexpr unique_resource<T, Alloc, Factory>::unique_resource(Deleter deleter, Args&&... args)
		noexcept(is_nothrow_constructible_v<mem_rsc> && is_nothrow_constructible_v<T, Args...>)
	{

	}

	template <typename T, allocator_type Alloc, untyped_factory_type Factory>
	constexpr unique_resource<T, Alloc, Factory>::~unique_resource() noexcept
	{

	}

	template <typename T, allocator_type Alloc, untyped_factory_type Factory>
	template <typename Deleter, typename... Args>
	constexpr void unique_resource<T, Alloc, Factory>::arm(Deleter deleter, Args&&... args)
		noexcept(is_nothrow_constructible_v<T, Args...>)
	{
		m_value.emplace(forward<Args>(args)...);

		mem_rsc::set_factory(Factory(construct_type_signal<internal::managed_payload<T, Deleter>>));
		mem_rsc::allocate_and_construct(1, forward<Args>(args)...);
		bit_cast<internal::managed_payload<T, Deleter>*>(mem_rsc::get_ptr())->deleter = deleter;
	}

	template <typename T, allocator_type Alloc, untyped_factory_type Factory>
	constexpr bool unique_resource<T, Alloc, Factory>::is_armed() const noexcept
	{
		return mem_rsc::get_ptr();
	}

	template <typename T, allocator_type Alloc, untyped_factory_type Factory>
	constexpr void unique_resource<T, Alloc, Factory>::disarm() noexcept
	{
		if (!is_armed())
		{
			return;
		}

		release();
	}

	template <typename T, allocator_type Alloc, untyped_factory_type Factory>
	constexpr void unique_resource<T, Alloc, Factory>::release() noexcept
	{
		rsl_assert_invalid_object(is_armed());
		mem_rsc::destroy_and_deallocate();
	}
}

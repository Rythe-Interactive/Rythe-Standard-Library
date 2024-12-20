#pragma once
#include "memory_resource_base.hpp"

namespace rsl
{
	template <typename T, allocator_type Alloc, factory_type Factory, bool AllowRawDataAccess>
	inline constexpr add_pointer_t<T> memory_resource_base<T, Alloc, Factory, AllowRawDataAccess>::data() noexcept
		requires AllowRawDataAccess
	{
		return m_ptr;
	}

	template <typename T, allocator_type Alloc, factory_type Factory, bool AllowRawDataAccess>
	inline constexpr decorate_type_t<T, const_signal, pointer_signal>
	memory_resource_base<T, Alloc, Factory, AllowRawDataAccess>::data() const noexcept
		requires AllowRawDataAccess
	{
		return m_ptr;
	}

	template <typename T, allocator_type Alloc, factory_type Factory, bool AllowRawDataAccess>
	inline void memory_resource_base<T, Alloc, Factory, AllowRawDataAccess>::set_allocator(
		const allocator_storage_type& allocStorage
	) noexcept(is_nothrow_copy_assignable_v<allocator_storage_type>)
	{
		m_alloc.set_allocator(allocStorage);
	}

	template <typename T, allocator_type Alloc, factory_type Factory, bool AllowRawDataAccess>
	inline Alloc& memory_resource_base<T, Alloc, Factory, AllowRawDataAccess>::get_allocator() noexcept
	{
		return m_alloc.get_allocator();
	}

	template <typename T, allocator_type Alloc, factory_type Factory, bool AllowRawDataAccess>
	inline const Alloc& memory_resource_base<T, Alloc, Factory, AllowRawDataAccess>::get_allocator() const noexcept
	{
		return m_alloc.get_allocator();
	}

	template <typename T, allocator_type Alloc, factory_type Factory, bool AllowRawDataAccess>
	inline void
	memory_resource_base<T, Alloc, Factory, AllowRawDataAccess>::set_factory(const factory_storage_type& factoryStorage)
		noexcept(is_nothrow_copy_assignable_v<factory_storage_type>)
	{
		m_alloc.set_factory(factoryStorage);
	}

	template <typename T, allocator_type Alloc, factory_type Factory, bool AllowRawDataAccess>
	inline Factory& memory_resource_base<T, Alloc, Factory, AllowRawDataAccess>::get_factory() noexcept
	{
		return m_alloc.get_factory();
	}

	template <typename T, allocator_type Alloc, factory_type Factory, bool AllowRawDataAccess>
	inline const Factory& memory_resource_base<T, Alloc, Factory, AllowRawDataAccess>::get_factory() const noexcept
	{
		return m_alloc.get_factory();
	}

	template <typename T, allocator_type Alloc, factory_type Factory, bool AllowRawDataAccess>
	template <typename... Args>
	inline constexpr void
	memory_resource_base<T, Alloc, Factory, AllowRawDataAccess>::allocate(size_type count, Args&&... args)
		noexcept(factory_traits<Factory>::template noexcept_constructable<Args...>)
	{
		m_ptr = m_alloc.allocate(count, forward<Args>(args)...);
	}

	template <typename T, allocator_type Alloc, factory_type Factory, bool AllowRawDataAccess>
	template <typename... Args>
	inline constexpr void memory_resource_base<T, Alloc, Factory, AllowRawDataAccess>::allocate_aligned(
		size_type count, size_type alignment, Args&&... args
	) noexcept(factory_traits<Factory>::template noexcept_constructable<Args...>)
	{
		m_ptr = m_alloc.allocate_aligned(count, alignment, forward<Args>(args)...);
	}

	template <typename T, allocator_type Alloc, factory_type Factory, bool AllowRawDataAccess>
	template <typename... Args>
	inline constexpr void memory_resource_base<T, Alloc, Factory, AllowRawDataAccess>::construct(
		size_type count, size_type offset, Args&&... args
	) noexcept(factory_traits<Factory>::template noexcept_constructable<Args...>)
	{
		m_ptr = m_alloc.construct(m_ptr + offset, count, forward<Args>(args)...);
	}

	template <typename T, allocator_type Alloc, factory_type Factory, bool AllowRawDataAccess>
	template <typename... Args>
	inline constexpr void memory_resource_base<T, Alloc, Factory, AllowRawDataAccess>::reallocate(
		size_type oldCount, size_type newCount, Args&&... args
	)
		noexcept(
			factory_traits<Factory>::template noexcept_constructable<Args...> &&
			factory_traits<Factory>::noexcept_moveable
		)
	{
		m_ptr = m_alloc.reallocate(m_ptr, oldCount, newCount, forward<Args>(args)...);
	}

	template <typename T, allocator_type Alloc, factory_type Factory, bool AllowRawDataAccess>
	template <typename... Args>
	inline constexpr void memory_resource_base<T, Alloc, Factory, AllowRawDataAccess>::reallocate_aligned(
		size_type oldCount, size_type newCount, size_type alignment, Args&&... args
	)
		noexcept(
			factory_traits<Factory>::template noexcept_constructable<Args...> &&
			factory_traits<Factory>::noexcept_moveable
		)
	{
		m_ptr = m_alloc.reallocate_aligned(m_ptr, oldCount, newCount, alignment, forward<Args>(args)...);
	}

	template <typename T, allocator_type Alloc, factory_type Factory, bool AllowRawDataAccess>
	inline constexpr void memory_resource_base<T, Alloc, Factory, AllowRawDataAccess>::deallocate(size_type count
	) noexcept
	{
		m_alloc.deallocate(m_ptr, count);
		m_ptr = nullptr;
	}

	template <typename T, allocator_type Alloc, factory_type Factory, bool AllowRawDataAccess>
	inline constexpr void memory_resource_base<T, Alloc, Factory, AllowRawDataAccess>::deallocate_aligned(
		size_type count, size_type alignment
	) noexcept
	{
		m_alloc.deallocate_aligned(m_ptr, count, alignment);
		m_ptr = nullptr;
	}
} // namespace rsl

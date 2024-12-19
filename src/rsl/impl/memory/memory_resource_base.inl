#pragma once
#include "memory_resource_base.hpp"

namespace rsl
{
	template <typename T, allocator_type Alloc, factory_type Factory, bool AllowRawDataAccess>
	inline constexpr add_pointer_t<T> memory_resource_base<T, Alloc, Factory, AllowRawDataAccess>::data() noexcept
		requires AllowRawDataAccess
	{
		return m_data;
	}

	template <typename T, allocator_type Alloc, factory_type Factory, bool AllowRawDataAccess>
	inline constexpr decorate_type_t<T, const_signal, pointer_signal>
	memory_resource_base<T, Alloc, Factory, AllowRawDataAccess>::data() const noexcept
		requires AllowRawDataAccess
	{
		return m_data;
	}

	template <typename T, allocator_type Alloc, factory_type Factory, bool AllowRawDataAccess>
	inline void memory_resource_base<T, Alloc, Factory, AllowRawDataAccess>::set_allocator(
		const allocator_storage_type& allocStorage
	) noexcept(is_nothrow_copy_assignable_v<allocator_storage_type>)
	{
		m_alloc = allocStorage;
	}

	template <typename T, allocator_type Alloc, factory_type Factory, bool AllowRawDataAccess>
	inline Alloc& memory_resource_base<T, Alloc, Factory, AllowRawDataAccess>::get_allocator() noexcept
	{
		return *m_alloc;
	}

	template <typename T, allocator_type Alloc, factory_type Factory, bool AllowRawDataAccess>
	inline const Alloc& memory_resource_base<T, Alloc, Factory, AllowRawDataAccess>::get_allocator() const noexcept
	{
		return *m_alloc;
	}

	template <typename T, allocator_type Alloc, factory_type Factory, bool AllowRawDataAccess>
	inline void
	memory_resource_base<T, Alloc, Factory, AllowRawDataAccess>::set_factory(const factory_storage_type& factoryStorage)
		noexcept(is_nothrow_copy_assignable_v<factory_storage_type>)
	{
		m_factory = factoryStorage;
	}

	template <typename T, allocator_type Alloc, factory_type Factory, bool AllowRawDataAccess>
	inline Factory& memory_resource_base<T, Alloc, Factory, AllowRawDataAccess>::get_factory() noexcept
	{
		return *m_factory;
	}

	template <typename T, allocator_type Alloc, factory_type Factory, bool AllowRawDataAccess>
	inline const Factory& memory_resource_base<T, Alloc, Factory, AllowRawDataAccess>::get_factory() const noexcept
	{
		return *m_factory;
	}

	template <typename T, allocator_type Alloc, factory_type Factory, bool AllowRawDataAccess>
	inline constexpr void memory_resource_base<T, Alloc, Factory, AllowRawDataAccess>::allocate(size_type n)
	{
		m_ptr = m_alloc.allocate(n);
	}

	template <typename T, allocator_type Alloc, factory_type Factory, bool AllowRawDataAccess>
	inline constexpr size_type
	memory_resource_base<T, Alloc, Factory, AllowRawDataAccess>::allocate_at_least(size_type n)
	{
		auto [ptr, count] = m_alloc.allocate_at_least(n);
		m_ptr = ptr;
		return count;
	}

	template <typename T, allocator_type Alloc, factory_type Factory, bool AllowRawDataAccess>
	template <typename... Args>
	inline constexpr void memory_resource_base<T, Alloc, Factory, AllowRawDataAccess>::construct(
		size_type n, diff_type offset, Args&&... args
	)
	{
		m_alloc.construct(m_ptr + offset, n, std::forward<Args>(args...));
	}

	template <typename T, allocator_type Alloc, factory_type Factory, bool AllowRawDataAccess>
	inline constexpr void memory_resource_base<T, Alloc, Factory, AllowRawDataAccess>::deallocate(size_type n)
	{
		m_alloc.deallocate(m_ptr, n);
	}

	template <typename T, allocator_type Alloc, factory_type Factory, bool AllowRawDataAccess>
	inline constexpr void memory_resource_base<T, Alloc, Factory, AllowRawDataAccess>::destroy()
	{
		m_alloc.destroy(m_ptr);
	}
} // namespace rsl

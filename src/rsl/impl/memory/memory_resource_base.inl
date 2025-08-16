#pragma once
#include "memory_resource_base.hpp"

namespace rsl
{
	template <allocator_type Alloc, untyped_factory_type Factory, typename UtilType>
	inline constexpr untyped_dynamic_memory_resource_base<Alloc, Factory, UtilType>::untyped_dynamic_memory_resource_base(
		const allocator_storage_type& allocStorage
	) noexcept(is_nothrow_constructible_v<typed_alloc_type, const allocator_storage_type&>)
		: m_alloc(allocStorage, construct_type_signal<UtilType>)
	{
	}

	template <allocator_type Alloc, untyped_factory_type Factory, typename UtilType>
	inline constexpr untyped_dynamic_memory_resource_base<Alloc, Factory, UtilType>::untyped_dynamic_memory_resource_base(
		const factory_storage_type& factoryStorage
	) noexcept(is_nothrow_constructible_v<typed_alloc_type, const factory_storage_type&>)
		: m_alloc(factoryStorage)
	{
	}

	template <allocator_type Alloc, untyped_factory_type Factory, typename UtilType>
	inline constexpr untyped_dynamic_memory_resource_base<Alloc, Factory, UtilType>::untyped_dynamic_memory_resource_base(
		const allocator_storage_type& allocStorage, const factory_storage_type& factoryStorage
	) noexcept(is_nothrow_constructible_v<typed_alloc_type, const allocator_storage_type&, const factory_storage_type&>)
		: m_alloc(allocStorage, factoryStorage)
	{
	}

	template <allocator_type Alloc, untyped_factory_type Factory, typename UtilType>
	inline constexpr void
	untyped_dynamic_memory_resource_base<Alloc, Factory, UtilType>::set_allocator(const allocator_storage_type& allocStorage)
		noexcept(is_nothrow_copy_assignable_v<allocator_storage_type>)
	{
		m_alloc.set_allocator(allocStorage);
	}

	template <allocator_type Alloc, untyped_factory_type Factory, typename UtilType>
	inline constexpr Alloc& untyped_dynamic_memory_resource_base<Alloc, Factory, UtilType>::get_allocator() noexcept
	{
		return m_alloc.get_allocator();
	}

	template <allocator_type Alloc, untyped_factory_type Factory, typename UtilType>
	inline constexpr const Alloc& untyped_dynamic_memory_resource_base<Alloc, Factory, UtilType>::get_allocator() const noexcept
	{
		return m_alloc.get_allocator();
	}

	template <allocator_type Alloc, untyped_factory_type Factory, typename UtilType>
	inline constexpr void
	untyped_dynamic_memory_resource_base<Alloc, Factory, UtilType>::set_factory(const factory_storage_type& factoryStorage)
		noexcept(is_nothrow_copy_assignable_v<factory_storage_type>)
	{
		m_alloc.set_factory(factoryStorage);
	}

	template <allocator_type Alloc, untyped_factory_type Factory, typename UtilType>
	inline constexpr Factory& untyped_dynamic_memory_resource_base<Alloc, Factory, UtilType>::get_factory() noexcept
	{
		return m_alloc.get_factory();
	}

	template <allocator_type Alloc, untyped_factory_type Factory, typename UtilType>
	inline constexpr const Factory& untyped_dynamic_memory_resource_base<Alloc, Factory, UtilType>::get_factory() const noexcept
	{
		return m_alloc.get_factory();
	}

	template <allocator_type Alloc, untyped_factory_type Factory, typename UtilType>
	inline constexpr typename untyped_dynamic_memory_resource_base<Alloc, Factory, UtilType>::allocator_storage_type&
	untyped_dynamic_memory_resource_base<Alloc, Factory, UtilType>::get_allocator_storage() noexcept
	{
		return m_alloc.get_allocator_storage();
	}

	template <allocator_type Alloc, untyped_factory_type Factory, typename UtilType>
	inline constexpr const typename untyped_dynamic_memory_resource_base<Alloc, Factory, UtilType>::allocator_storage_type&
	untyped_dynamic_memory_resource_base<Alloc, Factory, UtilType>::get_allocator_storage() const noexcept
	{
		return m_alloc.get_allocator_storage();
	}

	template <allocator_type Alloc, untyped_factory_type Factory, typename UtilType>
	inline constexpr typename untyped_dynamic_memory_resource_base<Alloc, Factory, UtilType>::factory_storage_type&
	untyped_dynamic_memory_resource_base<Alloc, Factory, UtilType>::get_factory_storage() noexcept
	{
		return m_alloc.get_factory_storage();
	}

	template <allocator_type Alloc, untyped_factory_type Factory, typename UtilType>
	inline constexpr const typename untyped_dynamic_memory_resource_base<Alloc, Factory, UtilType>::factory_storage_type&
	untyped_dynamic_memory_resource_base<Alloc, Factory, UtilType>::get_factory_storage() const noexcept
	{
		return m_alloc.get_factory_storage();
	}

	template <allocator_type Alloc, untyped_factory_type Factory, typename UtilType>
	inline constexpr void untyped_dynamic_memory_resource_base<Alloc, Factory, UtilType>::allocate_and_construct(size_type count
	) noexcept(factory_traits<Factory>::template noexcept_constructable<>)
	{
		m_ptr = m_alloc.allocate_and_construct(count);
	}

	template <allocator_type Alloc, untyped_factory_type Factory, typename UtilType>
	inline constexpr void untyped_dynamic_memory_resource_base<Alloc, Factory, UtilType>::allocate(size_type count) noexcept
	{
		m_ptr = m_alloc.allocate(count);
	}

	template <allocator_type Alloc, untyped_factory_type Factory, typename UtilType>
	inline constexpr void
	untyped_dynamic_memory_resource_base<Alloc, Factory, UtilType>::allocate(size_type count, size_type alignment) noexcept
	{
		m_ptr = m_alloc.allocate(count, alignment);
	}

	template <allocator_type Alloc, untyped_factory_type Factory, typename UtilType>
	inline constexpr void
	untyped_dynamic_memory_resource_base<Alloc, Factory, UtilType>::reallocate(size_type oldCount, size_type newCount)
		noexcept(factory_traits<Factory>::noexcept_moveable)
	{
		m_ptr = m_alloc.reallocate(m_ptr, oldCount, newCount);
	}

	template <allocator_type Alloc, untyped_factory_type Factory, typename UtilType>
	inline constexpr void untyped_dynamic_memory_resource_base<Alloc, Factory, UtilType>::reallocate(
		size_type oldCount, size_type newCount, size_type alignment
	) noexcept(factory_traits<Factory>::noexcept_moveable)
	{
		m_ptr = m_alloc.reallocate(m_ptr, oldCount, newCount, alignment);
	}

	template <allocator_type Alloc, untyped_factory_type Factory, typename UtilType>
	inline constexpr void untyped_dynamic_memory_resource_base<Alloc, Factory, UtilType>::deallocate(size_type count) noexcept
	{
		m_alloc.deallocate(m_ptr, count);
		m_ptr = nullptr;
	}

	template <allocator_type Alloc, untyped_factory_type Factory, typename UtilType>
	inline constexpr void
	untyped_dynamic_memory_resource_base<Alloc, Factory, UtilType>::deallocate(size_type count, size_type alignment) noexcept
	{
		m_alloc.deallocate(m_ptr, count, alignment);
		m_ptr = nullptr;
	}

	template <allocator_type Alloc, untyped_factory_type Factory, typename UtilType>
	inline constexpr void
	untyped_dynamic_memory_resource_base<Alloc, Factory, UtilType>::construct(size_type count, size_type offset)
		noexcept(factory_traits<Factory>::template noexcept_constructable<>)
	{
		m_alloc.construct(advance(m_ptr, offset * m_alloc.typeSize()), count);
	}

	template <allocator_type Alloc, untyped_factory_type Factory, typename UtilType>
	inline constexpr void
	untyped_dynamic_memory_resource_base<Alloc, Factory, UtilType>::destroy(size_type count, size_type offset) noexcept
	{
		m_alloc.destroy(advance(m_ptr, offset * m_alloc.typeSize()), count);
	}

	template <allocator_type Alloc, untyped_factory_type Factory, typename UtilType>
	template <typename... Args>
	inline constexpr void untyped_dynamic_memory_resource_base<Alloc, Factory, UtilType>::allocate_aligned_and_construct(
		size_type count, size_type alignment
	) noexcept(factory_traits<Factory>::template noexcept_constructable<>)
	{
		m_ptr = m_alloc.allocate_aligned_and_construct(count, alignment);
	}

	template <allocator_type Alloc, untyped_factory_type Factory, typename UtilType>
	template <typename... Args>
	inline constexpr void untyped_dynamic_memory_resource_base<Alloc, Factory, UtilType>::reallocate_and_construct(
		size_type oldCount, size_type newCount
	) noexcept(factory_traits<Factory>::template noexcept_constructable<> && factory_traits<Factory>::noexcept_moveable)
	{
		m_ptr = m_alloc.reallocate_and_construct(m_ptr, oldCount, newCount);
	}

	template <allocator_type Alloc, untyped_factory_type Factory, typename UtilType>
	template <typename... Args>
	inline constexpr void untyped_dynamic_memory_resource_base<Alloc, Factory, UtilType>::reallocate_aligned_and_construct(
		size_type oldCount, size_type newCount, size_type alignment
	) noexcept(factory_traits<Factory>::template noexcept_constructable<> && factory_traits<Factory>::noexcept_moveable)
	{
		m_ptr = m_alloc.reallocate_aligned_and_construct(m_ptr, oldCount, newCount, alignment);
	}

	template <allocator_type Alloc, untyped_factory_type Factory, typename UtilType>
	inline constexpr void untyped_dynamic_memory_resource_base<Alloc, Factory, UtilType>::destroy_and_deallocate(size_type count
	) noexcept
	{
		m_alloc.destroy_and_deallocate(m_ptr, count);
		m_ptr = nullptr;
	}

	template <allocator_type Alloc, untyped_factory_type Factory, typename UtilType>
	inline constexpr void untyped_dynamic_memory_resource_base<Alloc, Factory, UtilType>::destroy_and_deallocate_aligned(
		size_type count, size_type alignment
	) noexcept
	{
		m_alloc.destroy_and_deallocate_aligned(m_ptr, count, alignment);
		m_ptr = nullptr;
	}

	template <allocator_type Alloc, untyped_factory_type Factory, typename UtilType>
	inline constexpr UtilType* untyped_dynamic_memory_resource_base<Alloc, Factory, UtilType>::get_ptr() noexcept
	{
		return bit_cast<UtilType*>(m_ptr);
	}

	template <allocator_type Alloc, untyped_factory_type Factory, typename UtilType>
	inline constexpr const UtilType* untyped_dynamic_memory_resource_base<Alloc, Factory, UtilType>::get_ptr() const noexcept
	{
		return bit_cast<const UtilType*>(m_ptr);
	}

	template <allocator_type Alloc, untyped_factory_type Factory, typename UtilType>
	inline constexpr void untyped_dynamic_memory_resource_base<Alloc, Factory, UtilType>::set_ptr(UtilType* const& ptr) noexcept
	{
		m_ptr = ptr;
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	inline constexpr typed_dynamic_memory_resource_base<T, Alloc, Factory>::typed_dynamic_memory_resource_base(
		const allocator_storage_type& allocStorage
	) noexcept(is_nothrow_constructible_v<typed_alloc_type, const allocator_storage_type&>)
		: m_alloc(allocStorage)
	{
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	inline constexpr typed_dynamic_memory_resource_base<T, Alloc, Factory>::typed_dynamic_memory_resource_base(
		const factory_storage_type& factoryStorage
	) noexcept(is_nothrow_constructible_v<typed_alloc_type, const factory_storage_type&>)
		: m_alloc(factoryStorage)
	{
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	inline constexpr typed_dynamic_memory_resource_base<T, Alloc, Factory>::typed_dynamic_memory_resource_base(
		const allocator_storage_type& allocStorage, const factory_storage_type& factoryStorage
	) noexcept(is_nothrow_constructible_v<typed_alloc_type, const allocator_storage_type&, const factory_storage_type&>)
		: m_alloc(allocStorage, factoryStorage)
	{
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	inline constexpr void
	typed_dynamic_memory_resource_base<T, Alloc, Factory>::set_allocator(const allocator_storage_type& allocStorage)
		noexcept(is_nothrow_copy_assignable_v<allocator_storage_type>)
	{
		m_alloc.set_allocator(allocStorage);
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	inline constexpr Alloc& typed_dynamic_memory_resource_base<T, Alloc, Factory>::get_allocator() noexcept
	{
		return m_alloc.get_allocator();
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	inline constexpr const Alloc& typed_dynamic_memory_resource_base<T, Alloc, Factory>::get_allocator() const noexcept
	{
		return m_alloc.get_allocator();
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	inline constexpr void
	typed_dynamic_memory_resource_base<T, Alloc, Factory>::set_factory(const factory_storage_type& factoryStorage)
		noexcept(is_nothrow_copy_assignable_v<factory_storage_type>)
	{
		m_alloc.set_factory(factoryStorage);
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	inline constexpr Factory& typed_dynamic_memory_resource_base<T, Alloc, Factory>::get_factory() noexcept
	{
		return m_alloc.get_factory();
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	inline constexpr const Factory& typed_dynamic_memory_resource_base<T, Alloc, Factory>::get_factory() const noexcept
	{
		return m_alloc.get_factory();
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	inline constexpr typename typed_dynamic_memory_resource_base<T, Alloc, Factory>::allocator_storage_type&
	typed_dynamic_memory_resource_base<T, Alloc, Factory>::get_allocator_storage() noexcept
	{
		return m_alloc.get_allocator_storage();
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	inline constexpr const typename typed_dynamic_memory_resource_base<T, Alloc, Factory>::allocator_storage_type&
	typed_dynamic_memory_resource_base<T, Alloc, Factory>::get_allocator_storage() const noexcept
	{
		return m_alloc.get_allocator_storage();
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	inline constexpr typename typed_dynamic_memory_resource_base<T, Alloc, Factory>::factory_storage_type&
	typed_dynamic_memory_resource_base<T, Alloc, Factory>::get_factory_storage() noexcept
	{
		return m_alloc.get_factory_storage();
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	inline constexpr const typename typed_dynamic_memory_resource_base<T, Alloc, Factory>::factory_storage_type&
	typed_dynamic_memory_resource_base<T, Alloc, Factory>::get_factory_storage() const noexcept
	{
		return m_alloc.get_factory_storage();
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	inline constexpr void typed_dynamic_memory_resource_base<T, Alloc, Factory>::allocate(size_type count) noexcept
	{
		m_ptr = m_alloc.allocate(count);
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	inline constexpr void
	typed_dynamic_memory_resource_base<T, Alloc, Factory>::allocate(size_type count, size_type alignment) noexcept
	{
		m_ptr = m_alloc.allocate(count, alignment);
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	inline constexpr void
	typed_dynamic_memory_resource_base<T, Alloc, Factory>::reallocate(size_type oldCount, size_type newCount)
		noexcept(factory_traits<Factory>::noexcept_moveable)
	{
		m_ptr = m_alloc.reallocate(m_ptr, oldCount, newCount);
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	inline constexpr void typed_dynamic_memory_resource_base<T, Alloc, Factory>::reallocate(
		size_type oldCount, size_type newCount, size_type alignment
	) noexcept(factory_traits<Factory>::noexcept_moveable)
	{
		m_ptr = m_alloc.reallocate(m_ptr, oldCount, newCount, alignment);
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	inline constexpr void typed_dynamic_memory_resource_base<T, Alloc, Factory>::deallocate(size_type count) noexcept
	{
		m_alloc.deallocate(m_ptr, count);
		m_ptr = nullptr;
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	inline constexpr void
	typed_dynamic_memory_resource_base<T, Alloc, Factory>::deallocate(size_type count, size_type alignment) noexcept
	{
		m_alloc.deallocate(m_ptr, count, alignment);
		m_ptr = nullptr;
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	template <typename... Args>
	inline constexpr void
	typed_dynamic_memory_resource_base<T, Alloc, Factory>::construct(size_type count, size_type offset, Args&&... args)
		noexcept(factory_traits<Factory>::template noexcept_constructable<Args...>)
	{
		m_alloc.construct(m_ptr + offset, count, rsl::forward<Args>(args)...);
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	inline constexpr void
	typed_dynamic_memory_resource_base<T, Alloc, Factory>::destroy(size_type count, size_type offset) noexcept
	{
		m_alloc.destroy(m_ptr + offset, count);
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	template <typename ... Args>
	constexpr void typed_dynamic_memory_resource_base<T, Alloc, Factory>::copy(size_type count, size_type offset, const T* src)
		noexcept(factory_traits<Factory>::noexcept_copyable)
	{
		m_alloc.copy(m_ptr + offset, src, count);
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	template <typename ... Args>
	constexpr void typed_dynamic_memory_resource_base<T, Alloc, Factory>::move(size_type count, size_type offset, T* src)
		noexcept(factory_traits<Factory>::noexcept_moveable)
	{
		m_alloc.move(m_ptr + offset, src, count);
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	template <typename... Args>
	inline constexpr void
	typed_dynamic_memory_resource_base<T, Alloc, Factory>::allocate_and_construct(size_type count, Args&&... args)
		noexcept(factory_traits<Factory>::template noexcept_constructable<Args...>)
	{
		m_ptr = m_alloc.allocate_and_construct(count, forward<Args>(args)...);
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	template <typename... Args>
	inline constexpr void typed_dynamic_memory_resource_base<T, Alloc, Factory>::allocate_aligned_and_construct(
		size_type count, size_type alignment, Args&&... args
	) noexcept(factory_traits<Factory>::template noexcept_constructable<Args...>)
	{
		m_ptr = m_alloc.allocate_aligned_and_construct(count, alignment, forward<Args>(args)...);
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	template <typename... Args>
	inline constexpr void typed_dynamic_memory_resource_base<T, Alloc, Factory>::reallocate_and_construct(
		size_type oldCount, size_type newCount, Args&&... args
	)
		noexcept(
			factory_traits<Factory>::template noexcept_constructable<Args...> &&
			factory_traits<Factory>::noexcept_moveable
		)
	{
		m_ptr = m_alloc.reallocate_and_construct(m_ptr, oldCount, newCount, forward<Args>(args)...);
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	template <typename... Args>
	inline constexpr void typed_dynamic_memory_resource_base<T, Alloc, Factory>::reallocate_aligned_and_construct(
		size_type oldCount, size_type newCount, size_type alignment, Args&&... args
	)
		noexcept(
			factory_traits<Factory>::template noexcept_constructable<Args...> &&
			factory_traits<Factory>::noexcept_moveable
		)
	{
		m_ptr = m_alloc.reallocate_aligned_and_construct(m_ptr, oldCount, newCount, alignment, forward<Args>(args)...);
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	inline constexpr void typed_dynamic_memory_resource_base<T, Alloc, Factory>::destroy_and_deallocate(size_type count
	) noexcept
	{
		m_alloc.destroy_and_deallocate(m_ptr, count);
		m_ptr = nullptr;
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	inline constexpr void typed_dynamic_memory_resource_base<T, Alloc, Factory>::destroy_and_deallocate_aligned(
		size_type count, size_type alignment
	) noexcept
	{
		m_alloc.destroy_and_deallocate_aligned(m_ptr, count, alignment);
		m_ptr = nullptr;
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	inline constexpr T* typed_dynamic_memory_resource_base<T, Alloc, Factory>::get_ptr() noexcept
	{
		return m_ptr;
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	inline constexpr const T* typed_dynamic_memory_resource_base<T, Alloc, Factory>::get_ptr() const noexcept
	{
		return m_ptr;
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	inline constexpr void typed_dynamic_memory_resource_base<T, Alloc, Factory>::set_ptr(T* const& ptr) noexcept
	{
		m_ptr = ptr;
	}
} // namespace rsl

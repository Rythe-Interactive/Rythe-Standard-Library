#pragma once
#include "unique_object.hpp"

namespace rsl
{
	template <typename T, allocator_type Alloc, optional_typed_factory_type Factory>
	constexpr unique_object<T, Alloc, Factory>::unique_object(
		nullptr_type) noexcept(is_nothrow_constructible_v<mem_rsc>)
		: unique_rsc(), m_factory() {}

	template <typename T, allocator_type Alloc, optional_typed_factory_type Factory>
	unique_object<T, Alloc, Factory>::unique_object(const allocator_storage_type& allocStorage)
		noexcept(is_nothrow_constructible_v<mem_rsc, const allocator_storage_type&>)
		: unique_rsc(allocStorage), m_factory() {}

	template <typename T, allocator_type Alloc, optional_typed_factory_type Factory>
	unique_object<T, Alloc, Factory>::unique_object(const factory_storage_type& factoryStorage)
		noexcept(is_nothrow_constructible_v<mem_rsc>)
		: unique_rsc(), m_factory(factoryStorage) {}

	template <typename T, allocator_type Alloc, optional_typed_factory_type Factory>
	unique_object<T, Alloc, Factory>::unique_object(const allocator_storage_type& allocStorage,
	                                                const factory_storage_type& factoryStorage)
		noexcept(is_nothrow_constructible_v<mem_rsc, const allocator_storage_type&>)
		: unique_rsc(allocStorage), m_factory(factoryStorage) {}

	template <typename T, allocator_type Alloc, optional_typed_factory_type Factory>
	template <typename... Args>
	constexpr unique_object<T, Alloc, Factory>::unique_object(in_place_signal_type, Args&&... args)
		noexcept(is_nothrow_constructible_v<mem_rsc> && is_nothrow_constructible_v<T, Args...>)
		: unique_rsc(), m_factory()
	{
		unique_object::arm(forward<Args>(args)...);
	}

	template <typename T, allocator_type Alloc, optional_typed_factory_type Factory>
	template <typename... Args>
	unique_object<T, Alloc, Factory>::unique_object(const allocator_storage_type& allocStorage,
	                                                in_place_signal_type, Args&&... args)
		noexcept(is_nothrow_constructible_v<mem_rsc, const allocator_storage_type&> && is_nothrow_constructible_v<
			         T, Args...>)
		: unique_rsc(allocStorage), m_factory()
	{
		unique_object::arm(forward<Args>(args)...);
	}

	template <typename T, allocator_type Alloc, optional_typed_factory_type Factory>
	template <typename... Args>
	unique_object<T, Alloc, Factory>::unique_object(const allocator_storage_type& allocStorage,
	                                                const factory_storage_type& factoryStorage,
	                                                in_place_signal_type, Args&&... args)
		noexcept(is_nothrow_constructible_v<mem_rsc, const allocator_storage_type&> && is_nothrow_constructible_v<
			         T, Args...>)
		: unique_rsc(allocStorage), m_factory(factoryStorage)
	{
		unique_object::arm(forward<Args>(args)...);
	}

	template <typename T, allocator_type Alloc, optional_typed_factory_type Factory>
	constexpr unique_object<T, Alloc, Factory>::unique_object(unique_object&& other) noexcept
		: unique_rsc(rsl::move(other)), m_factory(other.m_factory) {}

	template <typename T, allocator_type Alloc, optional_typed_factory_type Factory>
	template <typename OtherType, optional_typed_factory_type OtherFactory>
			requires (is_pointer_assignable_v<T, OtherType>)
	constexpr unique_object<T, Alloc, Factory>::unique_object(unique_object<OtherType, Alloc, OtherFactory>&& other
	) noexcept
		: unique_rsc(other.template unique_object<OtherType, Alloc, OtherFactory>::unique_rsc::get_allocator_storage(), other.template unique_object<OtherType, Alloc, OtherFactory>::unique_rsc::get_factory_storage()), m_factory(other.get_factory_storage())
	{
		unique_rsc::m_value.emplace(rsl::move(other.template unique_object<OtherType, Alloc, OtherFactory>::unique_rsc::m_value.value()));
		mem_rsc::set_ptr(other.template unique_object<OtherType, Alloc, OtherFactory>::mem_rsc::get_ptr());
		other.template unique_object<OtherType, Alloc, OtherFactory>::mem_rsc::set_ptr(nullptr);
	}

	template <typename T, allocator_type Alloc, optional_typed_factory_type Factory>
	constexpr unique_object<T, Alloc, Factory>& unique_object<T, Alloc, Factory>::operator=(unique_object&& other) noexcept
	{
		unique_rsc::operator=(rsl::move(other));
		m_factory = rsl::move(other.get_factory_storage());
		return *this;
	}

	template <typename T, allocator_type Alloc, optional_typed_factory_type Factory>
	template <typename OtherType, optional_typed_factory_type OtherFactory> requires (is_pointer_assignable_v<T, OtherType>)
	constexpr unique_object<T, Alloc, Factory>& unique_object<T, Alloc, Factory>::operator=(unique_object<OtherType, Alloc, OtherFactory>&& other) noexcept
	{
		unique_rsc::disarm();

		mem_rsc::set_allocator(other.mem_rsc::get_allocator_storage());
		mem_rsc::set_factory(other.mem_rsc::get_factory_storage());
		unique_rsc::m_value.emplace(rsl::move(other.unique_rsc::m_value.value()));

		m_factory = rsl::move(other.get_factory_storage());

		mem_rsc::set_ptr(other.mem_rsc::get_ptr());
		other.mem_rsc::set_ptr(nullptr);

		return *this;
	}

	template <typename T, allocator_type Alloc, optional_typed_factory_type Factory>
	template <typename... Args>
	constexpr void unique_object<T, Alloc, Factory>::arm(Args&&... args)
		noexcept(is_nothrow_constructible_v<T, Args...>)
			requires (factory_t::valid_factory)
	{
		T* ptr = this->get_allocator().allocate(m_factory->type_size());
		m_factory->construct(ptr, 1, forward<Args>(args)...);

		unique_rsc::arm(
			[factory = m_factory, allocator = this->get_allocator_storage()](T* mem)
		{
			factory->destroy(mem, 1);
			allocator->deallocate(mem, factory->type_size());
		}, ptr);
	}
}

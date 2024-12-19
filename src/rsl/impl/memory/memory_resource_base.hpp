#pragma once

#include "allocator_storage.hpp"
#include "factory_storage.hpp"

namespace rsl
{
	template <
		typename T, allocator_type Alloc = default_allocator, factory_type Factory = default_factory<T>,
		bool AllowRawDataAccess = false>
	class memory_resource_base
	{
	public:
		using value_type = T;
		using ref_type = add_lval_ref_t<T>;
		using const_ref_type = decorate_type_t<T, const_signal, lval_ref_signal>;
		using ptr_type = add_pointer_t<T>;
		using const_ptr_type = decorate_type_t<T, const_signal, pointer_signal>;
		using allocator_storage_type = allocator_storage<Alloc>;
		using allocator_t = Alloc;
		using factory_storage_type = factory_storage<Factory>;
		using factory_t = Factory;

		virtual ~memory_resource_base() = default;

		[[nodiscard]] constexpr ptr_type data() noexcept
			requires AllowRawDataAccess;

		[[nodiscard]] constexpr const_ptr_type data() const noexcept
			requires AllowRawDataAccess;

		void set_allocator(const allocator_storage_type& allocStorage)
			noexcept(is_nothrow_copy_assignable_v<allocator_storage_type>);

		allocator_t& get_allocator() noexcept;
		const allocator_t& get_allocator() const noexcept;

		void set_factory(const factory_storage_type& factoryStorage)
			noexcept(is_nothrow_copy_assignable_v<factory_storage_type>);

		factory_t& get_factory() noexcept;
		const factory_t& get_factory() const noexcept;

	protected:
		constexpr void allocate(size_type n = 1);
		constexpr size_type allocate_at_least(size_type n);
		template <typename... Args>
		constexpr void construct(size_type n = 1, diff_type offset = 0, Args&&... args);
		constexpr void deallocate(size_type n = 1);
		constexpr void destroy();

		allocator_storage_type m_alloc;
		factory_storage_type m_factory;
		ptr_type m_ptr;
	};
} // namespace rsl

#include "memory_resource_base.inl"

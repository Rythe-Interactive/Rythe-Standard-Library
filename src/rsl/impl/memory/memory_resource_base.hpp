#pragma once

#include "allocator_storage.hpp"
#include "factory_storage.hpp"
#include "typed_allocator.hpp"

namespace rsl
{
	template <allocator_type Alloc = default_allocator, untyped_factory_type Factory = type_erased_factory>
	class untyped_memory_resource_base
	{
	public:
		using allocator_storage_type = allocator_storage<Alloc>;
		using allocator_t = Alloc;
		using factory_storage_type = factory_storage<Factory>;
		using factory_t = Factory;
		using typed_alloc_type = type_erased_allocator<Alloc, Factory>;

		virtual ~untyped_memory_resource_base() = default;

		void set_allocator(const allocator_storage_type& allocStorage)
			noexcept(is_nothrow_copy_assignable_v<allocator_storage_type>);

		allocator_t& get_allocator() noexcept;
		const allocator_t& get_allocator() const noexcept;

		void set_factory(const factory_storage_type& factoryStorage)
			noexcept(is_nothrow_copy_assignable_v<factory_storage_type>);

		factory_t& get_factory() noexcept;
		const factory_t& get_factory() const noexcept;

	protected:
		[[rythe_allocating]] [[rythe_always_inline]] constexpr void allocate(size_type count = 1)
			noexcept(factory_traits<Factory>::template noexcept_constructable<>);

		template <typename... Args>
		[[rythe_allocating]] [[rythe_always_inline]] constexpr void
		allocate_aligned(size_type count, size_type alignment)
			noexcept(factory_traits<Factory>::template noexcept_constructable<>);

		template <typename... Args>
		[[rythe_always_inline]] constexpr void construct(size_type count = 1, size_type offset = 0)
			noexcept(factory_traits<Factory>::template noexcept_constructable<>);

		template <typename... Args>
		[[rythe_allocating]] [[rythe_always_inline]] constexpr void reallocate(size_type oldCount, size_type newCount)
			noexcept(
				factory_traits<Factory>::template noexcept_constructable<> && factory_traits<Factory>::noexcept_moveable
			);

		template <typename... Args>
		[[rythe_allocating]] [[rythe_always_inline]] constexpr void
		reallocate_aligned(size_type oldCount, size_type newCount, size_type alignment) noexcept(
			factory_traits<Factory>::template noexcept_constructable<> && factory_traits<Factory>::noexcept_moveable
		);

		[[rythe_always_inline]] constexpr void deallocate(size_type count = 1) noexcept;
		[[rythe_always_inline]] constexpr void deallocate_aligned(size_type count, size_type alignment) noexcept;

		typed_alloc_type m_alloc;
		void* m_ptr = nullptr;
	};


	template <
		typename T, allocator_type Alloc = default_allocator, factory_type Factory = default_factory<T>,
		bool AllowRawDataAccess = false>
	class typed_memory_resource_base
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
		using typed_alloc_type = typed_allocator<T, Alloc, Factory>;

		virtual ~typed_memory_resource_base() = default;

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
		template <typename... Args>
		[[rythe_allocating]] [[rythe_always_inline]] constexpr void allocate(size_type count = 1, Args&&... args)
			noexcept(factory_traits<Factory>::template noexcept_constructable<Args...>);

		template <typename... Args>
		[[rythe_allocating]] [[rythe_always_inline]] constexpr void
		allocate_aligned(size_type count, size_type alignment, Args&&... args)
			noexcept(factory_traits<Factory>::template noexcept_constructable<Args...>);

		template <typename... Args>
		[[rythe_always_inline]] constexpr void construct(size_type count = 1, size_type offset = 0, Args&&... args)
			noexcept(factory_traits<Factory>::template noexcept_constructable<Args...>);

		template <typename... Args>
		[[rythe_allocating]] [[rythe_always_inline]] constexpr void
		reallocate(size_type oldCount, size_type newCount, Args&&... args) noexcept(
			factory_traits<Factory>::template noexcept_constructable<Args...> &&
			factory_traits<Factory>::noexcept_moveable
		);

		template <typename... Args>
		[[rythe_allocating]] [[rythe_always_inline]] constexpr void
		reallocate_aligned(size_type oldCount, size_type newCount, size_type alignment, Args&&... args) noexcept(
			factory_traits<Factory>::template noexcept_constructable<Args...> &&
			factory_traits<Factory>::noexcept_moveable
		);

		[[rythe_always_inline]] constexpr void deallocate(size_type count = 1) noexcept;
		[[rythe_always_inline]] constexpr void deallocate_aligned(size_type count, size_type alignment) noexcept;

		typed_alloc_type m_alloc;
		ptr_type m_ptr = nullptr;
	};
} // namespace rsl

#include "memory_resource_base.inl"

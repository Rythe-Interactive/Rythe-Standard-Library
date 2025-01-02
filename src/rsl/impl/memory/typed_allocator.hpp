#pragma once

#include "allocator_storage.hpp"
#include "factory_storage.hpp"

namespace rsl
{
	template <typename T, allocator_type Alloc = default_allocator, typed_factory_type Factory = default_factory<T>>
	class typed_allocator final
	{
	public:
		using value_type = T;
		using universal_type = Alloc;
		using allocator_storage_type = allocator_storage<Alloc>;
		using allocator_t = Alloc;
		using factory_storage_type = factory_storage<Factory>;
		using factory_t = Factory;

		template <typename Other>
		using retarget = typed_allocator<Other, Alloc, typename Factory::template retarget<Other>>;

		[[rythe_always_inline]] typed_allocator(
		) noexcept(is_nothrow_constructible_v<allocator_storage_type> && is_nothrow_constructible_v<factory_storage_type>) =
			default;

		[[rythe_always_inline]] explicit typed_allocator(const allocator_storage_type& allocStorage
		) noexcept(is_nothrow_copy_constructible_v<allocator_storage_type> && is_nothrow_constructible_v<factory_storage_type>);
		[[rythe_always_inline]] explicit typed_allocator(const factory_storage_type& factoryStorage
		) noexcept(is_nothrow_constructible_v<allocator_storage_type> && is_nothrow_copy_constructible_v<factory_storage_type>);
		[[rythe_always_inline]] typed_allocator(
			const allocator_storage_type& allocStorage, const factory_storage_type& factoryStorage
		) noexcept(is_nothrow_copy_constructible_v<allocator_storage_type> && is_nothrow_copy_constructible_v<factory_storage_type>);

		template <not_same_as<T> Other>
		[[rythe_always_inline]] typed_allocator(const retarget<Other>& other);

		[[rythe_always_inline]] constexpr void set_allocator(const allocator_storage_type& allocStorage)
			noexcept(is_nothrow_copy_assignable_v<allocator_storage_type>);

		[[nodiscard]] [[rythe_always_inline]] constexpr allocator_t& get_allocator() noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const allocator_t& get_allocator() const noexcept;

		[[rythe_always_inline]] constexpr void set_factory(const factory_storage_type& factoryStorage)
			noexcept(is_nothrow_copy_assignable_v<factory_storage_type>);

		[[nodiscard]] [[rythe_always_inline]] constexpr factory_t& get_factory() noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const factory_t& get_factory() const noexcept;

		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr T* allocate(size_type count = 1) noexcept;
		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr T*
		allocate(size_type count, size_type alignment) noexcept;
		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr T*
		reallocate(T* ptr, size_type oldCount, size_type newCount) noexcept(factory_traits<Factory>::noexcept_moveable);
		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr T*
		reallocate(T* ptr, size_type oldCount, size_type newCount, size_type alignment)
			noexcept(factory_traits<Factory>::noexcept_moveable);
		[[rythe_always_inline]] constexpr void deallocate(T* ptr, size_type count = 1) noexcept;
		[[rythe_always_inline]] constexpr void deallocate(T* ptr, size_type count, size_type alignment) noexcept;

		template <typename... Args>
		[[rythe_always_inline]] constexpr T* construct(T* ptr, size_type count = 1, Args&&... args)
			noexcept(factory_traits<Factory>::template noexcept_constructable<Args...>);
		[[rythe_always_inline]] constexpr T* move(T* dst, T* src, size_type count = 1)
			noexcept(factory_traits<Factory>::noexcept_moveable);
		[[rythe_always_inline]] constexpr void destroy(T* ptr, size_type count = 1) noexcept;

		template <typename... Args>
		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr T*
		allocate_and_construct(size_type count = 1, Args&&... args)
			noexcept(factory_traits<Factory>::template noexcept_constructable<Args...>);

		template <typename... Args>
		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr T*
		allocate_aligned_and_construct(size_type count, size_type alignment, Args&&... args)
			noexcept(factory_traits<Factory>::template noexcept_constructable<Args...>);

		template <typename... Args>
		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr T*
		reallocate_and_construct(T* ptr, size_type oldCount, size_type newCount, Args&&... args) noexcept(
			factory_traits<Factory>::template noexcept_constructable<Args...> &&
			factory_traits<Factory>::noexcept_moveable
		);

		template <typename... Args>
		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr T* reallocate_aligned_and_construct(
			T* ptr, size_type oldCount, size_type newCount, size_type alignment, Args&&... args
		)
			noexcept(
				factory_traits<Factory>::template noexcept_constructable<Args...> &&
				factory_traits<Factory>::noexcept_moveable
			);

		[[rythe_always_inline]] constexpr void destroy_and_deallocate(T* ptr, size_type count = 1) noexcept;
		[[rythe_always_inline]] constexpr void
		destroy_and_deallocate_aligned(T* ptr, size_type count, size_type alignment) noexcept;

		[[rythe_always_inline]] constexpr allocator_storage_type& get_allocator_storage() noexcept;
		[[rythe_always_inline]] constexpr const allocator_storage_type& get_allocator_storage() const noexcept;

		[[rythe_always_inline]] constexpr factory_storage_type& get_factory_storage() noexcept;
		[[rythe_always_inline]] constexpr const factory_storage_type& get_factory_storage() const noexcept;

	private:
		allocator_storage_type m_alloc;
		factory_storage_type m_factory;
	};

	template <allocator_type Alloc = default_allocator, untyped_factory_type Factory = type_erased_factory>
	class type_erased_allocator
	{
	public:
		using universal_type = Alloc;
		using allocator_storage_type = allocator_storage<Alloc>;
		using allocator_t = Alloc;
		using factory_storage_type = factory_storage<Factory>;
		using factory_t = Factory;

		template <typename Other>
		using retarget = type_erased_allocator<Alloc>;

		type_erased_allocator() = default;
        template<typename T>
		type_erased_allocator(const allocator_storage_type& allocStorage, construct_type_signal_type<T>)
			noexcept(is_nothrow_copy_constructible_v<allocator_storage_type> && is_nothrow_constructible_v<factory_t, construct_type_signal_type<T>> && is_nothrow_constructible_v<factory_storage_type, factory_t&&>);
		type_erased_allocator(const factory_storage_type& factoryStorage
		) noexcept(is_nothrow_constructible_v<allocator_storage_type> && is_nothrow_copy_constructible_v<factory_storage_type>);
		type_erased_allocator(
			const allocator_storage_type& allocStorage, const factory_storage_type& factoryStorage
		) noexcept(is_nothrow_copy_constructible_v<allocator_storage_type> && is_nothrow_copy_constructible_v<factory_storage_type>);

		[[rythe_always_inline]] constexpr void set_allocator(const allocator_storage_type& allocStorage)
			noexcept(is_nothrow_copy_assignable_v<allocator_storage_type>);

		[[nodiscard]] [[rythe_always_inline]] constexpr allocator_t& get_allocator() noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const allocator_t& get_allocator() const noexcept;

		[[rythe_always_inline]] constexpr void set_factory(const factory_storage_type& factoryStorage)
			noexcept(is_nothrow_copy_assignable_v<factory_storage_type>);

		[[nodiscard]] [[rythe_always_inline]] constexpr factory_t& get_factory() noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const factory_t& get_factory() const noexcept;

		[[nodiscard]] [[rythe_always_inline]] size_type typeSize() const noexcept;
		[[nodiscard]] [[rythe_always_inline]] bool trivialCopy() const noexcept;
		[[nodiscard]] [[rythe_always_inline]] id_type typeId() const noexcept;

		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr void*
		allocate(size_type count = 1) noexcept;
		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr void*
		allocate(size_type count, size_type alignment) noexcept;
		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr void*
		reallocate(void* ptr, size_type oldCount, size_type newCount)
			noexcept(factory_traits<Factory>::noexcept_moveable);
		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr void*
		reallocate(void* ptr, size_type oldCount, size_type newCount, size_type alignment)
			noexcept(factory_traits<Factory>::noexcept_moveable);
		[[rythe_always_inline]] constexpr void deallocate(void* ptr, size_type count = 1) noexcept;
		[[rythe_always_inline]] constexpr void deallocate(void* ptr, size_type count, size_type alignment) noexcept;

		[[rythe_always_inline]] constexpr void* construct(void* ptr, size_type count = 1)
			noexcept(factory_traits<Factory>::template noexcept_constructable<>);
		[[rythe_always_inline]] constexpr void* move(void* dst, void* src, size_type count = 1)
			noexcept(factory_traits<Factory>::noexcept_moveable);
		[[rythe_always_inline]] constexpr void destroy(void* ptr, size_type count = 1) noexcept;

		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr void*
		allocate_and_construct(size_type count = 1)
			noexcept(factory_traits<Factory>::template noexcept_constructable<>);

		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr void*
		allocate_aligned_and_construct(size_type count, size_type alignment)
			noexcept(factory_traits<Factory>::template noexcept_constructable<>);

		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr void*
		reallocate_and_construct(void* ptr, size_type oldCount, size_type newCount) noexcept(
			factory_traits<Factory>::template noexcept_constructable<> && factory_traits<Factory>::noexcept_moveable
		);

		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr void*
		reallocate_aligned_and_construct(void* ptr, size_type oldCount, size_type newCount, size_type alignment)
			noexcept(
				factory_traits<Factory>::template noexcept_constructable<> && factory_traits<Factory>::noexcept_moveable
			);

		[[rythe_always_inline]] constexpr void destroy_and_deallocate(void* ptr, size_type count = 1) noexcept;
		[[rythe_always_inline]] constexpr void
		destroy_and_deallocate_aligned(void* ptr, size_type count, size_type alignment) noexcept;

		[[nodiscard]] [[rythe_always_inline]] constexpr allocator_storage_type& get_allocator_storage() noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const allocator_storage_type&
		get_allocator_storage() const noexcept;

		[[nodiscard]] [[rythe_always_inline]] constexpr factory_storage_type& get_factory_storage() noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const factory_storage_type&
		get_factory_storage() const noexcept;

	private:
		allocator_storage_type m_alloc;
		factory_storage_type m_factory;
	};
} // namespace rsl

#include "typed_allocator.inl"

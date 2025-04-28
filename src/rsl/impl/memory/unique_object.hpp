#pragma once
#include "unique_resource.hpp"

namespace rsl
{
	template <typename T, allocator_type Alloc = default_allocator, typed_factory_type Factory = default_factory<T>>
	class unique_object : private unique_resource<T*, Alloc>
	{
	public:
		using unique_rsc = unique_resource<T*, Alloc>;
		using mem_rsc = typename unique_rsc::mem_rsc;
		using allocator_storage_type = typename mem_rsc::allocator_storage_type;
		using allocator_t = typename mem_rsc::allocator_t;
		using factory_storage_type = factory_storage<Factory>;
		using factory_t = Factory;

		[[rythe_always_inline]] constexpr unique_object(nullptr_type)
			noexcept(is_nothrow_constructible_v<mem_rsc>);

		[[rythe_always_inline]] explicit unique_object(const allocator_storage_type& allocStorage)
			noexcept(is_nothrow_constructible_v<mem_rsc, const allocator_storage_type&>);

		[[rythe_always_inline]] explicit unique_object(const factory_storage_type& factoryStorage)
			noexcept(is_nothrow_constructible_v<mem_rsc>);

		[[rythe_always_inline]] explicit unique_object(const allocator_storage_type& allocStorage, const factory_storage_type& factoryStorage)
			noexcept(is_nothrow_constructible_v<mem_rsc, const allocator_storage_type&>);

		template <typename... Args>
		[[rythe_always_inline]] constexpr explicit unique_object(in_place_signal_type, Args&&... args)
			noexcept(is_nothrow_constructible_v<mem_rsc> && is_nothrow_constructible_v<T, Args...>);

		template <typename... Args>
		[[rythe_always_inline]] unique_object(const allocator_storage_type& allocStorage,
		                                      in_place_signal_type, Args&&... args)
			noexcept(is_nothrow_constructible_v<mem_rsc, const allocator_storage_type&> &&
			is_nothrow_constructible_v<T, Args...>);

		template <typename... Args>
		[[rythe_always_inline]] unique_object(const allocator_storage_type& allocStorage, const factory_storage_type& factoryStorage,
											  in_place_signal_type, Args&&... args)
			noexcept(is_nothrow_constructible_v<mem_rsc, const allocator_storage_type&> &&
				is_nothrow_constructible_v<T, Args...>);

		[[rythe_always_inline]] constexpr unique_object() noexcept = default;
		unique_object(const unique_object&) = delete;
		[[rythe_always_inline]] constexpr unique_object(unique_object&& other) noexcept;

		template <typename OtherType, typed_factory_type OtherFactory>
			requires (is_pointer_assignable_v<T, OtherType>)
		[[rythe_always_inline]] constexpr unique_object(unique_object<OtherType, Alloc, OtherFactory>&& other) noexcept;

		[[rythe_always_inline]] constexpr unique_object& operator=(unique_object&& other) noexcept;

		template <typename OtherType, typed_factory_type OtherFactory>
			requires (is_pointer_assignable_v<T, OtherType>)
		[[rythe_always_inline]] constexpr unique_object& operator=(unique_object<OtherType, Alloc, OtherFactory>&& other) noexcept;

		[[rythe_always_inline]] constexpr void set_factory(const factory_storage_type& factoryStorage)
			noexcept(is_nothrow_copy_assignable_v<factory_storage_type>) { unique_rsc::set_factory(factoryStorage); }

		[[nodiscard]] [[rythe_always_inline]] constexpr factory_t& get_factory() noexcept
		{
			return unique_rsc::get_factory();
		}

		[[nodiscard]] [[rythe_always_inline]] constexpr const factory_t& get_factory() const noexcept
		{
			return unique_rsc::get_factory();
		}

		[[nodiscard]] [[rythe_always_inline]] constexpr factory_storage_type& get_factory_storage() noexcept
		{
			return unique_rsc::get_factory_storage();
		}

		[[nodiscard]] [[rythe_always_inline]] constexpr const factory_storage_type& get_factory_storage() const noexcept
		{
			return unique_rsc::get_factory_storage();
		}

		[[nodiscard]] [[rythe_always_inline]] constexpr bool is_armed() const noexcept
		{
			return unique_rsc::is_armed();
		}

		[[rythe_always_inline]] constexpr void disarm() noexcept { unique_rsc::disarm(); }

		[[rythe_always_inline]] constexpr T* get() noexcept { return *unique_rsc::get(); }
		[[rythe_always_inline]] constexpr const T* get() const noexcept { return *unique_rsc::get(); }

		[[rythe_always_inline]] constexpr T& operator*() noexcept { return **unique_rsc::get(); }
		[[rythe_always_inline]] constexpr const T& operator*() const noexcept { return **unique_rsc::get(); }
		[[rythe_always_inline]] constexpr T* operator->() noexcept { return *unique_rsc::get(); }
		[[rythe_always_inline]] constexpr const T* operator->() const noexcept { return *unique_rsc::get(); }

	private:
		template <typename FriendT, allocator_type FriendAlloc, typed_factory_type FriendFactory>
		friend class unique_object;

		template <typename... Args>
		[[rythe_always_inline]] constexpr void arm(Args&&... args) noexcept(is_nothrow_constructible_v<T, Args...>);

		factory_storage_type m_factory;
	};
}

#include "unique_object.inl"

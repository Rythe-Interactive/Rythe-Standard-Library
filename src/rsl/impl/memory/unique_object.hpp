#pragma once
#include "unique_resource.hpp"

namespace rsl
{
	template <typename T, allocator_type Alloc = default_allocator, statically_optional_typed_factory_type Factory = default_factory<T>>
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
		[[rythe_always_inline]] constexpr static unique_object create_in_place(Args&&... args)
			noexcept(is_nothrow_constructible_v<mem_rsc> && is_nothrow_constructible_v<T, Args...>);

		template <typename... Args>
		[[rythe_always_inline]] static unique_object create_in_place_with_allocator(const allocator_storage_type& allocStorage, Args&&... args)
			noexcept(is_nothrow_constructible_v<mem_rsc, const allocator_storage_type&> &&
			is_nothrow_constructible_v<T, Args...>);

		template <typename... Args>
		[[rythe_always_inline]] static unique_object create_in_place_alloc_factory(const allocator_storage_type& allocStorage, const factory_storage_type& factoryStorage, Args&&... args)
			noexcept(is_nothrow_constructible_v<mem_rsc, const allocator_storage_type&> &&
				is_nothrow_constructible_v<T, Args...>);

		[[rythe_always_inline]] constexpr unique_object() noexcept = default;
		unique_object(const unique_object&) = delete;
		[[rythe_always_inline]] constexpr unique_object(unique_object&& other) noexcept;

		template <typename OtherType, statically_optional_typed_factory_type OtherFactory>
			requires (is_pointer_assignable_v<T, OtherType>)
		[[rythe_always_inline]] constexpr unique_object(unique_object<OtherType, Alloc, OtherFactory>&& other) noexcept; // NOLINT(*-explicit-constructor)

		[[rythe_always_inline]] constexpr unique_object& operator=(unique_object&& other) noexcept;

		template <typename OtherType, statically_optional_typed_factory_type OtherFactory>
			requires (is_pointer_assignable_v<T, OtherType>)
		[[rythe_always_inline]] constexpr unique_object& operator=(unique_object<OtherType, Alloc, OtherFactory>&& other) noexcept;

		[[rythe_always_inline]] constexpr void set_factory(const factory_storage_type& factoryStorage)
			noexcept(is_nothrow_copy_assignable_v<factory_storage_type>) { unique_rsc::set_factory(factoryStorage); }

		[[nodiscard]] [[rythe_always_inline]] constexpr factory_t& get_factory() noexcept
		{
			return *m_factory;
		}

		[[nodiscard]] [[rythe_always_inline]] constexpr const factory_t& get_factory() const noexcept
		{
			return *m_factory;
		}

		[[nodiscard]] [[rythe_always_inline]] constexpr factory_storage_type& get_factory_storage() noexcept
		{
			return m_factory;
		}

		[[nodiscard]] [[rythe_always_inline]] constexpr const factory_storage_type& get_factory_storage() const noexcept
		{
			return m_factory;
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
		template <typename FriendT, allocator_type FriendAlloc, statically_optional_typed_factory_type FriendFactory>
		friend class unique_object;

		struct deleter
		{
			factory_storage_type factory;
			allocator_storage_type allocator;
			void operator()(T* mem) noexcept;

			operator bool() const noexcept { return factory && allocator; }
		};

		template <typename... Args>
		[[rythe_always_inline]] constexpr void arm(Args&&... args) noexcept(is_nothrow_constructible_v<T, Args...>)
			requires (factory_t::valid_factory);

		factory_storage_type m_factory;
	};

	// TODO(Glyn): Create `temporary_object` that when moved from will invalidate itself.
	// Effectively the same as `unique_object&&` but with clearer ownership transfer.
	// Crucially allows for `view<temporary_object>` to be used to move `unique_object`s into containers.
	template <typename T, allocator_type Alloc = default_allocator, statically_optional_typed_factory_type Factory = default_factory<T>>
	using temporary_object = unique_object<T, Alloc, Factory>;
}

#include "unique_object.inl"

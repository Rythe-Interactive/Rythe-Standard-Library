#pragma once
#include "reference_counter.hpp"

namespace rsl
{
	namespace internal
	{
		struct managed_payload_base : public manual_reference_counter
		{
			virtual void destroy(void*) noexcept { rsl_assert_unreachable(); }
		};

		template <typename T, typename Deleter>
		struct managed_payload final : public managed_payload_base
		{
			Deleter deleter;

			void destroy(void* value) noexcept override
			{
				if (deleter)
				{
					deleter(*static_cast<T*>(value));
				}
			}
		};
	} // namespace internal

	template <typename T, allocator_type Alloc = default_allocator, untyped_factory_type Factory = type_erased_factory>
	class managed_resource : public basic_reference_counter<internal::managed_payload_base, Alloc, Factory>
	{
	public:
		using ref_counter = basic_reference_counter<internal::managed_payload_base, Alloc, Factory>;
		using mem_rsc = ref_counter::mem_rsc;

		using allocator_storage_type = ref_counter::allocator_storage_type;
		using allocator_t = ref_counter::allocator_t;
		using factory_storage_type = ref_counter::factory_storage_type;
		using factory_t = ref_counter::factory_t;

		[[rythe_always_inline]] constexpr managed_resource(nullptr_type)
			noexcept(is_nothrow_constructible_v<ref_counter>);

		[[rythe_always_inline]] explicit managed_resource(const allocator_storage_type& allocStorage)
			noexcept(is_nothrow_constructible_v<ref_counter, const allocator_storage_type&>);

		template <typename Deleter, typename... Args>
		[[rythe_always_inline]] constexpr explicit managed_resource(Deleter deleter, Args&&... args)
			noexcept(is_nothrow_constructible_v<ref_counter> && is_nothrow_constructible_v<T, Args...>);

		template <typename Deleter, typename... Args>
		[[rythe_always_inline]] managed_resource(
			const allocator_storage_type& allocStorage, Deleter deleter, Args&&... args
		) noexcept(is_nothrow_constructible_v<ref_counter, const allocator_storage_type&> && is_nothrow_constructible_v<T, Args...>);

		[[rythe_always_inline]] constexpr managed_resource() noexcept = default;

		[[rythe_always_inline]] constexpr ~managed_resource() noexcept;

		template <typename Deleter, typename... Args>
		[[rythe_always_inline]] constexpr void arm(Deleter deleter, Args&&... args)
			noexcept(is_nothrow_constructible_v<T, Args...>);

		[[rythe_always_inline]] constexpr T* get() noexcept { return &m_value; }
		[[rythe_always_inline]] constexpr const T* get() const noexcept { return &m_value; }

		[[rythe_always_inline]] constexpr T& operator*() noexcept { return m_value; }
		[[rythe_always_inline]] constexpr const T& operator*() const noexcept { return m_value; }
		[[rythe_always_inline]] constexpr T* operator->() noexcept { return &m_value; }
		[[rythe_always_inline]] constexpr const T* operator->() const noexcept { return &m_value; }

	private:
		union
		{
			T m_value;
			byte m_dummy;
		};
	};
} // namespace rsl

#include "managed_resource.inl"

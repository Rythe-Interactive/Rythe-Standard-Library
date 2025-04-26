#pragma once
#include "../containers/optional.hpp"

namespace rsl
{
	namespace internal
	{
		struct unique_payload_base
		{
			virtual ~unique_payload_base() = default;
			virtual void destroy(void*) noexcept { rsl_assert_unreachable(); }
		};

		template <typename T, typename Deleter>
		struct unique_payload final : public unique_payload_base
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
	}

	template <typename T, allocator_type Alloc = default_allocator, untyped_factory_type Factory = type_erased_factory>
	class unique_resource : public internal::select_memory_resource<internal::unique_payload_base, Alloc, Factory>::type
	{
	public:
		constexpr static bool untyped_memory_resource =
			internal::select_memory_resource<internal::unique_payload_base, Alloc, Factory>::is_untyped;
		using mem_rsc = typename internal::select_memory_resource<internal::unique_payload_base, Alloc, Factory>::type;
		using allocator_storage_type = typename mem_rsc::allocator_storage_type;
		using allocator_t = typename mem_rsc::allocator_t;
		using factory_storage_type = typename mem_rsc::factory_storage_type;
		using factory_t = typename mem_rsc::factory_t;

		[[rythe_always_inline]] constexpr unique_resource(nullptr_type)
			noexcept(is_nothrow_constructible_v<mem_rsc>);

		[[rythe_always_inline]] explicit unique_resource(const allocator_storage_type& allocStorage)
			noexcept(is_nothrow_constructible_v<mem_rsc, const allocator_storage_type&>);

		template <typename Deleter, typename... Args>
		[[rythe_always_inline]] constexpr explicit unique_resource(Deleter deleter, Args&&... args)
			noexcept(is_nothrow_constructible_v<mem_rsc> && is_nothrow_constructible_v<T, Args...>);

		template <typename Deleter, typename... Args>
		[[rythe_always_inline]] unique_resource(
			const allocator_storage_type& allocStorage, Deleter deleter, Args&&... args
		) noexcept(is_nothrow_constructible_v<mem_rsc, const allocator_storage_type&> && is_nothrow_constructible_v<T, Args...>);

		[[rythe_always_inline]] constexpr unique_resource() noexcept = default;

		[[rythe_always_inline]] constexpr ~unique_resource() noexcept;

		template <typename Deleter, typename... Args>
		[[rythe_always_inline]] constexpr void arm(Deleter deleter, Args&&... args)
			noexcept(is_nothrow_constructible_v<T, Args...>);

		[[nodiscard]] [[rythe_always_inline]] constexpr bool is_armed() const noexcept;

		// Calls deleter and release
		[[rythe_always_inline]] constexpr void disarm() noexcept;

		// Releases held object without calling deleter
		[[rythe_always_inline]] constexpr void release() noexcept;

		[[rythe_always_inline]] constexpr T* get() noexcept { return &*m_value; }
		[[rythe_always_inline]] constexpr const T* get() const noexcept { return &*m_value; }

		[[rythe_always_inline]] constexpr T& operator*() noexcept { return *m_value; }
		[[rythe_always_inline]] constexpr const T& operator*() const noexcept { return *m_value; }
		[[rythe_always_inline]] constexpr T* operator->() noexcept { return &*m_value; }
		[[rythe_always_inline]] constexpr const T* operator->() const noexcept { return &*m_value; }

	private:
		optional<T> m_value;
	};
} // namespace rsl

#include "unique_resource.inl"

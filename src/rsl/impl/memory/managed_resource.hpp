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

	template <typename T, allocator_type Alloc = default_allocator, factory_type Factory = default_factory<T>>
	class managed_resource :
		public basic_reference_counter<
			internal::managed_payload_base, Alloc, typename Factory::template retarget<internal::managed_payload_base>>
	{
	protected:
		using ref_counter = basic_reference_counter<
			internal::managed_payload_base, Alloc, typename Factory::template retarget<internal::managed_payload_base>>;
		using mem_rsc = ref_counter::mem_rsc;

	public:
		[[rythe_always_inline]] constexpr managed_resource(nullptr_type) noexcept;

		template <typename Deleter, typename... Args>
		[[rythe_always_inline]] constexpr explicit managed_resource(Deleter deleter, Args&&... args) noexcept;

		[[rythe_always_inline]] constexpr managed_resource() noexcept = default;

		[[rythe_always_inline]] constexpr ~managed_resource() noexcept;

		[[rythe_always_inline]] constexpr T* get() noexcept { return &m_value; }
		[[rythe_always_inline]] constexpr const T* get() const noexcept { return &m_value; }

		[[rythe_always_inline]] constexpr T& operator*() noexcept { return m_value; }
		[[rythe_always_inline]] constexpr const T& operator*() const noexcept { return m_value; }
		[[rythe_always_inline]] constexpr T* operator->() noexcept { return &m_value; }
		[[rythe_always_inline]] constexpr const T* operator->() const noexcept { return &m_value; }

	private:
		T m_value;
	};
} // namespace rsl

#include "managed_resource.inl"

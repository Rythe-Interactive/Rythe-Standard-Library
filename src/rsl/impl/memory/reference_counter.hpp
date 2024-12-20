#pragma once

#include "../util/assert.hpp"
#include "../util/primitives.hpp"
#include "memory_resource_base.hpp"

namespace rsl
{
	template <typename T>
	concept reference_counted = requires(T& val) {
		{ val.borrow() };
		{ val.release() };
		{ val.count() } -> convertible_to<size_type>;
		{ val.occupied() } -> convertible_to<bool>;
		{ val.free() } -> convertible_to<bool>;
	};

	class manual_reference_counter
	{
	public:
		[[rythe_always_inline]] constexpr size_type borrow() noexcept;
		[[rythe_always_inline]] constexpr void release() noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr size_type count() const noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr bool occupied() const noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr bool free() const noexcept;

	private:
		size_type m_count = 0;
	};

    template<typename T>
	struct reference_counted_payload final : public manual_reference_counter
    {
		T value;
    };

    struct arm_signal_type
	{
	};

    constexpr arm_signal_type arm_signal;

	template <reference_counted Counter = manual_reference_counter, allocator_type Alloc = default_allocator, factory_type Factory = default_factory<Counter>>
	class basic_reference_counter : public memory_resource_base<Counter, Alloc, Factory, false>
	{
	protected:
		using mem_rsc = memory_resource_base<Counter, Alloc, Factory>;
		using allocator_storage_type = mem_rsc::allocator_storage_type;
		using allocator_t = mem_rsc::allocator_t;
		using factory_storage_type = mem_rsc::factory_storage_type;
		using factory_t = mem_rsc::factory_t;

	public:
		[[rythe_always_inline]] constexpr basic_reference_counter() noexcept = default;
		[[rythe_always_inline]] constexpr basic_reference_counter(arm_signal_type) noexcept;
		[[rythe_always_inline]] constexpr basic_reference_counter(const basic_reference_counter& other) noexcept;
		[[rythe_always_inline]] constexpr basic_reference_counter(basic_reference_counter&& other) noexcept;
		[[rythe_always_inline]] constexpr basic_reference_counter& operator=(const basic_reference_counter& other) noexcept;
		[[rythe_always_inline]] constexpr basic_reference_counter& operator=(basic_reference_counter&& other) noexcept;
		[[rythe_always_inline]] constexpr ~basic_reference_counter() noexcept;

        [[rythe_always_inline]] constexpr void arm() noexcept;
        [[rythe_always_inline]] constexpr void disarm() noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr bool is_armed() const noexcept;

		[[rythe_always_inline]] constexpr size_type borrow() noexcept;
		[[rythe_always_inline]] constexpr void release() noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr size_type count() const noexcept;

		// These versions of occupied and free don't count the reference you call this on.
		[[nodiscard]] [[rythe_always_inline]] constexpr bool occupied() const noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr bool free() const noexcept;

        protected:
		[[rythe_always_inline]] constexpr void arm(Counter* ptr) noexcept;
	};

    using reference_counter = basic_reference_counter<>;
} // namespace rsl

#include "reference_counter.inl"

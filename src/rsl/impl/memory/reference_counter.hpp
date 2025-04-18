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

	template <typename T>
	struct reference_counted_payload final : public manual_reference_counter
	{
		T value;
	};

	struct arm_signal_type
	{
	};

	constexpr arm_signal_type arm_signal;

	namespace internal
	{
		template <typename T, allocator_type Alloc, factory_type Factory, bool AllowRawDataAccess, bool Untyped>
		struct select_memory_resource
		{
			using type = typed_memory_resource_base<T, Alloc, Factory, AllowRawDataAccess>;
		};

		template <typename T, allocator_type Alloc, factory_type Factory, bool AllowRawDataAccess>
		struct select_memory_resource<T, Alloc, Factory, AllowRawDataAccess, true>
		{
			using type = untyped_memory_resource_base<Alloc, Factory>;
		};
	} // namespace internal

	template <
		reference_counted Counter = manual_reference_counter, allocator_type Alloc = default_allocator,
		factory_type Factory = default_factory<Counter>>
	class basic_reference_counter :
		public internal::select_memory_resource<Counter, Alloc, Factory, false, untyped_factory_type<Factory>>::type
	{
	protected:
		constexpr static bool untypedMemoryResource = untyped_factory_type<Factory>;
		using mem_rsc =
			internal::select_memory_resource<Counter, Alloc, Factory, false, untyped_factory_type<Factory>>::type;
		using allocator_storage_type = mem_rsc::allocator_storage_type;
		using allocator_t = mem_rsc::allocator_t;
		using factory_storage_type = mem_rsc::factory_storage_type;
		using factory_t = mem_rsc::factory_t;

	public:
		[[rythe_always_inline]] constexpr basic_reference_counter() noexcept = default;
		[[rythe_always_inline]] constexpr basic_reference_counter(arm_signal_type) noexcept;
		[[rythe_always_inline]] constexpr basic_reference_counter(const basic_reference_counter& other) noexcept;
		[[rythe_always_inline]] constexpr basic_reference_counter(basic_reference_counter&& other) noexcept;
		[[rythe_always_inline]] constexpr basic_reference_counter& operator=(const basic_reference_counter& other
		) noexcept;
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
		[[nodiscard]] [[rythe_always_inline]] constexpr Counter* get_ptr() noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const Counter* get_ptr() const noexcept;
		[[rythe_always_inline]] constexpr void set_ptr(Counter* ptr) noexcept;
	};

	using reference_counter = basic_reference_counter<>;
} // namespace rsl

#include "reference_counter.inl"

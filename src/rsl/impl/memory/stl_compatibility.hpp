#pragma once

#include "typed_allocator.hpp"

namespace rsl
{
	template <specialization_of<typed_allocator> Alloc>
	struct stl_allocator_compatible_wrapper
	{
		Alloc alloc;
		using value_type = Alloc::value_type;

		template <typename Other>
		struct rebind
		{
			using other = stl_allocator_compatible_wrapper<typename Alloc::template retarget<Other>>;
		};

		constexpr stl_allocator_compatible_wrapper() noexcept {}

		constexpr stl_allocator_compatible_wrapper(const stl_allocator_compatible_wrapper&) noexcept = default;
		constexpr stl_allocator_compatible_wrapper(const Alloc& other) noexcept;

		template <specialization_of<typed_allocator> Other>
		constexpr stl_allocator_compatible_wrapper(const stl_allocator_compatible_wrapper<Other>& other) noexcept
			requires is_constructible_v<Alloc, const Other&>;

		constexpr ~stl_allocator_compatible_wrapper() = default;
		constexpr stl_allocator_compatible_wrapper& operator=(const stl_allocator_compatible_wrapper&) = default;

		[[rythe_always_inline]] constexpr void deallocate(value_type* const ptr, const size_t count) noexcept;
		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr value_type* allocate(const size_t count
		) noexcept;
	};

	template <specialization_of<typed_allocator> Alloc>
	struct stl_deleter_compatible_wrapper
	{
		Alloc alloc;
		using value_type = Alloc::value_type;

		template <typename Other>
		struct rebind
		{
			using other = stl_deleter_compatible_wrapper<typename Alloc::template retarget<Other>>;
		};

		[[rythe_always_inline]] constexpr void operator()(value_type* ptr) const noexcept;
	};

	template <typename T>
	using stl_pmu_alloc = stl_allocator_compatible_wrapper<typed_allocator<T, pmu_alloc_ptr_wrapper>>;

	template <typename T>
	using stl_pmu_deleter = stl_deleter_compatible_wrapper<typed_allocator<T, pmu_alloc_ptr_wrapper>>;

	template <typename T>
	[[nodiscard]] [[rythe_always_inline]] constexpr stl_pmu_deleter<T> make_stl_pmu_deleter(pmu_allocator* alloc
	) noexcept;
} // namespace rsl

#include "stl_compatibility.inl"

#pragma once
#include "stl_compatibility.hpp"

namespace rsl
{
	template <specialization_of<typed_allocator> Alloc>
	inline constexpr stl_allocator_compatible_wrapper<Alloc>::stl_allocator_compatible_wrapper(const Alloc& other
	) noexcept
		: alloc(other)
	{
	}

	template <specialization_of<typed_allocator> Alloc>
	template <not_same_as<typename Alloc::value_type> Other>
	constexpr stl_allocator_compatible_wrapper<Alloc>::stl_allocator_compatible_wrapper(
		const stl_allocator_compatible_wrapper<typename Alloc::template retarget<Other>>& other
	) noexcept
		: alloc(other.alloc)
	{
	}

	template <specialization_of<typed_allocator> Alloc>
	inline constexpr void
	stl_allocator_compatible_wrapper<Alloc>::deallocate(value_type* const ptr, const size_t count) noexcept
	{
		alloc.get_allocator().deallocate(ptr, count * sizeof(value_type));
	}

	template <specialization_of<typed_allocator> Alloc>
	inline constexpr Alloc::value_type* stl_allocator_compatible_wrapper<Alloc>::allocate(const size_t count) noexcept
	{
		return static_cast<value_type*>(alloc.get_allocator().allocate(count * sizeof(value_type)));
	}

	template <specialization_of<typed_allocator> Alloc>
	inline constexpr void stl_deleter_compatible_wrapper<Alloc>::operator()(value_type* ptr) const noexcept
	{
		alloc.deallocate(ptr);
	}

	template <typename T>
	[[nodiscard]] [[rythe_always_inline]] constexpr stl_pmu_deleter<T> make_stl_pmu_deleter(pmu_allocator* alloc
	) noexcept
	{
		return stl_pmu_deleter<T>{typed_allocator<T, pmu_alloc_ptr_wrapper>{pmu_alloc_ptr_wrapper{alloc}}};
	}
} // namespace rsl

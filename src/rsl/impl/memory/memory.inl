#pragma once
#include "memory.hpp"

namespace rsl
{
	template <typename T, typename... Args>
	T* allocate(pmu_allocator& alloc, size_type count, Args&&... args)
		noexcept(default_factory<T>::template noexcept_constructable<Args...>)
	{
		using allocator_type = allocator<T, pmu_alloc_ptr_wrapper>;

		allocator_type wrappedAlloc{pmu_alloc_ptr_wrapper(&alloc)};
		return wrappedAlloc.allocate(count, forward<Args>(args)...);
	}

	template <typename T, typename... Args>
	T* allocate(pmu_allocator& alloc, size_type count, size_type alignment, Args&&... args)
		noexcept(default_factory<T>::template noexcept_constructable<Args...>)
	{
		using allocator_type = allocator<T, pmu_alloc_ptr_wrapper>;

		allocator_type wrappedAlloc{pmu_alloc_ptr_wrapper(&alloc)};
		return wrappedAlloc.allocate(count, alignment, forward<Args>(args)...);
	}

	template <typename T, typename... Args>
	T* reallocate(pmu_allocator& alloc, T* ptr, size_type oldCount, size_type newCount, Args&&... args)
		noexcept(default_factory<T>::template noexcept_constructable<Args...> && default_factory<T>::noexcept_moveable)
	{
		using allocator_type = allocator<T, pmu_alloc_ptr_wrapper>;

		allocator_type wrappedAlloc{pmu_alloc_ptr_wrapper(&alloc)};
		return wrappedAlloc.reallocate(ptr, oldCount, newCount, forward<Args>(args)...);
	}

	template <typename T, typename... Args>
	T* reallocate(
		pmu_allocator& alloc, T* ptr, size_type oldCount, size_type newCount, size_type alignment, Args&&... args
	) noexcept(default_factory<T>::template noexcept_constructable<Args...> && default_factory<T>::noexcept_moveable)
	{
		using allocator_type = allocator<T, pmu_alloc_ptr_wrapper>;

		allocator_type wrappedAlloc{pmu_alloc_ptr_wrapper(&alloc)};
		return wrappedAlloc.reallocate(ptr, oldCount, newCount, alignment, forward<Args>(args)...);
	}

	template <typename T>
	void deallocate(pmu_allocator& alloc, T* ptr, size_type count) noexcept
	{
		using allocator_type = allocator<T, pmu_alloc_ptr_wrapper>;

		allocator_type wrappedAlloc{pmu_alloc_ptr_wrapper(&alloc)};
		return wrappedAlloc.deallocate(ptr, count);
	}

	template <typename T>
	void deallocate(pmu_allocator& alloc, T* ptr, size_type count, size_type alignment) noexcept
	{
		using allocator_type = allocator<T, pmu_alloc_ptr_wrapper>;

		allocator_type wrappedAlloc{pmu_alloc_ptr_wrapper(&alloc)};
		return wrappedAlloc.deallocate(ptr, count, alignment);
	}
} // namespace rsl

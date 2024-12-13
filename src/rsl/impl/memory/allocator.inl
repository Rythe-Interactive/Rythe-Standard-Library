#pragma once
#include "allocator.hpp"

namespace rsl
{
	inline void* polymorphic_universal_allocator_ptr_wrapper::allocate(size_type size) noexcept
	{
		return allocatorPtr->allocate(size);
	}

	inline void* polymorphic_universal_allocator_ptr_wrapper::allocate(size_type size, size_type alignment) noexcept
	{
		return allocatorPtr->allocate(size, alignment);
	}

	inline void*
	polymorphic_universal_allocator_ptr_wrapper::reallocate(void* ptr, size_type oldSize, size_type newSize) noexcept
	{
		return allocatorPtr->reallocate(ptr, oldSize, newSize);
	}

	inline void* polymorphic_universal_allocator_ptr_wrapper::reallocate(
		void* ptr, size_type oldSize, size_type newSize, size_type alignment
	) noexcept
	{
		return allocatorPtr->reallocate(ptr, oldSize, newSize, alignment);
	}

	inline void polymorphic_universal_allocator_ptr_wrapper::deallocate(void* ptr, size_type size) noexcept
	{
		allocatorPtr->deallocate(ptr, size);
	}

	inline void
	polymorphic_universal_allocator_ptr_wrapper::deallocate(void* ptr, size_type size, size_type alignment) noexcept
	{
		allocatorPtr->deallocate(ptr, size, alignment);
	}

	namespace internal
	{
		template <typename T, typename = void>
		struct memset_zero : false_type
		{
		};

		template <typename T>
		struct memset_zero<T, void_t<decltype(T::memset_zero)>> : bool_constant<T::memset_zero>
		{
		};
	} // namespace internal

	template <typename T>
	template <typename... Args>
	inline T* default_constructor<T>::construct(void* ptr, size_type count, Args&&... args)
		noexcept(noexcept_constructable<Args...>)
	{
		if constexpr (is_trivially_default_constructible_v<T> && sizeof...(Args) == 0)
		{
			if constexpr (internal::memset_zero<T>::value)
			{
				std::memset(ptr, 0, count * sizeof(T));
			}

			return static_cast<T*>(ptr);
		}
		else
		{
			T* first = new (ptr) T(std::forward<Args>(args)...);

			for (size_type i = 1; i < count; i++)
			{
				new (first + i) T(std::forward<Args>(args)...);
			}

			return first;
		}
	}

	template <typename T>
	inline T* default_constructor<T>::move(void* dst, T* src, size_type count) noexcept(noexcept_moveable)
	{
		if constexpr (is_trivially_copy_constructible_v<T>)
		{
			std::memcpy(dst, src, count * sizeof(T));
			return static_cast<T*>(dst);
		}
		else
		{
			T* first = new (dst) T(std::move(src[0]));

			for (size_type i = 1; i < count; i++)
			{
				new (first + i) T(std::move(src[i]));
			}

			return first;
		}
	}

	template <typename T>
	inline void default_constructor<T>::destroy(T* ptr, size_type count) noexcept
	{
		if constexpr (!is_trivially_destructible_v<T>)
		{
			for (size_type i = 0; i < count; i++)
			{
				(ptr + i)->~T();
			}
		}
	}

	template <typename T, universal_allocator_type UniversalAlloc, constructor_type Constructor>
	template <typename... Args>
	inline T* allocator<T, UniversalAlloc, Constructor>::allocate(size_type count, Args&&... args)
		noexcept(Constructor::template noexcept_constructable<Args...>)
	{
		T* mem = static_cast<T*>(UniversalAlloc::allocate(count * sizeof(T)));
		return Constructor::construct(mem, count, std::forward<Args>(args)...);
	}

	template <typename T, universal_allocator_type UniversalAlloc, constructor_type Constructor>
	template <typename... Args>
	inline T* allocator<T, UniversalAlloc, Constructor>::allocate(size_type count, size_type alignment, Args&&... args)
		noexcept(Constructor::template noexcept_constructable<Args...>)
	{
		T* mem = static_cast<T*>(UniversalAlloc::allocate(count * sizeof(T), alignment));
		return Constructor::construct(mem, count, std::forward<Args>(args)...);
	}

	template <typename T, universal_allocator_type UniversalAlloc, constructor_type Constructor>
	template <typename... Args>
	inline T* allocator<T, UniversalAlloc, Constructor>::reallocate(
		T* ptr, size_type oldCount, size_type newCount, Args&&... args
	) noexcept(Constructor::template noexcept_constructable<Args...> && Constructor::noexcept_moveable)
	{
		if constexpr (std::is_trivially_copyable_v<T>)
		{
			T* mem = static_cast<T*>(UniversalAlloc::reallocate(ptr, oldCount * sizeof(T), newCount * sizeof(T)));

			if (newCount > oldCount)
			{
				Constructor::construct(mem + oldCount, oldCount - newCount, std::forward<Args>(args)...);
			}

			return mem;
		}
		else
		{
			T* mem = nullptr;

			if (newCount != 0)
			{
				mem = static_cast<T*>(UniversalAlloc::allocate(newCount * sizeof(T)));
				if (mem)
				{
					Constructor::move(mem, ptr, oldCount);
				}
			}

			UniversalAlloc::deallocate(ptr, oldCount * sizeof(T));

			if (newCount > oldCount)
			{
				Constructor::construct(mem + oldCount, oldCount - newCount, std::forward<Args>(args)...);
			}

			return mem;
		}
	}

	template <typename T, universal_allocator_type UniversalAlloc, constructor_type Constructor>
	template <typename... Args>
	inline T* allocator<T, UniversalAlloc, Constructor>::reallocate(
		T* ptr, size_type oldCount, size_type newCount, size_type alignment, Args&&... args
	) noexcept(Constructor::template noexcept_constructable<Args...> && Constructor::noexcept_moveable)
	{
		if constexpr (std::is_trivially_copyable_v<T>)
		{
			T* mem =
				static_cast<T*>(UniversalAlloc::reallocate(ptr, oldCount * sizeof(T), newCount * sizeof(T), alignment));

			if (newCount > oldCount)
			{
				Constructor::construct(mem + oldCount, oldCount - newCount, std::forward<Args>(args)...);
			}

			return mem;
		}
		else
		{
			T* mem = nullptr;

			if (newCount != 0)
			{
				mem = static_cast<T*>(UniversalAlloc::allocate(newCount * sizeof(T), alignment));
				if (mem)
				{
					Constructor::move(mem, ptr, oldCount);
				}
			}

			UniversalAlloc::deallocate(ptr, oldCount * sizeof(T), alignment);

			if (newCount > oldCount)
			{
				Constructor::construct(mem + oldCount, oldCount - newCount, std::forward<Args>(args)...);
			}

			return mem;
		}
	}

	template <typename T, universal_allocator_type UniversalAlloc, constructor_type Constructor>
	inline UniversalAlloc& allocator<T, UniversalAlloc, Constructor>::as_universal() noexcept
	{
		return *this;
	}

	template <typename T, universal_allocator_type UniversalAlloc, constructor_type Constructor>
	inline const UniversalAlloc& allocator<T, UniversalAlloc, Constructor>::as_universal() const noexcept
	{
		return *this;
	}

	template <typename T, universal_allocator_type UniversalAlloc, constructor_type Constructor>
	inline void allocator<T, UniversalAlloc, Constructor>::deallocate(T* ptr, size_type count) noexcept
	{
		Constructor::destroy(ptr, count);
		UniversalAlloc::deallocate(ptr, count * sizeof(T));
	}

	template <typename T, universal_allocator_type UniversalAlloc, constructor_type Constructor>
	inline void
	allocator<T, UniversalAlloc, Constructor>::deallocate(T* ptr, size_type count, size_type alignment) noexcept
	{
		Constructor::destroy(ptr, count);
		UniversalAlloc::deallocate(ptr, count * sizeof(T), alignment);
	}

	template <typename T, typename... Args>
	T* allocate(pmu_allocator& alloc, size_type count, Args&&... args)
		noexcept(default_constructor<T>::template noexcept_constructable<Args...>)
	{
		using allocator_type = allocator<T, pmu_alloc_ptr_wrapper>;

		allocator_type wrappedAlloc{pmu_alloc_ptr_wrapper(&alloc)};
		return wrappedAlloc.allocate(count, forward<Args>(args)...);
	}

	template <typename T, typename... Args>
	T* allocate(pmu_allocator& alloc, size_type count, size_type alignment, Args&&... args)
		noexcept(default_constructor<T>::template noexcept_constructable<Args...>)
	{
		using allocator_type = allocator<T, pmu_alloc_ptr_wrapper>;

		allocator_type wrappedAlloc{pmu_alloc_ptr_wrapper(&alloc)};
		return wrappedAlloc.allocate(count, alignment, forward<Args>(args)...);
	}

	template <typename T, typename... Args>
	T* reallocate(pmu_allocator& alloc, T* ptr, size_type oldCount, size_type newCount, Args&&... args) noexcept(
		default_constructor<T>::template noexcept_constructable<Args...> && default_constructor<T>::noexcept_moveable
	)
	{
		using allocator_type = allocator<T, pmu_alloc_ptr_wrapper>;

		allocator_type wrappedAlloc{pmu_alloc_ptr_wrapper(&alloc)};
		return wrappedAlloc.reallocate(ptr, oldCount, newCount, forward<Args>(args)...);
	}

	template <typename T, typename... Args>
	T* reallocate(
		pmu_allocator& alloc, T* ptr, size_type oldCount, size_type newCount, size_type alignment, Args&&... args
	)
		noexcept(
			default_constructor<T>::template noexcept_constructable<Args...> &&
			default_constructor<T>::noexcept_moveable
		)
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

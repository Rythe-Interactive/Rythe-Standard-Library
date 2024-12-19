#pragma once
#include "typed_allocator.hpp"

namespace rsl
{

	template <typename T, allocator_type UniversalAlloc, factory_type Factory>
	template <typename... Args>
	inline T* typed_allocator<T, UniversalAlloc, Factory>::allocate(size_type count, Args&&... args)
		noexcept(Factory::template noexcept_constructable<Args...>)
	{
		T* mem = static_cast<T*>(UniversalAlloc::allocate(count * sizeof(T)));
		return Factory::construct(mem, count, forward<Args>(args)...);
	}

	template <typename T, allocator_type UniversalAlloc, factory_type Factory>
	template <typename... Args>
	inline T*
	typed_allocator<T, UniversalAlloc, Factory>::allocate(size_type count, size_type alignment, Args&&... args)
		noexcept(Factory::template noexcept_constructable<Args...>)
	{
		T* mem = static_cast<T*>(UniversalAlloc::allocate(count * sizeof(T), alignment));
		return Factory::construct(mem, count, forward<Args>(args)...);
	}

	template <typename T, allocator_type UniversalAlloc, factory_type Factory>
	template <typename... Args>
	inline T* typed_allocator<T, UniversalAlloc, Factory>::reallocate(
		T* ptr, size_type oldCount, size_type newCount, Args&&... args
	) noexcept(Factory::template noexcept_constructable<Args...> && Factory::noexcept_moveable)
	{
		if constexpr (is_trivially_copyable_v<T>)
		{
			T* mem = static_cast<T*>(UniversalAlloc::reallocate(ptr, oldCount * sizeof(T), newCount * sizeof(T)));

			if (newCount > oldCount)
			{
				Factory::construct(mem + oldCount, oldCount - newCount, forward<Args>(args)...);
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
					Factory::move(mem, ptr, oldCount);
				}
			}

			UniversalAlloc::deallocate(ptr, oldCount * sizeof(T));

			if (newCount > oldCount)
			{
				Factory::construct(mem + oldCount, oldCount - newCount, forward<Args>(args)...);
			}

			return mem;
		}
	}

	template <typename T, allocator_type UniversalAlloc, factory_type Factory>
	template <typename... Args>
	inline T* typed_allocator<T, UniversalAlloc, Factory>::reallocate(
		T* ptr, size_type oldCount, size_type newCount, size_type alignment, Args&&... args
	) noexcept(Factory::template noexcept_constructable<Args...> && Factory::noexcept_moveable)
	{
		if constexpr (is_trivially_copyable_v<T>)
		{
			T* mem =
				static_cast<T*>(UniversalAlloc::reallocate(ptr, oldCount * sizeof(T), newCount * sizeof(T), alignment));

			if (newCount > oldCount)
			{
				Factory::construct(mem + oldCount, oldCount - newCount, forward<Args>(args)...);
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
					Factory::move(mem, ptr, oldCount);
				}
			}

			UniversalAlloc::deallocate(ptr, oldCount * sizeof(T), alignment);

			if (newCount > oldCount)
			{
				Factory::construct(mem + oldCount, oldCount - newCount, forward<Args>(args)...);
			}

			return mem;
		}
	}

	template <typename T, allocator_type UniversalAlloc, factory_type Factory>
	inline UniversalAlloc& typed_allocator<T, UniversalAlloc, Factory>::as_universal() noexcept
	{
		return *this;
	}

	template <typename T, allocator_type UniversalAlloc, factory_type Factory>
	inline const UniversalAlloc& typed_allocator<T, UniversalAlloc, Factory>::as_universal() const noexcept
	{
		return *this;
	}

	template <typename T, allocator_type UniversalAlloc, factory_type Factory>
	inline void typed_allocator<T, UniversalAlloc, Factory>::deallocate(T* ptr, size_type count) noexcept
	{
		Factory::destroy(ptr, count);
		UniversalAlloc::deallocate(ptr, count * sizeof(T));
	}

	template <typename T, allocator_type UniversalAlloc, factory_type Factory>
	inline void
	typed_allocator<T, UniversalAlloc, Factory>::deallocate(T* ptr, size_type count, size_type alignment) noexcept
	{
		Factory::destroy(ptr, count);
		UniversalAlloc::deallocate(ptr, count * sizeof(T), alignment);
	}
} // namespace rsl

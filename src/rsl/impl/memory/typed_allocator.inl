#pragma once
#include "typed_allocator.hpp"

namespace rsl
{
	template <typename T, allocator_type Alloc, factory_type Factory>
	inline typed_allocator<T, Alloc, Factory>::typed_allocator(
		const allocator_storage_type& allocStorage, const factory_storage_type& factoryStorage
	)
		: m_alloc(allocStorage),
		  m_factory(factoryStorage)
	{
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	template <not_same_as<T> Other>
	inline typed_allocator<T, Alloc, Factory>::typed_allocator(const retarget<Other>& other)
		: m_alloc(other.get_allocator_storage()),
		  m_factory(other.get_factory_storage())
	{
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	inline constexpr void typed_allocator<T, Alloc, Factory>::set_allocator(const allocator_storage_type& allocStorage)
		noexcept(is_nothrow_copy_assignable_v<allocator_storage_type>)
	{
		m_alloc = allocStorage;
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	inline constexpr Alloc& typed_allocator<T, Alloc, Factory>::get_allocator() noexcept
	{
		return *m_alloc;
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	inline constexpr const Alloc& typed_allocator<T, Alloc, Factory>::get_allocator() const noexcept
	{
		return *m_alloc;
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	inline constexpr void typed_allocator<T, Alloc, Factory>::set_factory(const factory_storage_type& factoryStorage)
		noexcept(is_nothrow_copy_assignable_v<factory_storage_type>)
	{
		m_factory = factoryStorage;
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	inline constexpr Factory& typed_allocator<T, Alloc, Factory>::get_factory() noexcept
	{
		return *m_factory;
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	inline constexpr const Factory& typed_allocator<T, Alloc, Factory>::get_factory() const noexcept
	{
		return *m_factory;
	}

	template <typename T, allocator_type UniversalAlloc, factory_type Factory>
	template <typename... Args>
	inline constexpr T* typed_allocator<T, UniversalAlloc, Factory>::allocate(size_type count, Args&&... args)
		noexcept(factory_traits<Factory>::template noexcept_constructable<Args...>)
	{
		void* mem = m_alloc->allocate(count * sizeof(T));
		return m_factory->construct(mem, count, forward<Args>(args)...);
	}

	template <typename T, allocator_type UniversalAlloc, factory_type Factory>
	template <typename... Args>
	inline constexpr T*
	typed_allocator<T, UniversalAlloc, Factory>::allocate_aligned(size_type count, size_type alignment, Args&&... args)
		noexcept(factory_traits<Factory>::template noexcept_constructable<Args...>)
	{
		void* mem = m_alloc->allocate(count * sizeof(T), alignment);
		return m_factory->construct(mem, count, forward<Args>(args)...);
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	template <typename... Args>
	inline constexpr T* typed_allocator<T, Alloc, Factory>::construct(T* ptr, size_type count, Args&&... args)
		noexcept(factory_traits<Factory>::template noexcept_constructable<Args...>)
	{
		return m_factory->construct(ptr, count, forward<Args>(args)...);
	}

	template <typename T, allocator_type UniversalAlloc, factory_type Factory>
	template <typename... Args>
	inline constexpr T* typed_allocator<T, UniversalAlloc, Factory>::reallocate(
		T* ptr, size_type oldCount, size_type newCount, Args&&... args
	)
		noexcept(
			factory_traits<Factory>::template noexcept_constructable<Args...> &&
			factory_traits<Factory>::noexcept_moveable
		)
	{
		if constexpr (is_trivially_copyable_v<T>)
		{
			T* mem = static_cast<T*>(m_alloc->reallocate(ptr, oldCount * sizeof(T), newCount * sizeof(T)));

			if (newCount > oldCount)
			{
				m_factory->construct(mem + oldCount, oldCount - newCount, forward<Args>(args)...);
			}

			return mem;
		}
		else
		{
			T* mem = nullptr;

			if (newCount != 0)
			{
				mem = static_cast<T*>(m_alloc->allocate(newCount * sizeof(T)));
				if (mem)
				{
					m_factory->move(mem, ptr, oldCount);
				}
			}

			m_alloc->deallocate(ptr, oldCount * sizeof(T));

			if (newCount > oldCount)
			{
				m_factory->construct(mem + oldCount, oldCount - newCount, forward<Args>(args)...);
			}

			return mem;
		}
	}

	template <typename T, allocator_type UniversalAlloc, factory_type Factory>
	template <typename... Args>
	inline constexpr T* typed_allocator<T, UniversalAlloc, Factory>::reallocate_aligned(
		T* ptr, size_type oldCount, size_type newCount, size_type alignment, Args&&... args
	)
		noexcept(
			factory_traits<Factory>::template noexcept_constructable<Args...> &&
			factory_traits<Factory>::noexcept_moveable
		)
	{
		if constexpr (is_trivially_copyable_v<T>)
		{
			T* mem = static_cast<T*>(m_alloc->reallocate(ptr, oldCount * sizeof(T), newCount * sizeof(T), alignment));

			if (newCount > oldCount)
			{
				m_factory->construct(mem + oldCount, oldCount - newCount, forward<Args>(args)...);
			}

			return mem;
		}
		else
		{
			T* mem = nullptr;

			if (newCount != 0)
			{
				mem = static_cast<T*>(m_alloc->allocate(newCount * sizeof(T), alignment));
				if (mem)
				{
					m_factory->move(mem, ptr, oldCount);
				}
			}

			m_alloc->deallocate(ptr, oldCount * sizeof(T), alignment);

			if (newCount > oldCount)
			{
				m_factory->construct(mem + oldCount, oldCount - newCount, forward<Args>(args)...);
			}

			return mem;
		}
	}

	template <typename T, allocator_type UniversalAlloc, factory_type Factory>
	inline constexpr void typed_allocator<T, UniversalAlloc, Factory>::deallocate(T* ptr, size_type count) noexcept
	{
		m_factory->destroy(ptr, count);
		m_alloc->deallocate(ptr, count * sizeof(T));
	}

	template <typename T, allocator_type UniversalAlloc, factory_type Factory>
	inline constexpr void typed_allocator<T, UniversalAlloc, Factory>::deallocate_aligned(
		T* ptr, size_type count, size_type alignment
	) noexcept
	{
		m_factory->destroy(ptr, count);
		m_alloc->deallocate(ptr, count * sizeof(T), alignment);
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	inline constexpr allocator_storage<Alloc>& typed_allocator<T, Alloc, Factory>::get_allocator_storage() noexcept
	{
		return m_alloc;
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	inline constexpr const allocator_storage<Alloc>&
	typed_allocator<T, Alloc, Factory>::get_allocator_storage() const noexcept
	{
		return m_alloc;
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	inline constexpr factory_storage<Factory>& typed_allocator<T, Alloc, Factory>::get_factory_storage() noexcept
	{
		return m_factory;
	}

	template <typename T, allocator_type Alloc, factory_type Factory>
	inline constexpr const factory_storage<Factory>&
	typed_allocator<T, Alloc, Factory>::get_factory_storage() const noexcept
	{
		return m_factory;
	}

	template <allocator_type Alloc, untyped_factory_type Factory>
	inline constexpr void
	type_erased_allocator<Alloc, Factory>::set_allocator(const allocator_storage_type& allocStorage)
		noexcept(is_nothrow_copy_assignable_v<allocator_storage_type>)
	{
		m_alloc = allocStorage;
	}

	template <allocator_type Alloc, untyped_factory_type Factory>
	inline constexpr Alloc& type_erased_allocator<Alloc, Factory>::get_allocator() noexcept
	{
		return *m_alloc;
	}

	template <allocator_type Alloc, untyped_factory_type Factory>
	inline constexpr const Alloc& type_erased_allocator<Alloc, Factory>::get_allocator() const noexcept
	{
		return *m_alloc;
	}

	template <allocator_type Alloc, untyped_factory_type Factory>
	inline constexpr void type_erased_allocator<Alloc, Factory>::set_factory(const factory_storage_type& factoryStorage)
		noexcept(is_nothrow_copy_assignable_v<factory_storage_type>)
	{
		m_factory = factoryStorage;
	}

	template <allocator_type Alloc, untyped_factory_type Factory>
	inline constexpr Factory& type_erased_allocator<Alloc, Factory>::get_factory() noexcept
	{
		return *m_factory;
	}

	template <allocator_type Alloc, untyped_factory_type Factory>
	inline constexpr const Factory& type_erased_allocator<Alloc, Factory>::get_factory() const noexcept
	{
		return *m_factory;
	}

	template <allocator_type Alloc, untyped_factory_type Factory>
	inline constexpr void* type_erased_allocator<Alloc, Factory>::allocate(size_type count)
		noexcept(factory_traits<Factory>::template noexcept_constructable<>)
	{
		void* mem = m_alloc->allocate(count * m_factory->typeSize());
		return m_factory->construct(mem, count);
	}

	template <allocator_type Alloc, untyped_factory_type Factory>
	inline constexpr void* type_erased_allocator<Alloc, Factory>::allocate_aligned(size_type count, size_type alignment)
		noexcept(factory_traits<Factory>::template noexcept_constructable<>)
	{
		void* mem = m_alloc->allocate(count * m_factory->typeSize(), alignment);
		return m_factory->construct(mem, count);
	}

	template <allocator_type Alloc, untyped_factory_type Factory>
	inline constexpr void* type_erased_allocator<Alloc, Factory>::construct(void* ptr, size_type count)
		noexcept(factory_traits<Factory>::template noexcept_constructable<>)
	{
		return m_factory->construct(ptr, count);
	}

	template <allocator_type Alloc, untyped_factory_type Factory>
	inline constexpr void* type_erased_allocator<Alloc, Factory>::reallocate(
		void* ptr, size_type oldCount, size_type newCount
	) noexcept(factory_traits<Factory>::template noexcept_constructable<> && factory_traits<Factory>::noexcept_moveable)
	{
		size_type typeSize = m_factory->typeSize();
		bool canTriviallyCopy = can_trivially_copy(*m_factory);

		if (canTriviallyCopy)
		{
			void* mem = m_alloc->reallocate(ptr, oldCount * typeSize, newCount * typeSize);

			if (newCount > oldCount)
			{
				m_factory->construct(advance(mem, oldCount * typeSize), oldCount - newCount);
			}

			return mem;
		}
		else
		{
			void* mem = nullptr;

			if (newCount != 0)
			{
				mem = m_alloc->allocate(newCount * typeSize);
				if (mem)
				{
					m_factory->move(mem, ptr, oldCount);
				}
			}

			m_alloc->deallocate(ptr, oldCount * typeSize);

			if (newCount > oldCount)
			{
				m_factory->construct(advance(mem, oldCount * typeSize), oldCount - newCount);
			}

			return mem;
		}
	}

	template <allocator_type Alloc, untyped_factory_type Factory>
	inline constexpr void* type_erased_allocator<Alloc, Factory>::reallocate_aligned(
		void* ptr, size_type oldCount, size_type newCount, size_type alignment
	) noexcept(factory_traits<Factory>::template noexcept_constructable<> && factory_traits<Factory>::noexcept_moveable)
	{
		size_type typeSize = m_factory->typeSize();
		bool canTriviallyCopy = can_trivially_copy(*m_factory);

		if (canTriviallyCopy)
		{
			void* mem = m_alloc->reallocate(ptr, oldCount * typeSize, newCount * typeSize, alignment);

			if (newCount > oldCount)
			{
				m_factory->construct(advance(mem, oldCount * typeSize), oldCount - newCount);
			}

			return mem;
		}
		else
		{
			void* mem = nullptr;

			if (newCount != 0)
			{
				mem = m_alloc->allocate(newCount * typeSize, alignment);
				if (mem)
				{
					m_factory->move(mem, ptr, oldCount);
				}
			}

			m_alloc->deallocate(ptr, oldCount * typeSize, alignment);

			if (newCount > oldCount)
			{
				m_factory->construct(advance(mem, oldCount * typeSize), oldCount - newCount);
			}

			return mem;
		}
	}

	template <allocator_type Alloc, untyped_factory_type Factory>
	inline constexpr void type_erased_allocator<Alloc, Factory>::deallocate(void* ptr, size_type count) noexcept
	{
		m_factory->destroy(ptr, count);
		m_alloc->deallocate(ptr, count * m_factory->typeSize());
	}

	template <allocator_type Alloc, untyped_factory_type Factory>
	inline constexpr void
	type_erased_allocator<Alloc, Factory>::deallocate_aligned(void* ptr, size_type count, size_type alignment) noexcept
	{
		m_factory->destroy(ptr, count);
		m_alloc->deallocate(ptr, count * m_factory->typeSize(), alignment);
	}

	template <allocator_type Alloc, untyped_factory_type Factory>
	inline constexpr allocator_storage<Alloc>& type_erased_allocator<Alloc, Factory>::get_allocator_storage() noexcept
	{
		return m_alloc;
	}

	template <allocator_type Alloc, untyped_factory_type Factory>
	inline constexpr const allocator_storage<Alloc>&
	type_erased_allocator<Alloc, Factory>::get_allocator_storage() const noexcept
	{
		return m_alloc;
	}

	template <allocator_type Alloc, untyped_factory_type Factory>
	inline constexpr factory_storage<Factory>& type_erased_allocator<Alloc, Factory>::get_factory_storage() noexcept
	{
		return m_factory;
	}

	template <allocator_type Alloc, untyped_factory_type Factory>
	inline constexpr const factory_storage<Factory>&
	type_erased_allocator<Alloc, Factory>::get_factory_storage() const noexcept
	{
		return m_factory;
	}
} // namespace rsl

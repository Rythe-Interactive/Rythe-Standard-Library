#pragma once

namespace rsl
{
	template <typename TypedAllocator>
    [[nodiscard]] [[rythe_always_inline]] constexpr size_type typed_allocator_impl<TypedAllocator>::type_size() const noexcept
    {
        return self().get_factory().type_size();
    }

	template <typename TypedAllocator>
    constexpr TypedAllocator::value_type* typed_allocator_impl<TypedAllocator>::allocate(const size_type count) noexcept
    {
	    allocator_t& allocator = self().get_allocator();
        return bit_cast<value_type*>(allocator.allocate(count * type_size()));
    }

	template <typename TypedAllocator>
    constexpr TypedAllocator::value_type* typed_allocator_impl<TypedAllocator>::allocate(const size_type count, size_type alignment) noexcept
    {
	    allocator_t& allocator = self().get_allocator();
        return bit_cast<value_type*>(allocator.allocate(count * type_size(), alignment));
    }

	template <typename TypedAllocator>
    constexpr TypedAllocator::value_type* typed_allocator_impl<TypedAllocator>::reallocate(
            value_type* ptr,
            const size_type oldCount,
            const size_type newCount
            )
        noexcept(factory_traits<factory_t>::noexcept_moveable)
    {
        if constexpr (is_trivially_copyable_v<value_type>)
        {
	        allocator_t& allocator = self().get_allocator();
            value_type* mem = bit_cast<value_type*>(allocator.reallocate(ptr, oldCount * type_size(), newCount * type_size()));

            return mem;
        }
        else
        {
            value_type* mem = nullptr;

            if (newCount != 0)
            {
                mem = allocate(newCount);
                if (mem)
                {
                    move(mem, ptr, oldCount);
                }
            }

            deallocate(ptr, oldCount);

            return mem;
        }
    }

	template <typename TypedAllocator>
    constexpr TypedAllocator::value_type*
    typed_allocator_impl<TypedAllocator>::reallocate(
            value_type* ptr,
            const size_type oldCount,
            const size_type newCount,
            size_type alignment
            )
        noexcept(factory_traits<factory_t>::noexcept_moveable)
    {
        if constexpr (is_trivially_copyable_v<value_type>)
        {
	        allocator_t& allocator = self().get_allocator();
            value_type* mem = static_cast<value_type*>(allocator.reallocate(
                    ptr,
                    oldCount * type_size(),
                    newCount * type_size(),
                    alignment
                    ));

            return mem;
        }
        else
        {
            value_type* mem = nullptr;

            if (newCount != 0)
            {
                mem = allocate(newCount, alignment);
                if (mem)
                {
                    move(mem, ptr, oldCount);
                }
            }

            deallocate(ptr, oldCount, alignment);

            return mem;
        }
    }

	template <typename TypedAllocator>
    constexpr void typed_allocator_impl<TypedAllocator>::deallocate(value_type* ptr, const size_type count) noexcept
    {
        self().get_allocator().deallocate(ptr, count * type_size());
    }

	template <typename TypedAllocator>
    constexpr void
        typed_allocator_impl<TypedAllocator>::deallocate(value_type* ptr, const size_type count, size_type alignment) noexcept
    {
        self().get_allocator().deallocate(ptr, count * type_size(), alignment);
    }

	template <typename TypedAllocator>
    template <typename... Args>
    constexpr TypedAllocator::value_type* typed_allocator_impl<TypedAllocator>::construct(
            value_type* ptr,
            size_type count,
            Args&&... args
            )
        noexcept(factory_traits<factory_t>::template noexcept_constructable<Args...>)
    {
        return self().get_factory().construct(ptr, count, rsl::forward<Args>(args)...);
    }

	template <typename TypedAllocator>
    constexpr TypedAllocator::value_type* typed_allocator_impl<TypedAllocator>::copy(
            value_type* dst,
            const value_type* src,
            size_type count
            )
        noexcept(factory_traits<factory_t>::noexcept_moveable)
    {
        return self().get_factory().copy(dst, src, count);
    }

	template <typename TypedAllocator>
    constexpr TypedAllocator::value_type* typed_allocator_impl<TypedAllocator>::move(
            value_type* dst,
            value_type* src,
            size_type count
            )
        noexcept(factory_traits<factory_t>::noexcept_moveable)
    {
        return self().get_factory().move(dst, src, count);
    }

	template <typename TypedAllocator>
    constexpr void typed_allocator_impl<TypedAllocator>::destroy(value_type* ptr, size_type count) noexcept
    {
        self().get_factory().destroy(ptr, count);
    }

    template <typename TypedAllocator>
    template <typename... Args>
    constexpr TypedAllocator::value_type*
    typed_allocator_impl<TypedAllocator>::allocate_and_construct(
            size_type count,
            Args&&... args
            )
        noexcept(factory_traits<factory_t>::template noexcept_constructable<Args...>)
    {
        value_type* mem = allocate(count);
        return construct(mem, count, forward<Args>(args)...);
    }

    template <typename TypedAllocator>
    template <typename... Args>
    constexpr TypedAllocator::value_type* typed_allocator_impl<TypedAllocator>::allocate_aligned_and_construct(
            const size_type count,
            const size_type alignment,
            Args&&... args
            ) noexcept(factory_traits<factory_t>::template noexcept_constructable<Args...>)
    {
        value_type* mem = allocate(count, alignment);
        return construct(mem, count, forward<Args>(args)...);
    }

    template <typename TypedAllocator>
    template <typename... Args>
    constexpr TypedAllocator::value_type* typed_allocator_impl<TypedAllocator>::reallocate_and_construct(
            value_type* ptr,
            size_type oldCount,
            const size_type newCount,
            Args&&... args
            )
        noexcept(
            factory_traits<factory_t>::template noexcept_constructable<Args...> &&
            factory_traits<factory_t>::noexcept_moveable
        )
    {
        if constexpr (is_trivially_copyable_v<value_type>)
        {
	        allocator_t& allocator = self().get_allocator();
            value_type* mem = static_cast<value_type*>(allocator.reallocate(ptr, oldCount * type_size(), newCount * type_size()));

            if (newCount > oldCount)
            {
                construct(mem + oldCount, oldCount - newCount, forward<Args>(args)...);
            }

            return mem;
        }
        else
        {
            value_type* mem = nullptr;

            if (newCount != 0)
            {
                mem = allocate(newCount);
                if (mem)
                {
                    move(mem, ptr, oldCount);
                }
            }

            deallocate(ptr, oldCount);

            if (newCount > oldCount)
            {
                construct(mem + oldCount, oldCount - newCount, forward<Args>(args)...);
            }

            return mem;
        }
    }

    template <typename TypedAllocator>
    template <typename... Args>
    constexpr TypedAllocator::value_type* typed_allocator_impl<TypedAllocator>::reallocate_aligned_and_construct(
            value_type* ptr,
            size_type oldCount,
            const size_type newCount,
            size_type alignment,
            Args&&... args
            )
        noexcept(
            factory_traits<factory_t>::template noexcept_constructable<Args...> &&
            factory_traits<factory_t>::noexcept_moveable
        )
    {
        if constexpr (is_trivially_copyable_v<value_type>)
        {
	        allocator_t& allocator = self().get_allocator();
            value_type* mem = static_cast<value_type*>(allocator.reallocate(
                    ptr,
                    oldCount * type_size(),
                    newCount * type_size(),
                    alignment
                    ));

            if (newCount > oldCount)
            {
                construct(mem + oldCount, oldCount - newCount, forward<Args>(args)...);
            }

            return mem;
        }
        else
        {
            value_type* mem = nullptr;

            if (newCount != 0)
            {
                mem = allocate(newCount, alignment);
                if (mem)
                {
                    move(mem, ptr, oldCount);
                }
            }

            deallocate(ptr, oldCount, alignment);

            if (newCount > oldCount)
            {
                construct(mem + oldCount, oldCount - newCount, forward<Args>(args)...);
            }

            return mem;
        }
    }

    template <typename TypedAllocator>
    constexpr void
        typed_allocator_impl<TypedAllocator>::destroy_and_deallocate(value_type* ptr, const size_type count) noexcept
    {
        destroy(ptr, count);
        deallocate(ptr, count);
    }

    template <typename TypedAllocator>
    constexpr void typed_allocator_impl<TypedAllocator>::destroy_and_deallocate_aligned(
            value_type* ptr,
            const size_type count,
            const size_type alignment
            ) noexcept
    {
        destroy(ptr, count);
        deallocate(ptr, count, alignment);
    }

    template <typename TypedAllocator>
    constexpr TypedAllocator& typed_allocator_impl<TypedAllocator>::self() noexcept
	{
	    return static_cast<TypedAllocator&>(*this);
	}

    template <typename TypedAllocator>
    constexpr const TypedAllocator& typed_allocator_impl<TypedAllocator>::self() const noexcept
	{
	    return static_cast<const TypedAllocator&>(*this);
	}

    template <allocator_type Alloc, untyped_factory_type Factory>
    template <typename T>
    type_erased_allocator<
        Alloc,
        Factory>::type_erased_allocator(
            const allocator_storage_type& allocStorage,
            construct_type_signal_type<T>
            )
        noexcept(is_nothrow_copy_constructible_v<allocator_storage_type> && is_nothrow_constructible_v<
            factory_t, construct_type_signal_type<T>> && is_nothrow_constructible_v<factory_storage_type, factory_t&&>)
        : m_factory(factory_t(construct_type_signal<T>)),
          m_alloc(allocStorage) {}

    template <allocator_type Alloc, untyped_factory_type Factory>
    type_erased_allocator<Alloc, Factory>::type_erased_allocator(
            const factory_storage_type& factoryStorage
            ) noexcept(is_nothrow_constructible_v<allocator_storage_type> && is_nothrow_copy_constructible_v<factory_storage_type>)
        : m_factory(factoryStorage),
          m_alloc() {}

    template <allocator_type Alloc, untyped_factory_type Factory>
    type_erased_allocator<Alloc, Factory>::type_erased_allocator(
            const allocator_storage_type& allocStorage,
            const factory_storage_type& factoryStorage
            ) noexcept(is_nothrow_copy_constructible_v<allocator_storage_type> && is_nothrow_copy_constructible_v<factory_storage_type>
    )
        : m_factory(factoryStorage),
          m_alloc(allocStorage) {}

    template <allocator_type Alloc, untyped_factory_type Factory>
    constexpr void
        type_erased_allocator<Alloc, Factory>::set_allocator(
                const allocator_storage_type& allocStorage
                )
            noexcept(is_nothrow_copy_assignable_v<allocator_storage_type>)
    {
        m_alloc = allocStorage;
    }

    template <allocator_type Alloc, untyped_factory_type Factory>
    constexpr Alloc& type_erased_allocator<Alloc, Factory>::get_allocator() noexcept
    {
        return *m_alloc;
    }

    template <allocator_type Alloc, untyped_factory_type Factory>
    constexpr const Alloc& type_erased_allocator<Alloc, Factory>::get_allocator() const noexcept
    {
        return *m_alloc;
    }

    template <allocator_type Alloc, untyped_factory_type Factory>
    constexpr void type_erased_allocator<Alloc, Factory>::set_factory(
            const factory_storage_type& factoryStorage
            )
        noexcept(is_nothrow_copy_assignable_v<factory_storage_type>)
    {
        m_factory = factoryStorage;
    }

    template <allocator_type Alloc, untyped_factory_type Factory>
    constexpr Factory& type_erased_allocator<Alloc, Factory>::get_factory() noexcept
    {
        return *m_factory;
    }

    template <allocator_type Alloc, untyped_factory_type Factory>
    constexpr const Factory& type_erased_allocator<Alloc, Factory>::get_factory() const noexcept
    {
        return *m_factory;
    }

    template <allocator_type Alloc, untyped_factory_type Factory>
    size_type type_erased_allocator<Alloc, Factory>::type_size() const noexcept
    {
        return m_factory->type_size();
    }

    template <allocator_type Alloc, untyped_factory_type Factory>
    bool type_erased_allocator<Alloc, Factory>::trivial_copy() const noexcept
    {
        return can_trivially_copy(*m_factory);
    }

    template <allocator_type Alloc, untyped_factory_type Factory>
    id_type type_erased_allocator<Alloc, Factory>::type_id() const noexcept
    {
        return m_factory->type_id();
    }

    template <allocator_type Alloc, untyped_factory_type Factory>
    constexpr void* type_erased_allocator<Alloc, Factory>::allocate(const size_type count) noexcept
    {
        return m_alloc->allocate(count * type_size());
    }

    template <allocator_type Alloc, untyped_factory_type Factory>
    constexpr void*
    type_erased_allocator<Alloc, Factory>::allocate(const size_type count, size_type alignment) noexcept
    {
        return m_alloc->allocate(count * type_size(), alignment);
    }

    template <allocator_type Alloc, untyped_factory_type Factory>
    constexpr void*
    type_erased_allocator<Alloc, Factory>::reallocate(
            void* ptr,
            size_type oldCount,
            const size_type newCount
            )
        noexcept(factory_traits<Factory>::noexcept_moveable)
    {
        const size_type typeSize = type_size();
        const bool canTriviallyCopy = can_trivially_copy(*m_factory);

        if (canTriviallyCopy)
        {
            return m_alloc->reallocate(ptr, oldCount * typeSize, newCount * typeSize);
        }

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

        return mem;
    }

    template <allocator_type Alloc, untyped_factory_type Factory>
    constexpr void* type_erased_allocator<Alloc, Factory>::reallocate(
            void* ptr,
            size_type oldCount,
            const size_type newCount,
            size_type alignment
            ) noexcept(factory_traits<Factory>::noexcept_moveable)
    {
        size_type typeSize = type_size();
        bool canTriviallyCopy = can_trivially_copy(*m_factory);

        if (canTriviallyCopy)
        {
            return m_alloc->reallocate(ptr, oldCount * typeSize, newCount * typeSize, alignment);
        }
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

        return mem;
    }

    template <allocator_type Alloc, untyped_factory_type Factory>
    constexpr void type_erased_allocator<Alloc, Factory>::deallocate(void* ptr, const size_type count) noexcept
    {
        m_alloc->deallocate(ptr, count * type_size());
    }

    template <allocator_type Alloc, untyped_factory_type Factory>
    constexpr void
        type_erased_allocator<Alloc, Factory>::deallocate(void* ptr, const size_type count, size_type alignment) noexcept
    {
        m_alloc->deallocate(ptr, count * type_size(), alignment);
    }

    template <allocator_type Alloc, untyped_factory_type Factory>
    constexpr void* type_erased_allocator<Alloc, Factory>::construct(
            void* ptr,
            size_type count
            )
        noexcept(factory_traits<Factory>::template noexcept_constructable<>)
    {
        return m_factory->construct(ptr, count);
    }

    template <allocator_type Alloc, untyped_factory_type Factory>
    constexpr void* type_erased_allocator<Alloc, Factory>::move(
            void* dst,
            void* src,
            size_type count
            )
        noexcept(factory_traits<Factory>::noexcept_moveable)
    {
        return m_factory->move(dst, src, count);
    }

    template <allocator_type Alloc, untyped_factory_type Factory>
    constexpr void type_erased_allocator<Alloc, Factory>::destroy(void* ptr, size_type count) noexcept
    {
        m_factory->destroy(ptr, count);
    }

    template <allocator_type Alloc, untyped_factory_type Factory>
    constexpr void* type_erased_allocator<Alloc, Factory>::allocate_and_construct(
            size_type count
            )
        noexcept(factory_traits<Factory>::template noexcept_constructable<>)
    {
        void* mem = m_alloc->allocate(count * type_size());
        return m_factory->construct(mem, count);
    }

    template <allocator_type Alloc, untyped_factory_type Factory>
    constexpr void*
    type_erased_allocator<Alloc, Factory>::allocate_aligned_and_construct(
            size_type count,
            size_type alignment
            )
        noexcept(factory_traits<Factory>::template noexcept_constructable<>)
    {
        void* mem = m_alloc->allocate(count * type_size(), alignment);
        return m_factory->construct(mem, count);
    }

    template <allocator_type Alloc, untyped_factory_type Factory>
    constexpr void* type_erased_allocator<Alloc, Factory>::reallocate_and_construct(
            void* ptr,
            size_type oldCount,
            const size_type newCount
            ) noexcept(factory_traits<Factory>::template noexcept_constructable<> && factory_traits<Factory>::noexcept_moveable)
    {
        size_type typeSize = type_size();
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

    template <allocator_type Alloc, untyped_factory_type Factory>
    constexpr void* type_erased_allocator<Alloc, Factory>::reallocate_aligned_and_construct(
            void* ptr,
            size_type oldCount,
            const size_type newCount,
            size_type alignment
            ) noexcept(factory_traits<Factory>::template noexcept_constructable<> && factory_traits<Factory>::noexcept_moveable)
    {
        size_type typeSize = type_size();
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

    template <allocator_type Alloc, untyped_factory_type Factory>
    constexpr void
        type_erased_allocator<Alloc, Factory>::destroy_and_deallocate(void* ptr, size_type count) noexcept
    {
        m_factory->destroy(ptr, count);
        m_alloc->deallocate(ptr, count * type_size());
    }

    template <allocator_type Alloc, untyped_factory_type Factory>
    constexpr void type_erased_allocator<Alloc, Factory>::destroy_and_deallocate_aligned(
            void* ptr,
            size_type count,
            size_type alignment
            ) noexcept
    {
        m_factory->destroy(ptr, count);
        m_alloc->deallocate(ptr, count * type_size(), alignment);
    }

    template <allocator_type Alloc, untyped_factory_type Factory>
    constexpr allocator_storage<Alloc>& type_erased_allocator<Alloc, Factory>::get_allocator_storage() noexcept
    {
        return m_alloc;
    }

    template <allocator_type Alloc, untyped_factory_type Factory>
    constexpr const allocator_storage<Alloc>&
    type_erased_allocator<Alloc, Factory>::get_allocator_storage() const noexcept
    {
        return m_alloc;
    }

    template <allocator_type Alloc, untyped_factory_type Factory>
    constexpr factory_storage<Factory>& type_erased_allocator<Alloc, Factory>::get_factory_storage() noexcept
    {
        return m_factory;
    }

    template <allocator_type Alloc, untyped_factory_type Factory>
    constexpr const factory_storage<Factory>&
    type_erased_allocator<Alloc, Factory>::get_factory_storage() const noexcept
    {
        return m_factory;
    }
} // namespace rsl

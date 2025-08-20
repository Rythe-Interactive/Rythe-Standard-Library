#pragma once
#include "memory_resource_base.hpp"

namespace rsl
{
    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::dynamic_memory_resource_base(
            const allocator_storage_type& allocStorage
            )
        noexcept(is_nothrow_constructible_v<typed_alloc_type, const allocator_storage_type&>)
        : m_alloc(allocStorage, construct_type_signal<UtilType>) {}

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::dynamic_memory_resource_base(
            const factory_storage_type& factoryStorage
            )
        noexcept(is_nothrow_constructible_v<typed_alloc_type, const factory_storage_type&>)
        : m_alloc(factoryStorage) {}

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::dynamic_memory_resource_base(
            const allocator_storage_type& allocStorage,
            const factory_storage_type& factoryStorage
            )
        noexcept(is_nothrow_constructible_v<typed_alloc_type, const allocator_storage_type&, const factory_storage_type&>)
        : m_alloc(allocStorage, factoryStorage) {}

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void
        dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::set_allocator(
                const allocator_storage_type& allocStorage
                )
            noexcept(is_nothrow_copy_assignable_v<allocator_storage_type>)
    {
        m_alloc.set_allocator(allocStorage);
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr Alloc& dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::get_allocator() noexcept
    {
        return m_alloc.get_allocator();
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr const Alloc& dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::get_allocator() const noexcept
    {
        return m_alloc.get_allocator();
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void
        dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::set_factory(
                const factory_storage_type& factoryStorage
                )
            noexcept(is_nothrow_copy_assignable_v<factory_storage_type>)
    {
        m_alloc.set_factory(factoryStorage);
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr Factory& dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::get_factory() noexcept
    {
        return m_alloc.get_factory();
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr const Factory& dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::get_factory() const noexcept
    {
        return m_alloc.get_factory();
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::allocator_storage_type&
    dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::get_allocator_storage() noexcept
    {
        return m_alloc.get_allocator_storage();
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr const dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::allocator_storage_type&
    dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::get_allocator_storage() const noexcept
    {
        return m_alloc.get_allocator_storage();
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::factory_storage_type&
    dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::get_factory_storage() noexcept
    {
        return m_alloc.get_factory_storage();
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr const dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::factory_storage_type&
    dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::get_factory_storage() const noexcept
    {
        return m_alloc.get_factory_storage();
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::allocate(size_type count) noexcept
    {
        m_ptr = m_alloc.allocate(count);
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void
        dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::allocate(size_type count, size_type alignment) noexcept
    {
        m_ptr = m_alloc.allocate(count, alignment);
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::reallocate(
            size_type oldCount,
            size_type newCount
            )
        noexcept(factory_traits<Factory>::noexcept_moveable)
    {
        m_ptr = m_alloc.reallocate(get_ptr(), oldCount, newCount);
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void
        dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::reallocate(
                size_type oldCount,
                size_type newCount,
                size_type alignment
                )
            noexcept(factory_traits<Factory>::noexcept_moveable)
    {
        m_ptr = m_alloc.reallocate(get_ptr(), oldCount, newCount, alignment);
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::deallocate(size_type count) noexcept
    {
        m_alloc.deallocate(get_ptr(), count);
        m_ptr = nullptr;
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void
        dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::deallocate(size_type count, size_type alignment) noexcept
    {
        m_alloc.deallocate(get_ptr(), count, alignment);
        m_ptr = nullptr;
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::construct(
            size_type count,
            size_type offset
            )
        noexcept(factory_traits<Factory>::template noexcept_constructable<>)
    {
        m_alloc.construct(advance(get_ptr(), offset * m_alloc.type_size()), count);
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::copy(
            const size_type count,
            const size_type offset,
            const void* src
            ) noexcept(factory_traits<Factory>::noexcept_copyable)
    {
        m_alloc.copy(advance(get_ptr(), offset * m_alloc.type_size()), static_cast<const UtilType*>(src), count);
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::move(
            const size_type count,
            const size_type offset,
            void* src
            ) noexcept(factory_traits<Factory>::noexcept_moveable)
    {
        m_alloc.move(advance(get_ptr(), offset * m_alloc.type_size()), src, count);
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void
        dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::destroy(const size_type count, const size_type offset) noexcept
    {
        m_alloc.destroy(advance(get_ptr(), offset * m_alloc.type_size()), count);
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::allocate_and_construct(
            const size_type count
            )
        noexcept(factory_traits<Factory>::template noexcept_constructable<>)
    {
        m_ptr = m_alloc.allocate_and_construct(count);
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    template <typename... Args>
    constexpr void
        dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::allocate_aligned_and_construct(
                const size_type count,
                const size_type alignment
                )
            noexcept(factory_traits<Factory>::template noexcept_constructable<>)
    {
        m_ptr = m_alloc.allocate_aligned_and_construct(count, alignment);
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    template <typename... Args>
    constexpr void
        dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::reallocate_and_construct(
                const size_type oldCount,
                const size_type newCount
                )
            noexcept(factory_traits<Factory>::template noexcept_constructable<> && factory_traits<Factory>::noexcept_moveable)
    {
        m_ptr = m_alloc.reallocate_and_construct(get_ptr(), oldCount, newCount);
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    template <typename... Args>
    constexpr void dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::reallocate_aligned_and_construct(
            const size_type oldCount,
            const size_type newCount,
            const size_type alignment
            )
        noexcept(factory_traits<Factory>::template noexcept_constructable<> && factory_traits<Factory>::noexcept_moveable)
    {
        m_ptr = m_alloc.reallocate_aligned_and_construct(get_ptr(), oldCount, newCount, alignment);
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::destroy_and_deallocate(const size_type count) noexcept
    {
        m_alloc.destroy_and_deallocate(get_ptr(), count);
        m_ptr = nullptr;
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::destroy_and_deallocate_aligned(
            const size_type count,
            const size_type alignment
            ) noexcept
    {
        m_alloc.destroy_and_deallocate_aligned(get_ptr(), count, alignment);
        m_ptr = nullptr;
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr UtilType* dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::get_ptr() noexcept
    {
        return bit_cast<UtilType*>(m_ptr);
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr const UtilType* dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::get_ptr() const noexcept
    {
        return bit_cast<const UtilType*>(m_ptr);
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::set_ptr(UtilType* const& ptr) noexcept
    {
        m_ptr = ptr;
    }

    template <typename T, allocator_type Alloc, factory_type Factory>
    template <typename... Args>
    constexpr void
        typed_dynamic_memory_resource<T, Alloc, Factory>::construct(
                size_type count,
                size_type offset,
                Args&&... args
                )
            noexcept(factory_traits<Factory>::template noexcept_constructable<Args...>)
            requires (sizeof...(Args) != 0)
    {
        base_type::m_alloc.construct(base_type::get_ptr() + offset, count, rsl::forward<Args>(args)...);
    }

    template <typename T, allocator_type Alloc, factory_type Factory>
    template <typename... Args>
    constexpr void typed_dynamic_memory_resource<T, Alloc, Factory>::allocate_and_construct(
            size_type count,
            Args&&... args
            )
        noexcept(factory_traits<Factory>::template noexcept_constructable<Args...>)
        requires (sizeof...(Args) != 0)
    {
        base_type::m_ptr = base_type::m_alloc.allocate_and_construct(count, forward<Args>(args)...);
    }

    template <typename T, allocator_type Alloc, factory_type Factory>
    template <typename... Args>
    constexpr void typed_dynamic_memory_resource<T, Alloc, Factory>::allocate_aligned_and_construct(
            size_type count,
            size_type alignment,
            Args&&... args
            )
        noexcept(factory_traits<Factory>::template noexcept_constructable<Args...>)
        requires (sizeof...(Args) != 0)
    {
        base_type::m_ptr = base_type::m_alloc.allocate_aligned_and_construct(count, alignment, forward<Args>(args)...);
    }

    template <typename T, allocator_type Alloc, factory_type Factory>
    template <typename... Args>
    constexpr void
        typed_dynamic_memory_resource<T, Alloc, Factory>::reallocate_and_construct(
                size_type oldCount,
                size_type newCount,
                Args&&... args
                )
            noexcept(factory_traits<Factory>::template noexcept_constructable<Args...> && factory_traits<Factory>::noexcept_moveable)
            requires (sizeof...(Args) != 0)
    {
        base_type::m_ptr = base_type::m_alloc.reallocate_and_construct(
                base_type::get_ptr(),
                oldCount,
                newCount,
                forward<Args>(args)...
                );
    }

    template <typename T, allocator_type Alloc, factory_type Factory>
    template <typename... Args>
    constexpr void typed_dynamic_memory_resource<T, Alloc, Factory>::reallocate_aligned_and_construct(
            size_type oldCount,
            size_type newCount,
            size_type alignment,
            Args&&... args
            )
        noexcept(factory_traits<Factory>::template noexcept_constructable<Args...> && factory_traits<Factory>::noexcept_moveable)
        requires (sizeof...(Args) != 0)
    {
        base_type::m_ptr = base_type::m_alloc.reallocate_aligned_and_construct(
                base_type::get_ptr(),
                oldCount,
                newCount,
                alignment,
                forward<Args>(args)...
                );
    }

    template <size_type BufferSize, factory_type Factory, typename UtilType, bool Untyped>
    constexpr static_memory_resource_base<BufferSize, Factory, UtilType, Untyped>::static_memory_resource_base(
            const factory_storage_type& factoryStorage
            ) noexcept(is_nothrow_copy_constructible_v<factory_storage_type>)
        : m_factory(factoryStorage) {}

    template <size_type BufferSize, factory_type Factory, typename UtilType, bool Untyped>
    constexpr static_memory_resource_base<BufferSize, Factory, UtilType, Untyped>::static_memory_resource_base(
            factory_storage_type&& factoryStorage
            ) noexcept(is_nothrow_move_constructible_v<factory_storage_type>)
        : m_factory(rsl::move(factoryStorage)) {}

    template <size_type BufferSize, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void static_memory_resource_base<BufferSize, Factory, UtilType, Untyped>::set_factory(
            const factory_storage_type& factoryStorage
            ) noexcept(is_nothrow_copy_assignable_v<factory_storage_type>)
    {
        m_factory = factoryStorage;
    }

    template <size_type BufferSize, factory_type Factory, typename UtilType, bool Untyped>
    constexpr static_memory_resource_base<BufferSize, Factory, UtilType, Untyped>::factory_t& static_memory_resource_base<
        BufferSize, Factory, UtilType, Untyped>::get_factory() noexcept
    {
        return *m_factory;
    }

    template <size_type BufferSize, factory_type Factory, typename UtilType, bool Untyped>
    constexpr const static_memory_resource_base<BufferSize, Factory, UtilType, Untyped>::factory_t&
        static_memory_resource_base<BufferSize, Factory, UtilType, Untyped>::get_factory() const noexcept
    {
        return *m_factory;
    }

    template <size_type BufferSize, factory_type Factory, typename UtilType, bool Untyped>
    constexpr static_memory_resource_base<BufferSize, Factory, UtilType, Untyped>::factory_storage_type&
    static_memory_resource_base<BufferSize, Factory, UtilType, Untyped>::get_factory_storage() noexcept
    {
        return m_factory;
    }

    template <size_type BufferSize, factory_type Factory, typename UtilType, bool Untyped>
    constexpr const static_memory_resource_base<BufferSize, Factory, UtilType, Untyped>::factory_storage_type&
        static_memory_resource_base<BufferSize, Factory, UtilType, Untyped>::get_factory_storage() const noexcept
    {
        return m_factory;
    }

    template <size_type BufferSize, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void static_memory_resource_base<BufferSize, Factory, UtilType, Untyped>::construct(
            const size_type count,
            const size_type offset
            ) noexcept(factory_traits<Factory>::template noexcept_constructable<>)
    {
        m_factory.construct(advance(get_ptr(), offset * m_factory.type_size()), count);
    }

    template <size_type BufferSize, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void static_memory_resource_base<BufferSize, Factory, UtilType, Untyped>::destroy(
            const size_type count,
            const size_type offset
            ) noexcept
    {
        m_factory.destroy(advance(get_ptr(), offset * m_factory.type_size()), count);
    }

    template <size_type BufferSize, factory_type Factory, typename UtilType, bool Untyped>
    constexpr UtilType* static_memory_resource_base<BufferSize, Factory, UtilType, Untyped>::get_ptr() noexcept
    {
        return m_buffer.data;
    }

    template <size_type BufferSize, factory_type Factory, typename UtilType, bool Untyped>
    constexpr const UtilType* static_memory_resource_base<BufferSize, Factory, UtilType, Untyped>::get_ptr() const noexcept
    {
        return m_buffer.data;
    }
} // namespace rsl

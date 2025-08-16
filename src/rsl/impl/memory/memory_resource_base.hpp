#pragma once

#include "allocator_storage.hpp"
#include "factory_storage.hpp"
#include "mock_allocator.hpp"
#include "typed_allocator.hpp"

namespace rsl
{
    template <typename T, size_type Capacity>
    struct static_capacity_storage
    {
        T data[Capacity];
    };

    template <typename T>
    struct static_capacity_storage<T, 0ull> {};

    template <
        allocator_type Alloc = default_allocator, untyped_factory_type Factory = type_erased_factory,
        typename UtilType = void>
    class untyped_dynamic_memory_resource_base
    {
    public:
        using allocator_storage_type = allocator_storage<Alloc>;
        using allocator_t = Alloc;
        using factory_storage_type = factory_storage<Factory>;
        using factory_t = Factory;
        using typed_alloc_type = type_erased_allocator<Alloc, Factory>;

        [[rythe_always_inline]] constexpr untyped_dynamic_memory_resource_base()
            noexcept(is_nothrow_constructible_v<typed_alloc_type>) = default;
        [[rythe_always_inline]] constexpr untyped_dynamic_memory_resource_base(
                const untyped_dynamic_memory_resource_base& other)
            noexcept(is_nothrow_copy_constructible_v<typed_alloc_type>) = default;
        [[rythe_always_inline]] constexpr untyped_dynamic_memory_resource_base(
                untyped_dynamic_memory_resource_base&&)
            noexcept(is_nothrow_move_constructible_v<typed_alloc_type>) = default;

        [[rythe_always_inline]] constexpr untyped_dynamic_memory_resource_base& operator=(
                const untyped_dynamic_memory_resource_base&)
            noexcept(is_nothrow_copy_assignable_v<typed_alloc_type>) = default;
        [[rythe_always_inline]] constexpr untyped_dynamic_memory_resource_base& operator=(
                untyped_dynamic_memory_resource_base&&)
            noexcept(is_nothrow_move_assignable_v<typed_alloc_type>) = default;

        virtual ~untyped_dynamic_memory_resource_base() = default;

        [[rythe_always_inline]] constexpr explicit untyped_dynamic_memory_resource_base(
                const allocator_storage_type& allocStorage
                ) noexcept(is_nothrow_constructible_v<typed_alloc_type, const allocator_storage_type&>);

        [[rythe_always_inline]] constexpr explicit untyped_dynamic_memory_resource_base(
                const factory_storage_type& factoryStorage
                ) noexcept(is_nothrow_constructible_v<typed_alloc_type, const factory_storage_type&>);

        [[rythe_always_inline]] constexpr untyped_dynamic_memory_resource_base(
                const allocator_storage_type& allocStorage,
                const factory_storage_type& factoryStorage
                )
            noexcept(is_nothrow_constructible_v<
                typed_alloc_type, const allocator_storage_type&, const factory_storage_type&>);

        [[rythe_always_inline]] constexpr void set_allocator(
                const allocator_storage_type& allocStorage)
            noexcept(is_nothrow_copy_assignable_v<allocator_storage_type>);

        [[nodiscard]] [[rythe_always_inline]] constexpr allocator_t& get_allocator() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const allocator_t& get_allocator() const noexcept;

        [[rythe_always_inline]] constexpr void set_factory(
                const factory_storage_type& factoryStorage)
            noexcept(is_nothrow_copy_assignable_v<factory_storage_type>);

        [[nodiscard]] [[rythe_always_inline]] constexpr factory_t& get_factory() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const factory_t& get_factory() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr allocator_storage_type& get_allocator_storage() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const allocator_storage_type&
        get_allocator_storage() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr factory_storage_type& get_factory_storage() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const factory_storage_type&
        get_factory_storage() const noexcept;

    protected:
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void allocate(size_type count = 1) noexcept;
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void
        allocate(size_type count, size_type alignment) noexcept;
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void reallocate(
                size_type oldCount,
                size_type newCount)
            noexcept(factory_traits<Factory>::noexcept_moveable);
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void
        reallocate(
                size_type oldCount,
                size_type newCount,
                size_type alignment)
            noexcept(factory_traits<Factory>::noexcept_moveable);
        [[rythe_always_inline]] constexpr void deallocate(size_type count = 1) noexcept;
        [[rythe_always_inline]] constexpr void deallocate(size_type count, size_type alignment) noexcept;

        [[rythe_always_inline]] constexpr void construct(
                size_type count = 1,
                size_type offset = 0)
            noexcept(factory_traits<Factory>::template noexcept_constructable<>);
        [[rythe_always_inline]] constexpr void destroy(size_type count = 1, size_type offset = 0) noexcept;

        [[rythe_allocating]] [[rythe_always_inline]] constexpr void allocate_and_construct(
                size_type count = 1)
            noexcept(factory_traits<Factory>::template noexcept_constructable<>);

        template <typename... Args>
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void
        allocate_aligned_and_construct(
                size_type count,
                size_type alignment)
            noexcept(factory_traits<Factory>::template noexcept_constructable<>);

        template <typename... Args>
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void
        reallocate_and_construct(
                size_type oldCount,
                size_type newCount) noexcept(
            factory_traits<Factory>::template noexcept_constructable<> && factory_traits<Factory>::noexcept_moveable
        );

        template <typename... Args>
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void
        reallocate_aligned_and_construct(
                size_type oldCount,
                size_type newCount,
                size_type alignment) noexcept(
            factory_traits<Factory>::template noexcept_constructable<> && factory_traits<Factory>::noexcept_moveable
        );

        [[rythe_always_inline]] constexpr void destroy_and_deallocate(size_type count = 1) noexcept;
        [[rythe_always_inline]] constexpr void
        destroy_and_deallocate_aligned(size_type count, size_type alignment) noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr UtilType* get_ptr() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const UtilType* get_ptr() const noexcept;
        [[rythe_always_inline]] constexpr void set_ptr(UtilType* const& ptr) noexcept;

        typed_alloc_type m_alloc;
        void* m_ptr = nullptr;
    };

    template <typename T, allocator_type Alloc = default_allocator, factory_type Factory = default_factory<T>>
    class typed_dynamic_memory_resource_base
    {
    public:
        using value_type = T;
        using ref_type = add_lval_ref_t<T>;
        using const_ref_type = decorate_type_t<T, const_signal, lval_ref_signal>;
        using ptr_type = add_pointer_t<T>;
        using const_ptr_type = decorate_type_t<T, const_signal, pointer_signal>;
        using allocator_storage_type = allocator_storage<Alloc>;
        using allocator_t = Alloc;
        using factory_storage_type = factory_storage<Factory>;
        using factory_t = Factory;
        using typed_alloc_type = typed_allocator<T, Alloc, Factory>;

        [[rythe_always_inline]] constexpr typed_dynamic_memory_resource_base()
            noexcept(is_nothrow_constructible_v<typed_alloc_type>) = default;

        virtual ~typed_dynamic_memory_resource_base() = default;

        [[rythe_always_inline]] constexpr explicit typed_dynamic_memory_resource_base(
                const allocator_storage_type& allocStorage
                ) noexcept(is_nothrow_constructible_v<typed_alloc_type, const allocator_storage_type&>);

        [[rythe_always_inline]] constexpr explicit typed_dynamic_memory_resource_base(
                const factory_storage_type& factoryStorage
                ) noexcept(is_nothrow_constructible_v<typed_alloc_type, const factory_storage_type&>);

        [[rythe_always_inline]] constexpr typed_dynamic_memory_resource_base(
                const allocator_storage_type& allocStorage,
                const factory_storage_type& factoryStorage
                )
            noexcept(is_nothrow_constructible_v<
                typed_alloc_type, const allocator_storage_type&, const factory_storage_type&>);

        [[rythe_always_inline]] constexpr void set_allocator(
                const allocator_storage_type& allocStorage)
            noexcept(is_nothrow_copy_assignable_v<allocator_storage_type>);

        [[nodiscard]] [[rythe_always_inline]] constexpr allocator_t& get_allocator() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const allocator_t& get_allocator() const noexcept;

        [[rythe_always_inline]] constexpr void set_factory(
                const factory_storage_type& factoryStorage)
            noexcept(is_nothrow_copy_assignable_v<factory_storage_type>);

        [[nodiscard]] [[rythe_always_inline]] constexpr factory_t& get_factory() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const factory_t& get_factory() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr allocator_storage_type& get_allocator_storage() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const allocator_storage_type&
        get_allocator_storage() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr factory_storage_type& get_factory_storage() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const factory_storage_type&
        get_factory_storage() const noexcept;

    protected:
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void allocate(size_type count = 1) noexcept;
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void
        allocate(size_type count, size_type alignment) noexcept;
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void reallocate(
                size_type oldCount,
                size_type newCount)
            noexcept(factory_traits<Factory>::noexcept_moveable);
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void
        reallocate(
                size_type oldCount,
                size_type newCount,
                size_type alignment)
            noexcept(factory_traits<Factory>::noexcept_moveable);
        [[rythe_always_inline]] constexpr void deallocate(size_type count = 1) noexcept;
        [[rythe_always_inline]] constexpr void deallocate(size_type count, size_type alignment) noexcept;

        template <typename... Args>
        [[rythe_always_inline]] constexpr void construct(
                size_type count = 1,
                size_type offset = 0,
                Args&&... args)
            noexcept(factory_traits<Factory>::template noexcept_constructable<Args...>);
        [[rythe_always_inline]] constexpr void destroy(size_type count = 1, size_type offset = 0) noexcept;

        template <typename... Args>
        [[rythe_always_inline]] constexpr void copy(
                size_type count,
                size_type offset,
                const T* src)
            noexcept(factory_traits<Factory>::noexcept_copyable);
        template <typename... Args>
        [[rythe_always_inline]] constexpr void move(
                size_type count,
                size_type offset,
                T* src)
            noexcept(factory_traits<Factory>::noexcept_moveable);

        template <typename... Args>
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void
        allocate_and_construct(
                size_type count = 1,
                Args&&... args)
            noexcept(factory_traits<Factory>::template noexcept_constructable<Args...>);

        template <typename... Args>
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void
        allocate_aligned_and_construct(
                size_type count,
                size_type alignment,
                Args&&... args)
            noexcept(factory_traits<Factory>::template noexcept_constructable<Args...>);

        template <typename... Args>
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void
        reallocate_and_construct(
                size_type oldCount,
                size_type newCount,
                Args&&... args) noexcept(
            factory_traits<Factory>::template noexcept_constructable<Args...> &&
            factory_traits<Factory>::noexcept_moveable
        );

        template <typename... Args>
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void
        reallocate_aligned_and_construct(
                size_type oldCount,
                size_type newCount,
                size_type alignment,
                Args&&... args)
            noexcept(
                factory_traits<Factory>::template noexcept_constructable<Args...> &&
                factory_traits<Factory>::noexcept_moveable
            );

        [[rythe_always_inline]] constexpr void destroy_and_deallocate(size_type count = 1) noexcept;
        [[rythe_always_inline]] constexpr void
        destroy_and_deallocate_aligned(size_type count, size_type alignment) noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr T* get_ptr() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const T* get_ptr() const noexcept;
        [[rythe_always_inline]] constexpr void set_ptr(T* const& ptr) noexcept;

        typed_alloc_type m_alloc;
        ptr_type m_ptr = nullptr;
    };

    template <size_type BufferSize, untyped_factory_type Factory = type_erased_factory,
              typename UtilType = void>
    class untyped_static_memory_resource_base
    {
    public:
        using factory_storage_type = factory_storage<Factory>;
        using factory_t = Factory;
        constexpr static size_type buffer_size = BufferSize;

        [[rythe_always_inline]] constexpr untyped_static_memory_resource_base()
            noexcept(is_nothrow_constructible_v<factory_storage_type>) = default;
        [[rythe_always_inline]] constexpr untyped_static_memory_resource_base(
                const untyped_static_memory_resource_base& other)
            noexcept(is_nothrow_copy_constructible_v<factory_storage_type>) = default;
        [[rythe_always_inline]] constexpr untyped_static_memory_resource_base(
                untyped_static_memory_resource_base&&)
            noexcept(is_nothrow_move_constructible_v<factory_storage_type>) = default;

        [[rythe_always_inline]] constexpr untyped_static_memory_resource_base& operator=(
                const untyped_static_memory_resource_base&)
            noexcept(is_nothrow_copy_assignable_v<factory_storage_type>) = default;
        [[rythe_always_inline]] constexpr untyped_static_memory_resource_base& operator=(
                untyped_static_memory_resource_base&&)
            noexcept(is_nothrow_move_assignable_v<factory_storage_type>) = default;

        virtual ~untyped_static_memory_resource_base() = default;

        [[rythe_always_inline]] constexpr explicit untyped_static_memory_resource_base(
                const factory_storage_type& factoryStorage
                ) noexcept(is_nothrow_copy_constructible_v<factory_storage_type>);

        [[rythe_always_inline]] constexpr explicit untyped_static_memory_resource_base(
                factory_storage_type&& factoryStorage
                ) noexcept(is_nothrow_move_constructible_v<factory_storage_type>);

        [[rythe_always_inline]] constexpr void set_factory(
                const factory_storage_type& factoryStorage)
            noexcept(is_nothrow_copy_assignable_v<factory_storage_type>);

        [[nodiscard]] [[rythe_always_inline]] constexpr factory_t& get_factory() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const factory_t& get_factory() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr factory_storage_type& get_factory_storage() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const factory_storage_type&
        get_factory_storage() const noexcept;

    protected:
        [[rythe_always_inline]] constexpr void construct(
                size_type count = 1,
                size_type offset = 0)
            noexcept(factory_traits<Factory>::template noexcept_constructable<>);
        [[rythe_always_inline]] constexpr void destroy(size_type count = 1, size_type offset = 0) noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr UtilType* get_ptr() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const UtilType* get_ptr() const noexcept;

        byte m_data[buffer_size];
    };

    template <typename T, size_type BufferCount, factory_type Factory = default_factory<T>>
    class typed_static_memory_resource_base
    {
    public:
        using value_type = T;
        using ref_type = add_lval_ref_t<T>;
        using const_ref_type = decorate_type_t<T, const_signal, lval_ref_signal>;
        using ptr_type = add_pointer_t<T>;
        using const_ptr_type = decorate_type_t<T, const_signal, pointer_signal>;
        using factory_storage_type = factory_storage<Factory>;
        using factory_t = Factory;
        constexpr static size_type buffer_size = BufferCount * sizeof(T);
        constexpr static size_type buffer_count = BufferCount;

        [[rythe_always_inline]] constexpr typed_static_memory_resource_base()
            noexcept(is_nothrow_constructible_v<factory_storage_type>) = default;

        virtual ~typed_static_memory_resource_base() = default;

        [[rythe_always_inline]] constexpr explicit typed_static_memory_resource_base(
                const factory_storage_type& factoryStorage)
            noexcept(is_nothrow_copy_constructible_v<factory_storage_type>);
        [[rythe_always_inline]] constexpr explicit typed_static_memory_resource_base(
                factory_storage_type&& factoryStorage)
            noexcept(is_nothrow_move_constructible_v<factory_storage_type>);

        [[rythe_always_inline]] constexpr void set_factory(
                const factory_storage_type& factoryStorage)
            noexcept(is_nothrow_copy_assignable_v<factory_storage_type>);
        [[rythe_always_inline]] constexpr void set_factory(
                factory_storage_type&& factoryStorage)
            noexcept(is_nothrow_move_assignable_v<factory_storage_type>);

        [[nodiscard]] [[rythe_always_inline]] constexpr factory_t& get_factory() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const factory_t& get_factory() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr factory_storage_type& get_factory_storage() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const factory_storage_type& get_factory_storage() const noexcept;

    protected:
        template <typename... Args>
        [[rythe_always_inline]] constexpr void construct(
                size_type count = 1,
                size_type offset = 0,
                Args&&... args)
            noexcept(factory_traits<Factory>::template noexcept_constructable<Args...>);
        [[rythe_always_inline]] constexpr void destroy(size_type count = 1, size_type offset = 0) noexcept;

        template <typename... Args>
        [[rythe_always_inline]] constexpr void copy(
                size_type count,
                size_type offset,
                const T* src)
            noexcept(factory_traits<Factory>::noexcept_copyable);
        template <typename... Args>
        [[rythe_always_inline]] constexpr void move(
                size_type count,
                size_type offset,
                T* src)
            noexcept(factory_traits<Factory>::noexcept_moveable);

        [[nodiscard]] [[rythe_always_inline]] constexpr T* get_ptr() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const T* get_ptr() const noexcept;

        value_type m_data[buffer_count];
    };

    template <size_type BufferSize,
        allocator_type Alloc = default_allocator, untyped_factory_type Factory = type_erased_factory,
        typename UtilType = void>
    class untyped_hybrid_memory_resource_base
    {};

    template <typename T, size_type BufferCount, allocator_type Alloc = default_allocator, factory_type Factory = default_factory<T>>
    class typed_hybrid_memory_resource_base
    {
    public:
        using value_type = T;
        using ref_type = add_lval_ref_t<T>;
        using const_ref_type = decorate_type_t<T, const_signal, lval_ref_signal>;
        using ptr_type = add_pointer_t<T>;
        using const_ptr_type = decorate_type_t<T, const_signal, pointer_signal>;
        using allocator_storage_type = allocator_storage<Alloc>;
        using allocator_t = Alloc;
        using factory_storage_type = factory_storage<Factory>;
        using factory_t = Factory;
        using typed_alloc_type = typed_allocator<T, Alloc, Factory>;
        constexpr static size_type buffer_size = BufferCount * sizeof(T);
        constexpr static size_type buffer_count = BufferCount;

        [[rythe_always_inline]] constexpr typed_hybrid_memory_resource_base()
            noexcept(is_nothrow_constructible_v<typed_alloc_type>) = default;

        virtual ~typed_hybrid_memory_resource_base() = default;

        [[rythe_always_inline]] constexpr explicit typed_hybrid_memory_resource_base(
                const allocator_storage_type& allocStorage
                ) noexcept(is_nothrow_constructible_v<typed_alloc_type, const allocator_storage_type&>);

        [[rythe_always_inline]] constexpr explicit typed_hybrid_memory_resource_base(
                const factory_storage_type& factoryStorage
                ) noexcept(is_nothrow_constructible_v<typed_alloc_type, const factory_storage_type&>);

        [[rythe_always_inline]] constexpr typed_hybrid_memory_resource_base(
                const allocator_storage_type& allocStorage,
                const factory_storage_type& factoryStorage
                )
            noexcept(is_nothrow_constructible_v<
                typed_alloc_type, const allocator_storage_type&, const factory_storage_type&>);

        [[rythe_always_inline]] constexpr void set_allocator(
                const allocator_storage_type& allocStorage)
            noexcept(is_nothrow_copy_assignable_v<allocator_storage_type>);

        [[nodiscard]] [[rythe_always_inline]] constexpr allocator_t& get_allocator() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const allocator_t& get_allocator() const noexcept;

        [[rythe_always_inline]] constexpr void set_factory(
                const factory_storage_type& factoryStorage)
            noexcept(is_nothrow_copy_assignable_v<factory_storage_type>);

        [[nodiscard]] [[rythe_always_inline]] constexpr factory_t& get_factory() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const factory_t& get_factory() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr allocator_storage_type& get_allocator_storage() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const allocator_storage_type&
        get_allocator_storage() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr factory_storage_type& get_factory_storage() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const factory_storage_type&
        get_factory_storage() const noexcept;

    protected:
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void allocate(size_type count = 1) noexcept;
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void
        allocate(size_type count, size_type alignment) noexcept;
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void reallocate(
                size_type oldCount,
                size_type newCount)
            noexcept(factory_traits<Factory>::noexcept_moveable);
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void
        reallocate(
                size_type oldCount,
                size_type newCount,
                size_type alignment)
            noexcept(factory_traits<Factory>::noexcept_moveable);
        [[rythe_always_inline]] constexpr void deallocate(size_type count = 1) noexcept;
        [[rythe_always_inline]] constexpr void deallocate(size_type count, size_type alignment) noexcept;

        template <typename... Args>
        [[rythe_always_inline]] constexpr void construct(
                size_type count = 1,
                size_type offset = 0,
                Args&&... args)
            noexcept(factory_traits<Factory>::template noexcept_constructable<Args...>);
        [[rythe_always_inline]] constexpr void destroy(size_type count = 1, size_type offset = 0) noexcept;

        template <typename... Args>
        [[rythe_always_inline]] constexpr void copy(
                size_type count,
                size_type offset,
                const T* src)
            noexcept(factory_traits<Factory>::noexcept_copyable);
        template <typename... Args>
        [[rythe_always_inline]] constexpr void move(
                size_type count,
                size_type offset,
                T* src)
            noexcept(factory_traits<Factory>::noexcept_moveable);

        template <typename... Args>
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void
        allocate_and_construct(
                size_type count = 1,
                Args&&... args)
            noexcept(factory_traits<Factory>::template noexcept_constructable<Args...>);

        template <typename... Args>
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void
        allocate_aligned_and_construct(
                size_type count,
                size_type alignment,
                Args&&... args)
            noexcept(factory_traits<Factory>::template noexcept_constructable<Args...>);

        template <typename... Args>
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void
        reallocate_and_construct(
                size_type oldCount,
                size_type newCount,
                Args&&... args) noexcept(
            factory_traits<Factory>::template noexcept_constructable<Args...> &&
            factory_traits<Factory>::noexcept_moveable
        );

        template <typename... Args>
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void
        reallocate_aligned_and_construct(
                size_type oldCount,
                size_type newCount,
                size_type alignment,
                Args&&... args)
            noexcept(
                factory_traits<Factory>::template noexcept_constructable<Args...> &&
                factory_traits<Factory>::noexcept_moveable
            );

        [[rythe_always_inline]] constexpr void destroy_and_deallocate(size_type count = 1) noexcept;
        [[rythe_always_inline]] constexpr void
        destroy_and_deallocate_aligned(size_type count, size_type alignment) noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr T* get_ptr() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const T* get_ptr() const noexcept;
        [[rythe_always_inline]] constexpr void set_ptr(T* const& ptr) noexcept;

        union
        {
            struct
            {
                typed_alloc_type m_alloc;
                ptr_type m_ptr = nullptr;
            };
            static_capacity_storage<T, BufferCount> m_staticStorage;
        };
    };

    namespace internal
    {
        template <typename T, allocator_type Alloc, factory_type Factory, size_type StaticStorageCount, bool CanAllocate = !is_same_v<Alloc, mock_allocator>, bool Untyped = untyped_factory_type<Factory>>
        struct select_memory_resource;

        template <typename T, allocator_type Alloc, factory_type Factory>
        struct select_memory_resource<T, Alloc, Factory, 0ull, true, false>
        {
            static_assert(!is_same_v<Alloc, mock_allocator>);
            using type = typed_dynamic_memory_resource_base<T, Alloc, Factory>;
            constexpr static bool is_untyped = false;
        };

        template <typename T, allocator_type Alloc, factory_type Factory>
        struct select_memory_resource<T, Alloc, Factory, 0ull, true, true>
        {
            static_assert(!is_same_v<Alloc, mock_allocator>);
            using type = untyped_dynamic_memory_resource_base<Alloc, Factory, T>;
            constexpr static bool is_untyped = true;
        };

        template <typename T, allocator_type Alloc, factory_type Factory, size_type StaticStorageCount>
        struct select_memory_resource<T, Alloc, Factory, StaticStorageCount, true, false>
        {
            static_assert(!is_same_v<Alloc, mock_allocator>);
            using type = typed_hybrid_memory_resource_base<T, StaticStorageCount, Alloc, Factory>;
            constexpr static bool is_untyped = false;
        };

        template <typename T, allocator_type Alloc, factory_type Factory, size_type StaticStorageCount>
        struct select_memory_resource<T, Alloc, Factory, StaticStorageCount, true, true>
        {
            static_assert(!is_same_v<Alloc, mock_allocator>);
            using type = untyped_hybrid_memory_resource_base<StaticStorageCount * sizeof(T), Alloc, Factory, T>;
            constexpr static bool is_untyped = true;
        };

        template <typename T, factory_type Factory, size_type StaticStorageCount, bool CanAllocate>
        struct select_memory_resource<T, mock_allocator, Factory, StaticStorageCount, CanAllocate, false>
        {
            static_assert(CanAllocate == false, "Allocating using a mock_allocator is not possible.");
            using type = typed_static_memory_resource_base<T, StaticStorageCount, Factory>;
            constexpr static bool is_untyped = false;
        };

        template <typename T, factory_type Factory, size_type StaticStorageCount, bool CanAllocate>
        struct select_memory_resource<T, mock_allocator, Factory, StaticStorageCount, CanAllocate, true>
        {
            static_assert(CanAllocate == false, "Allocating using a mock_allocator is not possible.");
            using type = untyped_static_memory_resource_base<StaticStorageCount * sizeof(T), Factory, T>;
            constexpr static bool is_untyped = true;
        };

        template <typename T, allocator_type Alloc, factory_type Factory, size_type StaticStorageCount>
        struct select_memory_resource<T, Alloc, Factory, StaticStorageCount, false, false> : public select_memory_resource<T, mock_allocator, Factory, StaticStorageCount, false, false>
        {
        };

        template <typename T, allocator_type Alloc, factory_type Factory, size_type StaticStorageCount>
        struct select_memory_resource<T, Alloc, Factory, StaticStorageCount, false, true> : public select_memory_resource<T, mock_allocator, Factory, StaticStorageCount, false, true>
        {
        };
    } // namespace internal
}     // namespace rsl

#include "memory_resource_base.inl"

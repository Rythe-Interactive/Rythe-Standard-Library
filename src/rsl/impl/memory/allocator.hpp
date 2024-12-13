#pragma once

#include "../util/assert.hpp"
#include "../util/common.hpp"
#include "../util/concepts.hpp"
#include "../util/type_util.hpp"

#include "heap_allocator.hpp"

namespace rsl
{
	template <typename T>
	concept allocator_type = requires(T alloc, typename T::value_type* ptr, size_type n) {
		{ alloc.allocate(n) } -> convertible_to<typename T::value_type*>;
		{ alloc.allocate(n, n) } -> convertible_to<typename T::value_type*>;
		{ alloc.reallocate(ptr, n, n) } -> convertible_to<typename T::value_type*>;
		{ alloc.reallocate(ptr, n, n, n) } -> convertible_to<typename T::value_type*>;
		{ alloc.deallocate(ptr, n) };
		{ alloc.deallocate(ptr, n, n) };
	};

	template <typename T>
	concept noexcept_allocator_type = requires(T alloc, typename T::value_type* ptr, size_type n) {
		{ alloc.allocate(n) } noexcept -> convertible_to<typename T::value_type*>;
		{ alloc.allocate(n, n) } noexcept -> convertible_to<typename T::value_type*>;
		{ alloc.reallocate(ptr, n, n) } noexcept -> convertible_to<typename T::value_type*>;
		{ alloc.reallocate(ptr, n, n, n) } noexcept -> convertible_to<typename T::value_type*>;
		{ alloc.deallocate(ptr, n) } noexcept;
		{ alloc.deallocate(ptr, n, n) } noexcept;
	};

	template <typename T>
	concept universal_allocator_type = allocator_type<T> && same_as<typename T::value_type, void>;

#if !defined(RSL_DEFAULT_ALLOCATOR_OVERRIDE)
	using default_allocator = heap_allocator;
#else
	using default_allocator = RSL_DEFAULT_ALLOCATOR_OVERRIDE;
#endif

	class polymorphic_universal_allocator
	{
	public:
		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] virtual void* allocate(size_type size) noexcept = 0;
		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] virtual void*
		allocate(size_type size, size_type alignment) noexcept = 0;

		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] virtual void*
		reallocate(void* ptr, size_type oldSize, size_type newSize) noexcept = 0;
		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] virtual void*
		reallocate(void* ptr, size_type oldSize, size_type newSize, size_type alignment) noexcept = 0;

		[[rythe_always_inline]] virtual void deallocate(void* ptr, size_type size) noexcept = 0;
		[[rythe_always_inline]] virtual void deallocate(void* ptr, size_type size, size_type alignment) noexcept = 0;
	};

	using pmu_allocator = polymorphic_universal_allocator;

	template <rsl::universal_allocator_type Alloc = default_allocator>
	class polymorphic_universal_allocator_mixin final : public polymorphic_universal_allocator
	{
	public:
		using value_type = void;

		Alloc impl;

		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] void* allocate(size_type size) noexcept override
		{
			return impl.allocate(size);
		}

		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] void*
		allocate(size_type size, size_type alignment) noexcept override
		{
			return impl.allocate(size, alignment);
		}

		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] void*
		reallocate(void* ptr, size_type oldSize, size_type newSize) noexcept override
		{
			return impl.reallocate(ptr, oldSize, newSize);
		}

		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] void*
		reallocate(void* ptr, size_type oldSize, size_type newSize, size_type alignment) noexcept override
		{
			return impl.reallocate(ptr, oldSize, newSize, alignment);
		}

		[[rythe_always_inline]] void deallocate(void* ptr, size_type size) noexcept override
		{
			impl.deallocate(ptr, size);
		}

		[[rythe_always_inline]] void deallocate(void* ptr, size_type size, size_type alignment) noexcept override
		{
			impl.deallocate(ptr, size, alignment);
		}
	};

	class polymorphic_universal_allocator_ptr_wrapper
	{
	public:
		using value_type = void;

		pmu_allocator* allocatorPtr;

		constexpr polymorphic_universal_allocator_ptr_wrapper() noexcept = default;
		constexpr polymorphic_universal_allocator_ptr_wrapper(pmu_allocator* ptr) noexcept
			: allocatorPtr(ptr)
		{
		}


		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] void* allocate(size_type size) noexcept;
		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] void*
		allocate(size_type size, size_type alignment) noexcept;

		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] void*
		reallocate(void* ptr, size_type oldSize, size_type newSize) noexcept;
		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] void*
		reallocate(void* ptr, size_type oldSize, size_type newSize, size_type alignment) noexcept;

		[[rythe_always_inline]] void deallocate(void* ptr, size_type size) noexcept;
		[[rythe_always_inline]] void deallocate(void* ptr, size_type size, size_type alignment) noexcept;
	};

	using pmu_alloc_ptr_wrapper = polymorphic_universal_allocator_ptr_wrapper;

	using default_polymorphic_allocator = polymorphic_universal_allocator_mixin<default_allocator>;
	using default_pmu_allocator = default_polymorphic_allocator;

	template <typename T>
	class default_constructor
	{
	public:
		using value_type = T;

		template <typename Other>
		using retarget = default_constructor<Other>;

		template <typename... Args>
		constexpr static bool noexcept_constructable = is_nothrow_constructible_v<T, Args...>;
		constexpr static bool noexcept_moveable = is_nothrow_move_constructible_v<T>;

		template <typename... Args>
		T* construct(void* ptr, size_type count, Args&&... args) noexcept(noexcept_constructable<Args...>);

		T* move(void* dst, T* src, size_type count) noexcept(noexcept_moveable);

		void destroy(T* ptr, size_type count) noexcept;
	};

	template <typename T>
	concept constructor_type = requires(T ctor, void* mem, typename T::value_type* ptr, size_type n) {
		{ ctor.construct(mem, n) } -> convertible_to<typename T::value_type*>;
		{ ctor.move(mem, ptr, n) } -> convertible_to<typename T::value_type*>;
		{ ctor.destroy(ptr, n) } noexcept;
	};

	template <typename T>
	concept noexcept_constructor_type = requires(T ctor, void* mem, typename T::value_type* ptr, size_type n) {
		{ ctor.construct(mem, n) } noexcept -> convertible_to<typename T::value_type*>;
		{ ctor.move(mem, ptr, n) } noexcept -> convertible_to<typename T::value_type*>;
		{ ctor.destroy(ptr, n) } noexcept;
	};

	template <
		typename T, universal_allocator_type UniversalAlloc = default_allocator,
		constructor_type Constructor = default_constructor<T>>
	class allocator final : public UniversalAlloc, private Constructor
	{
	public:
		using value_type = T;
		using universal_type = UniversalAlloc;

		allocator() = default;
		allocator(const UniversalAlloc& uAlloc, const Constructor& construct = {})
			: UniversalAlloc(uAlloc),
			  Constructor(construct)
		{
		}
		allocator(UniversalAlloc&& uAlloc, const Constructor& construct = {})
			: UniversalAlloc(move(uAlloc)),
			  Constructor(construct)
		{
		}
		allocator(const UniversalAlloc& uAlloc, Constructor&& construct)
			: UniversalAlloc(uAlloc),
			  Constructor(move(construct))
		{
		}
		allocator(UniversalAlloc&& uAlloc, Constructor&& construct)
			: UniversalAlloc(move(uAlloc)),
			  Constructor(move(construct))
		{
		}

		universal_type& as_universal() noexcept;
		const universal_type& as_universal() const noexcept;

		template <typename... Args>
		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] T* allocate(size_type count = 1, Args&&... args)
			noexcept(Constructor::template noexcept_constructable<Args...>);

		template <typename... Args>
		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] T*
		allocate(size_type count, size_type alignment, Args&&... args)
			noexcept(Constructor::template noexcept_constructable<Args...>);

		template <typename... Args>
		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] T*
		reallocate(T* ptr, size_type oldCount, size_type newCount, Args&&... args)
			noexcept(Constructor::template noexcept_constructable<Args...> && Constructor::noexcept_moveable);

		template <typename... Args>
		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] T*
		reallocate(T* ptr, size_type oldCount, size_type newCount, size_type alignment, Args&&... args)
			noexcept(Constructor::template noexcept_constructable<Args...> && Constructor::noexcept_moveable);

		[[rythe_always_inline]] void deallocate(T* ptr, size_type count = 1) noexcept;
		[[rythe_always_inline]] void deallocate(T* ptr, size_type count, size_type alignment) noexcept;

		template <typename Other>
		using retarget = allocator<Other, UniversalAlloc, Constructor>;
	};

	template <allocator_type Alloc>
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
		constexpr stl_allocator_compatible_wrapper(const Alloc& other) noexcept
			: alloc(other)
		{
		}
		template <class _Other>
		constexpr stl_allocator_compatible_wrapper(
			const stl_allocator_compatible_wrapper<typename Alloc::template retarget<_Other>>& other
		) noexcept
			: alloc(other.alloc)
		{
		}

		constexpr ~stl_allocator_compatible_wrapper() = default;
		constexpr stl_allocator_compatible_wrapper& operator=(const stl_allocator_compatible_wrapper&) = default;

		constexpr void deallocate(value_type* const ptr, const size_t count)
		{
			alloc.as_universal().deallocate(ptr, count * sizeof(value_type));
		}

		[[nodiscard]] [[rythe_allocating]] constexpr value_type* allocate(const size_t count)
		{
			return static_cast<value_type*>(alloc.as_universal().allocate(count * sizeof(value_type)));
		}
	};

	template <allocator_type Alloc>
	struct stl_deleter_compatible_wrapper
	{
		Alloc alloc;
		using value_type = Alloc::value_type;

		constexpr void operator()(value_type* ptr) const noexcept { alloc.deallocate(ptr); }
	};

	template <typename T>
	using stl_pmu_alloc = stl_allocator_compatible_wrapper<allocator<T, pmu_alloc_ptr_wrapper>>;

	template <typename T>
	using stl_pmu_deleter = stl_deleter_compatible_wrapper<allocator<T, pmu_alloc_ptr_wrapper>>;

	template <typename T>
	[[nodiscard]] [[rythe_always_inline]] constexpr stl_pmu_deleter<T> make_stl_pmu_deleter(pmu_allocator* alloc
	) noexcept
	{
		return stl_pmu_deleter<T>{allocator<T, pmu_alloc_ptr_wrapper>{pmu_alloc_ptr_wrapper{alloc}}};
	}

	struct allocator_context
	{
		static default_pmu_allocator defaultGlobalAllocator;
		static pmu_allocator* globalAllocator;
		static thread_local pmu_allocator* threadSpecificAllocator;
	};

	template <typename T, typename... Args>
	[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] T*
	allocate(pmu_allocator& alloc, size_type count = 1, Args&&... args)
		noexcept(default_constructor<T>::template noexcept_constructable<Args...>);

	template <typename T, typename... Args>
	[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] T*
	allocate(pmu_allocator& alloc, size_type count, size_type alignment, Args&&... args)
		noexcept(default_constructor<T>::template noexcept_constructable<Args...>);

	template <typename T, typename... Args>
	[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] T*
	reallocate(pmu_allocator& alloc, T* ptr, size_type oldCount, size_type newCount, Args&&... args) noexcept(
		default_constructor<T>::template noexcept_constructable<Args...> && default_constructor<T>::noexcept_moveable
	);

	template <typename T, typename... Args>
	[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] T* reallocate(
		pmu_allocator& alloc, T* ptr, size_type oldCount, size_type newCount, size_type alignment, Args&&... args
	)
		noexcept(
			default_constructor<T>::template noexcept_constructable<Args...> &&
			default_constructor<T>::noexcept_moveable
		);

	template <typename T>
	[[rythe_always_inline]] void deallocate(pmu_allocator& alloc, T* ptr, size_type count = 1) noexcept;

	template <typename T>
	[[rythe_always_inline]] void
	deallocate(pmu_allocator& alloc, T* ptr, size_type count, size_type alignment) noexcept;
} // namespace rsl

#include "allocator.inl"

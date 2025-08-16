#pragma once

#include "../util/primitives.hpp"

#include "contiguous_container_base.hpp"

namespace rsl
{
	template <typename T, allocator_type Alloc = default_allocator, typed_factory_type Factory = default_factory<T>,
			  bool UsePostFix = false, size_type StaticCapacity = 0ull>
	class dynamic_array : public contiguous_container_base<T, Alloc, Factory, T*, const T*, contiguous_container_info<UsePostFix, StaticCapacity, true, true>>
	{
	public:
		using container_base = contiguous_container_base<T, Alloc, Factory, T*, const T*, contiguous_container_info<UsePostFix, StaticCapacity, true, true>>;
		using mem_rsc = typename container_base::mem_rsc;
		using value_type = T;
		using iterator_type = typename container_base::iterator_type;
		using const_iterator_type = typename container_base::const_iterator_type;
		using reverse_iterator_type = typename container_base::reverse_iterator_type;
		using const_reverse_iterator_type = typename container_base::const_reverse_iterator_type;
		using view_type = typename container_base::view_type;
		using const_view_type = typename container_base::const_view_type;
		using allocator_storage_type = allocator_storage<Alloc>;
		using allocator_t = Alloc;
		using factory_storage_type = factory_storage<Factory>;
		using factory_t = Factory;

		[[rythe_always_inline]] constexpr dynamic_array()
			noexcept(is_nothrow_constructible_v<container_base>) = default;

		[[rythe_always_inline]] constexpr dynamic_array(const dynamic_array& src)
			noexcept(container_base::copy_construct_noexcept);
		[[rythe_always_inline]] constexpr dynamic_array(dynamic_array&& src) noexcept;

		[[rythe_always_inline]] explicit constexpr dynamic_array(const allocator_storage_type& allocStorage)
			noexcept(is_nothrow_constructible_v<container_base, const allocator_storage_type&>);
		[[rythe_always_inline]] explicit constexpr dynamic_array(const factory_storage_type& factoryStorage)
			noexcept(is_nothrow_constructible_v<container_base, const factory_storage_type&>);
		[[rythe_always_inline]] constexpr dynamic_array(
			const allocator_storage_type& allocStorage, const factory_storage_type& factoryStorage
		)
			noexcept(is_nothrow_constructible_v<
				container_base, const allocator_storage_type&, const factory_storage_type&>);

		constexpr ~dynamic_array() override = default;

		template <size_type N>
		[[rythe_always_inline]] constexpr static dynamic_array from_array(const value_type (&arr)[N])
			noexcept(container_base::copy_construct_noexcept);
		template <size_type N>
		[[rythe_always_inline]] constexpr static dynamic_array from_array(value_type (&&arr)[N])
			noexcept(container_base::move_construct_noexcept);

		[[rythe_always_inline]] constexpr static dynamic_array from_buffer(const value_type* ptr, size_type count)
			noexcept(container_base::copy_construct_noexcept);

		[[rythe_always_inline]] constexpr static dynamic_array from_view(const_view_type src)
		noexcept(container_base::copy_construct_noexcept);

		[[rythe_always_inline]] constexpr static dynamic_array create_reserved(size_type capacity) noexcept;

		template <typename... Args>
		[[rythe_always_inline]] constexpr static dynamic_array create_in_place(size_type count, Args&&... args)
			noexcept(container_base::template construct_noexcept<Args...>);

		using container_base::operator view_type;
		using container_base::operator const_view_type;

	    using container_base::operator[];
	    using container_base::operator=;
	    using container_base::operator==;
	    using container_base::operator!=;
	};
} // namespace rsl

#include "dynamic_array.inl"

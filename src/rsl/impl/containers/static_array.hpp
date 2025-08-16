#pragma once

#include "../util/primitives.hpp"
#include "contiguous_container_base.hpp"
#include "../memory/mock_allocator.hpp"

namespace rsl
{
	template <typename T, size_type Capacity, typed_factory_type Factory = default_factory<T>, bool
			  UsePostFix = false>
	class static_array : public contiguous_container_base<T, mock_allocator, Factory, T*, const T*, contiguous_container_info<UsePostFix, Capacity, false, true>>
	{
	public:
		using container_base = contiguous_container_base<T, mock_allocator, Factory, T*, const T*, contiguous_container_info<UsePostFix, Capacity, false, true>>;
		using mem_rsc = typename container_base::mem_rsc;
		using value_type = T;
		using iterator_type = typename container_base::iterator_type;
		using const_iterator_type = typename container_base::const_iterator_type;
		using reverse_iterator_type = typename container_base::reverse_iterator_type;
		using const_reverse_iterator_type = typename container_base::const_reverse_iterator_type;
		using view_type = typename container_base::view_type;
		using const_view_type = typename container_base::const_view_type;
		using allocator_storage_type = allocator_storage<mock_allocator>;
		using allocator_t = mock_allocator;
		using factory_storage_type = factory_storage<Factory>;
		using factory_t = Factory;

		[[rythe_always_inline]] constexpr static_array()
			noexcept(is_nothrow_constructible_v<container_base>) = default;

		[[rythe_always_inline]] constexpr static_array(const static_array& src)
			noexcept(container_base::copy_construct_noexcept);
		[[rythe_always_inline]] constexpr static_array(static_array&& src) noexcept;

		[[rythe_always_inline]] explicit constexpr static_array(const factory_storage_type& factoryStorage)
			noexcept(is_nothrow_constructible_v<container_base, const factory_storage_type&>);

		constexpr ~static_array() override = default;

		template <size_type N>
		[[rythe_always_inline]] constexpr static static_array from_array(const value_type (&arr)[N])
			noexcept(container_base::copy_construct_noexcept);
		template <size_type N>
		[[rythe_always_inline]] constexpr static static_array from_array(value_type (&&arr)[N])
			noexcept(container_base::move_construct_noexcept);

		[[rythe_always_inline]] constexpr static static_array from_buffer(const value_type* ptr, size_type count)
			noexcept(container_base::copy_construct_noexcept);

		[[rythe_always_inline]] constexpr static static_array from_view(const_view_type src)
		noexcept(container_base::copy_construct_noexcept);

		template <typename... Args>
		[[rythe_always_inline]] constexpr static static_array create_in_place(size_type count, Args&&... args)
			noexcept(container_base::template construct_noexcept<Args...>);

		using container_base::operator view_type;
		using container_base::operator const_view_type;

		[[rythe_always_inline]] constexpr static_array& operator=(const static_array& src)
			noexcept(container_base::copy_assign_noexcept && container_base::copy_construct_noexcept);
		[[rythe_always_inline]] constexpr static_array& operator=(static_array&& src) noexcept;
		template <size_type N>
		[[rythe_always_inline]] constexpr static_array& operator=(const value_type (&arr)[N])
			noexcept(container_base::copy_assign_noexcept && container_base::copy_construct_noexcept);
		template <size_type N>
		[[rythe_always_inline]] constexpr static_array& operator=(value_type (&&arr)[N])
			noexcept(container_base::move_assign_noexcept && container_base::move_construct_noexcept);
		[[rythe_always_inline]] constexpr static_array& operator=(view_type src)
			noexcept(container_base::copy_assign_noexcept && container_base::copy_construct_noexcept);

		[[rythe_always_inline]] constexpr bool operator==(const static_array& rhs);
		[[rythe_always_inline]] constexpr bool operator!=(const static_array& rhs);
	};
}

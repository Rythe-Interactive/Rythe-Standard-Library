#pragma once

#include "../util/assert.hpp"
#include "../util/primitives.hpp"
#include "contiguous_container_base.hpp"

namespace rsl
{
	template <typename T, allocator_type Alloc = default_allocator, untyped_factory_type Factory = default_factory<T>>
	class dynamic_array : public contiguous_container_base<T, Alloc, Factory, T*, const T*>
	{
	public:
		using container_base = contiguous_container_base<T, Alloc, Factory, T*, const T*>;
		using mem_rsc = container_base::mem_rsc;
		using value_type = T;
		using iterator_type = container_base::iterator_type;
		using const_iterator_type = container_base::const_iterator_type;
		using reverse_iterator_type = container_base::reverse_iterator_type;
		using const_reverse_iterator_type = container_base::const_reverse_iterator_type;
		using view_type = container_base::view_type;
		using const_view_type = container_base::const_view_type;

		[[rythe_always_inline]] constexpr dynamic_array() noexcept = default;

		[[rythe_always_inline]] constexpr dynamic_array(const dynamic_array& src)
			noexcept(container_base::copy_construct_noexcept)
		{
			mem_rsc::allocate(src.m_size);
			container_base::m_size = src.m_size;
			container_base::copy_construct_from_unsafe_impl(0, container_base::m_size, src.get_ptr());
		}

	};
} // namespace rsl

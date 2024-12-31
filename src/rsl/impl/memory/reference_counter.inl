#pragma once
#include "reference_counter.hpp"

namespace rsl
{
	inline constexpr size_type manual_reference_counter::borrow() noexcept
	{
		return m_count++;
	}

	inline constexpr void manual_reference_counter::release() noexcept
	{
		rsl_assert_borrow_release_mismatch(occupied());
		m_count--;
	}

	inline constexpr size_type manual_reference_counter::count() const noexcept
	{
		return m_count;
	}

	inline constexpr bool manual_reference_counter::occupied() const noexcept
	{
		return m_count != 0;
	}

	inline constexpr bool manual_reference_counter::free() const noexcept
	{
		return !occupied();
	}

	template <reference_counted Counter, allocator_type Alloc, factory_type Factory>
	inline constexpr basic_reference_counter<Counter, Alloc, Factory>::basic_reference_counter(arm_signal_type) noexcept
	{
		arm();
	}

	template <reference_counted Counter, allocator_type Alloc, factory_type Factory>
	inline constexpr basic_reference_counter<Counter, Alloc, Factory>::basic_reference_counter(
		const basic_reference_counter& other
	) noexcept
		: mem_rsc(other)
	{
		if (is_armed())
		{
			borrow();
		}
	}

	template <reference_counted Counter, allocator_type Alloc, factory_type Factory>
	inline constexpr basic_reference_counter<Counter, Alloc, Factory>::basic_reference_counter(
		basic_reference_counter&& other
	) noexcept
		: mem_rsc(move(other))
	{
		other.set_ptr(nullptr);
	}

	template <reference_counted Counter, allocator_type Alloc, factory_type Factory>
	inline constexpr basic_reference_counter<Counter, Alloc, Factory>&
	basic_reference_counter<Counter, Alloc, Factory>::operator=(const basic_reference_counter& other) noexcept
	{
		if (is_armed())
		{
			disarm();
		}

		mem_rsc::operator=(other);

		if (is_armed())
		{
			borrow();
		}

		return *this;
	}

	template <reference_counted Counter, allocator_type Alloc, factory_type Factory>
	inline constexpr basic_reference_counter<Counter, Alloc, Factory>&
	basic_reference_counter<Counter, Alloc, Factory>::operator=(basic_reference_counter&& other) noexcept
	{
		mem_rsc::operator=(move(other));
		other.set_ptr(nullptr);
		return *this;
	}

	template <reference_counted Counter, allocator_type Alloc, factory_type Factory>
	inline constexpr basic_reference_counter<Counter, Alloc, Factory>::~basic_reference_counter() noexcept
	{
		disarm();
	}

	template <reference_counted Counter, allocator_type Alloc, factory_type Factory>
	inline basic_reference_counter<Counter, Alloc, Factory>::basic_reference_counter(
		const allocator_storage_type& allocStorage
	) noexcept(is_nothrow_constructible_v<mem_rsc, const allocator_storage_type&>)
		: mem_rsc(allocStorage)
	{
	}

	template <reference_counted Counter, allocator_type Alloc, factory_type Factory>
	inline basic_reference_counter<Counter, Alloc, Factory>::basic_reference_counter(
		const factory_storage_type& factoryStorage
	) noexcept(is_nothrow_constructible_v<mem_rsc, const factory_storage_type&>)
		: mem_rsc(factoryStorage)
	{
	}

	template <reference_counted Counter, allocator_type Alloc, factory_type Factory>
	inline basic_reference_counter<Counter, Alloc, Factory>::basic_reference_counter(
		const allocator_storage_type& allocStorage, const factory_storage_type& factoryStorage
	) noexcept(is_nothrow_constructible_v<mem_rsc, const allocator_storage_type&, const factory_storage_type&>)
		: mem_rsc(allocStorage, factoryStorage)
	{
	}

	template <reference_counted Counter, allocator_type Alloc, factory_type Factory>
	inline constexpr void basic_reference_counter<Counter, Alloc, Factory>::arm() noexcept
	{
		arm(bit_cast<Counter*>(mem_rsc::m_alloc.allocate_and_construct()));
	}

	template <reference_counted Counter, allocator_type Alloc, factory_type Factory>
	inline constexpr void basic_reference_counter<Counter, Alloc, Factory>::disarm() noexcept
	{
		if (!is_armed())
		{
			return;
		}

		if (free())
		{
			mem_rsc::destroy_and_deallocate();
		}
		else
		{
			release();
			mem_rsc::set_ptr(nullptr);
		}
	}

	template <reference_counted Counter, allocator_type Alloc, factory_type Factory>
	inline constexpr bool basic_reference_counter<Counter, Alloc, Factory>::is_armed() const noexcept
	{
		return mem_rsc::get_ptr();
	}

	template <reference_counted Counter, allocator_type Alloc, factory_type Factory>
	inline constexpr size_type basic_reference_counter<Counter, Alloc, Factory>::borrow() noexcept
	{
		rsl_assert_invalid_object(is_armed());
		return mem_rsc::get_ptr()->borrow();
	}

	template <reference_counted Counter, allocator_type Alloc, factory_type Factory>
	inline constexpr void basic_reference_counter<Counter, Alloc, Factory>::release() noexcept
	{
		rsl_assert_invalid_object(is_armed());
		rsl_assert_borrow_release_mismatch(occupied());
		mem_rsc::get_ptr()->release();
	}

	template <reference_counted Counter, allocator_type Alloc, factory_type Factory>
	inline constexpr size_type basic_reference_counter<Counter, Alloc, Factory>::count() const noexcept
	{
		return is_armed() ? mem_rsc::get_ptr()->count() : 0ull;
	}

	template <reference_counted Counter, allocator_type Alloc, factory_type Factory>
	inline constexpr bool basic_reference_counter<Counter, Alloc, Factory>::occupied() const noexcept
	{
		// Don't count ourselves, we will release the last reference upon destruction.
		return is_armed() && mem_rsc::get_ptr()->count() > 1;
	}

	template <reference_counted Counter, allocator_type Alloc, factory_type Factory>
	inline constexpr bool basic_reference_counter<Counter, Alloc, Factory>::free() const noexcept
	{
		return !occupied();
	}

	template <reference_counted Counter, allocator_type Alloc, factory_type Factory>
	inline constexpr void basic_reference_counter<Counter, Alloc, Factory>::arm(Counter* ptr) noexcept
	{
		rsl_assert_duplicate_object(!is_armed());
		mem_rsc::set_ptr(ptr);
		borrow();
	}
} // namespace rsl

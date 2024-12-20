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
		other.m_ptr = nullptr;
	}

	template <reference_counted Counter, allocator_type Alloc, factory_type Factory>
	inline constexpr basic_reference_counter<Counter, Alloc, Factory>&
	basic_reference_counter<Counter, Alloc, Factory>::operator=(const basic_reference_counter& other) noexcept
	{
		this->m_ptr = other.m_ptr;
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
		this->m_ptr = other.m_ptr;
		other.m_ptr = nullptr;
		return *this;
	}

	template <reference_counted Counter, allocator_type Alloc, factory_type Factory>
	inline constexpr basic_reference_counter<Counter, Alloc, Factory>::~basic_reference_counter() noexcept
	{
		disarm();
	}

	template <reference_counted Counter, allocator_type Alloc, factory_type Factory>
	inline constexpr void basic_reference_counter<Counter, Alloc, Factory>::arm() noexcept
	{
		arm(this->m_alloc.allocate());
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
			this->deallocate();
		}
		else
		{
			release();
		}
	}

	template <reference_counted Counter, allocator_type Alloc, factory_type Factory>
	inline constexpr bool basic_reference_counter<Counter, Alloc, Factory>::is_armed() const noexcept
	{
		return this->m_ptr;
	}

	template <reference_counted Counter, allocator_type Alloc, factory_type Factory>
	inline constexpr size_type basic_reference_counter<Counter, Alloc, Factory>::borrow() noexcept
	{
		rsl_assert_invalid_object(is_armed());
		return this->m_ptr->borrow();
	}

	template <reference_counted Counter, allocator_type Alloc, factory_type Factory>
	inline constexpr void basic_reference_counter<Counter, Alloc, Factory>::release() noexcept
	{
		rsl_assert_invalid_object(is_armed());
		rsl_assert_borrow_release_mismatch(occupied());
		this->m_ptr->release();
	}

	template <reference_counted Counter, allocator_type Alloc, factory_type Factory>
	inline constexpr size_type basic_reference_counter<Counter, Alloc, Factory>::count() const noexcept
	{
		rsl_assert_invalid_object(is_armed());
		return this->m_ptr->count();
	}

	template <reference_counted Counter, allocator_type Alloc, factory_type Factory>
	inline constexpr bool basic_reference_counter<Counter, Alloc, Factory>::occupied() const noexcept
	{
		return is_armed() &&
			   this->m_ptr->count() > 1; // Don't count ourselves, we will release the last reference upon destruction.
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
		this->m_ptr = ptr;
		borrow();
	}
} // namespace rsl

#pragma once

namespace rsl
{
	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix, size_type StaticCapacity>
	constexpr dynamic_array<T, Alloc, Factory, UsePostFix, StaticCapacity>::dynamic_array(const dynamic_array& src)
		noexcept(container_base::copy_construct_noexcept)
		: container_base(src)
	{
	    // TODO(Glyn): implement in container_base instead
		// size_type newMemorySize;
		// if constexpr (UsePostFix)
		// {
		// 	newMemorySize = src.m_size + 1ull;
		// }
		// else
		// {
		// 	newMemorySize = src.m_size;
		// }
		//
		// mem_rsc::allocate(newMemorySize);
		// container_base::m_size = src.m_size;
		// container_base::m_capacity = src.m_size;
		// container_base::copy_construct_from_unsafe_impl(0ull, newMemorySize, src.get_ptr());
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix, size_type StaticCapacity>
	constexpr dynamic_array<T, Alloc, Factory, UsePostFix, StaticCapacity>::dynamic_array(dynamic_array&& src) noexcept
		: container_base(rsl::move(src))
	{
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix, size_type StaticCapacity>
	constexpr dynamic_array<T, Alloc, Factory, UsePostFix, StaticCapacity>::dynamic_array(const allocator_storage_type& allocStorage)
		noexcept(is_nothrow_constructible_v<container_base, const allocator_storage_type&>)
		: container_base(allocStorage) {}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix, size_type StaticCapacity>
	constexpr dynamic_array<T, Alloc, Factory, UsePostFix, StaticCapacity>::dynamic_array(const factory_storage_type& factoryStorage)
		noexcept(is_nothrow_constructible_v<container_base, const factory_storage_type&>)
		: container_base(factoryStorage) {}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix, size_type StaticCapacity>
	constexpr dynamic_array<T, Alloc, Factory, UsePostFix, StaticCapacity>::dynamic_array(
		const allocator_storage_type& allocStorage, const factory_storage_type& factoryStorage
	) noexcept(is_nothrow_constructible_v<container_base, const allocator_storage_type&, const factory_storage_type&>)
		: container_base(allocStorage, factoryStorage) {}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix, size_type StaticCapacity>
	template <size_type N>
	constexpr dynamic_array<T, Alloc, Factory, UsePostFix, StaticCapacity> dynamic_array<T, Alloc, Factory, UsePostFix, StaticCapacity>::from_array(
		const value_type (&arr)[N])
		noexcept(container_base::copy_construct_noexcept)
	{
		dynamic_array result;
		result.reserve(N);
		result.m_size = N;
		result.copy_construct_from_unsafe_impl(0ull, result.m_size, &arr[0]);

		if constexpr (UsePostFix)
		{
			result.construct(1ull, result.m_size);
		}

		return result;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix, size_type StaticCapacity>
	template <size_type N>
	constexpr dynamic_array<T, Alloc, Factory, UsePostFix, StaticCapacity> dynamic_array<T, Alloc, Factory, UsePostFix, StaticCapacity>::from_array(
		value_type (&&arr)[N])
		noexcept(container_base::move_construct_noexcept)
	{
		dynamic_array result;
		result.reserve(N);
		result.m_size = N;
		result.move_construct_from_unsafe_impl(0ull, result.m_size, &arr[0]);

		if constexpr (UsePostFix)
		{
			result.construct(1ull, result.m_size);
		}

		return result;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix, size_type StaticCapacity>
	constexpr dynamic_array<T, Alloc, Factory, UsePostFix, StaticCapacity> dynamic_array<T, Alloc, Factory, UsePostFix, StaticCapacity>::from_buffer(
		const value_type* ptr,
		size_type count) noexcept(container_base::copy_construct_noexcept)
	{
		dynamic_array result;
		result.reserve(count);
		result.m_size = count;
		result.copy_construct_from_unsafe_impl(0ull, result.m_size, ptr);

		if constexpr (UsePostFix)
		{
			result.construct(1ull, result.m_size);
		}

		return result;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix, size_type StaticCapacity>
	constexpr dynamic_array<T, Alloc, Factory, UsePostFix, StaticCapacity> dynamic_array<T, Alloc, Factory, UsePostFix, StaticCapacity>::from_view(
		const_view_type src) noexcept(container_base::copy_construct_noexcept)
	{
		dynamic_array result;
		result.reserve(src.size());
		result.m_size = src.size();
		result.copy_construct_from_unsafe_impl(0ull, result.m_size, src.data());

		if constexpr (UsePostFix)
		{
			result.construct(1ull, result.m_size);
		}

		return result;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix, size_type StaticCapacity>
	constexpr dynamic_array<T, Alloc, Factory, UsePostFix, StaticCapacity>
		dynamic_array<T, Alloc, Factory, UsePostFix, StaticCapacity>::create_reserved(const size_type capacity) noexcept
	{
		dynamic_array result;
		result.reserve(capacity);

		return result;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix, size_type StaticCapacity>
	template <typename... Args>
	constexpr dynamic_array<T, Alloc, Factory, UsePostFix, StaticCapacity> dynamic_array<T, Alloc, Factory, UsePostFix, StaticCapacity>::create_in_place(
		size_type count, Args&&... args)
		noexcept(container_base::template construct_noexcept<Args...>)
	{
		dynamic_array result;
		result.reserve(count);
		result.m_size = count;
		result.construct(count, 0ull, rsl::forward<Args>(args)...);

		if constexpr (UsePostFix)
		{
			result.construct(1ull, result.m_size);
		}

		return result;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix, size_type StaticCapacity>
	constexpr dynamic_array<T, Alloc, Factory, UsePostFix, StaticCapacity>&
	dynamic_array<T, Alloc, Factory, UsePostFix, StaticCapacity>::operator=(const dynamic_array& src)
		noexcept(container_base::copy_assign_noexcept && container_base::copy_construct_noexcept)
	{
	    // TODO(Glyn): move to container_base
		copy_assign_impl(src.get_ptr(), src.m_size, mem_rsc::m_alloc == src.m_alloc ? nullptr : &src.m_alloc);
		return *this;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix, size_type StaticCapacity>
	constexpr dynamic_array<T, Alloc, Factory, UsePostFix, StaticCapacity>& dynamic_array<T, Alloc, Factory, UsePostFix, StaticCapacity>::operator=(
		dynamic_array&& src
	) noexcept
	{
	    // TODO(Glyn): move to container_base
		container_base::operator=(move(src));
		src.set_ptr(nullptr);
		src.m_size = 0ull;
		src.m_capacity = 0ull;
		return *this;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix, size_type StaticCapacity>
	template <size_type N>
	constexpr dynamic_array<T, Alloc, Factory, UsePostFix, StaticCapacity>&
	dynamic_array<T, Alloc, Factory, UsePostFix, StaticCapacity>::operator=(const value_type (&arr)[N])
		noexcept(container_base::copy_assign_noexcept && container_base::copy_construct_noexcept)
	{
		copy_assign_impl(arr, N);
		return *this;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix, size_type StaticCapacity>
	template <size_type N>
	constexpr dynamic_array<T, Alloc, Factory, UsePostFix, StaticCapacity>& dynamic_array<T, Alloc, Factory, UsePostFix, StaticCapacity>::operator=(
		value_type (&&arr)[N]
	) noexcept(container_base::move_assign_noexcept && container_base::move_construct_noexcept)
	{
		move_data_assign_impl(arr, N);
		return *this;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix, size_type StaticCapacity>
	constexpr dynamic_array<T, Alloc, Factory, UsePostFix, StaticCapacity>& dynamic_array<T, Alloc, Factory, UsePostFix, StaticCapacity>::operator=(
		view_type src)
		noexcept(container_base::copy_assign_noexcept && container_base::copy_construct_noexcept)
	{
		copy_assign_impl(src.data(), src.size());
		return *this;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix, size_type StaticCapacity>
	inline constexpr bool dynamic_array<T, Alloc, Factory, UsePostFix, StaticCapacity>::operator==(
		const dynamic_array<T, Alloc, Factory, UsePostFix, StaticCapacity>& rhs)
	{
		bool result = true;
		for (size_type i = 0; i < this->size(); ++i)
		{
			result &= this->at(i) == rhs.at(i);
		}

		return result;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory, bool UsePostFix, size_type StaticCapacity>
	inline constexpr bool dynamic_array<T, Alloc, Factory, UsePostFix, StaticCapacity>::operator!=(
		const dynamic_array<T, Alloc, Factory, UsePostFix, StaticCapacity>& rhs)
	{
		return !(*this == rhs);
	}
} // namespace rsl

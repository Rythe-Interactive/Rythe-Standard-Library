#pragma once

#include "../util/assert.hpp"
#include "../util/common.hpp"
#include "../util/primitives.hpp"


namespace rsl
{
	template <allocator_type Alloc = default_allocator>
	struct string final : public dynamic_array<char, Alloc>
	{
	public:
		constexpr static size_type npos = static_cast<size_type>(-1);
		using base = dynamic_array<char, Alloc>;
		using value_type = typename base::value_type;
		using view_type = typename base::view_type;

		[[rythe_always_inline]] string() noexcept(is_nothrow_constructible_v<base>);
		[[rythe_always_inline]] string(const string& src) noexcept(base::container_base::copy_construct_noexcept);
		[[rythe_always_inline]] string(string&& src) noexcept;
		template<size_type N>
		[[rythe_always_inline]] string(const value_type (&arr)[N]) noexcept(base::container_base::move_construct_noexcept);
		template<size_type N>
		[[rythe_always_inline]] string(value_type (&&arr)[N]) noexcept(base::container_base::copy_construct_noexcept);
		[[rythe_always_inline]] string(view_type src) noexcept(base::container_base::copy_construct_noexcept);
		[[rythe_always_inline]] string(size_type capacity) noexcept;

	public:
		//assign
		template<size_type N>
		[[rythe_always_inline]] void assign(const value_type (&src)[N]) noexcept(base::container_base::copy_assign_noexcept);
		template<size_type N>
		[[rythe_always_inline]] void assign(value_type (&&src)[N]) noexcept(base::container_base::move_assign_noexcept);

		//append
		[[rythe_always_inline]] string& append(view_type);

		// operator+=
		[[rythe_always_inline]] string& operator+=(view_type);
		//replace
		[[rythe_always_inline]] string& replace(size_type, size_type, view_type);
	};

} // namespace rsl

#include "string.inl"

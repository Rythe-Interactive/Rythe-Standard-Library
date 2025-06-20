#pragma once

#include "../util/assert.hpp"
#include "../util/common.hpp"
#include "../util/primitives.hpp"

namespace rsl
{
	// TODO(Rowan): constexpr strings are still useful if the string never leaks from constant evaluation time to runtime. constexpr_string is needed if you want to be able to read the results of a string at runtime.
	template <typename CharType = char, allocator_type Alloc = default_allocator>
	class basic_dynamic_string final : public dynamic_array<CharType, Alloc>
	{
	public:
		constexpr static size_type npos = static_cast<size_type>(-1);
		using base = dynamic_array<CharType, Alloc>;
		using container_base = typename base::container_base;
		using value_type = typename base::value_type;
		using view_type = typename base::view_type;

		using dynamic_array<CharType, Alloc>::dynamic_array;
		using dynamic_array<CharType, Alloc>::operator=;

		template <size_type N>
		[[rythe_always_inline]] constexpr static basic_dynamic_string from_array(const value_type (&arr)[N])
			noexcept(container_base::copy_construct_noexcept);
		template <size_type N>
		[[rythe_always_inline]] constexpr static basic_dynamic_string from_array(value_type (&&arr)[N])
			noexcept(container_base::move_construct_noexcept);

		[[rythe_always_inline]] constexpr static basic_dynamic_string from_buffer(const value_type* ptr, size_type count)
			noexcept(container_base::copy_construct_noexcept);

		[[rythe_always_inline]] constexpr static basic_dynamic_string from_view(view_type src)
			noexcept(container_base::copy_construct_noexcept);

		[[rythe_always_inline]] constexpr static basic_dynamic_string create_reserved(size_type capacity) noexcept;

		template <typename... Args>
		[[rythe_always_inline]] constexpr static basic_dynamic_string create_in_place(size_type count, CharType val = CharType(0))
			noexcept(container_base::copy_construct_noexcept);

		// append
		[[rythe_always_inline]] basic_dynamic_string& append(view_type); // TODO(Rowan): move to dynamic_array

		// operator+=
		[[rythe_always_inline]] basic_dynamic_string& operator+=(view_type);

		// replace
		[[rythe_always_inline]] basic_dynamic_string& replace(size_type, size_type, view_type);
	};

	using dynamic_string = basic_dynamic_string<>;

} // namespace rsl

#include "string.inl"

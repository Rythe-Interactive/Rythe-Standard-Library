#pragma once

#include "../util/assert.hpp"
#include "../util/common.hpp"
#include "../util/primitives.hpp"
#include "../util/error_handling.hpp"

#include "../memory/allocator.hpp"

namespace rsl
{
	// TODO: should probably automatically make sure the last value is always `\0`
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
		using const_view_type = typename base::const_view_type;

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

		[[rythe_always_inline]] constexpr static basic_dynamic_string from_view(const_view_type src)
			noexcept(container_base::copy_construct_noexcept);

		[[rythe_always_inline]] constexpr static basic_dynamic_string create_reserved(size_type capacity) noexcept;

		template <typename... Args>
		[[rythe_always_inline]] constexpr static basic_dynamic_string create_in_place(size_type count, CharType val = CharType(0))
			noexcept(container_base::copy_construct_noexcept);

		// append
		[[rythe_always_inline]] basic_dynamic_string& append(const_view_type); // TODO(Rowan): move to dynamic_array

		// operator+=
		[[rythe_always_inline]] basic_dynamic_string& operator+=(const_view_type);

		// replace
		[[rythe_always_inline]] basic_dynamic_string& replace(size_type, size_type, const_view_type);
	};

	using dynamic_string = basic_dynamic_string<>;

	using dynamic_wstring = basic_dynamic_string<utf16>;

	dynamic_wstring to_utf16(dynamic_string::const_view_type str);

	template<typename T>
	[[nodiscard]] [[rythe_always_inline]] result<dynamic_string> to_string(const T& value) noexcept;

} // namespace rsl

#include "string.inl"

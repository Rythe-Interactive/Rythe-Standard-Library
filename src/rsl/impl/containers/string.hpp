#pragma once
#include "dynamic_array.hpp"

namespace rsl
{
	// TODO: should probably automatically make sure the last value is always `\0`
	// TODO(Rowan): constexpr strings are still useful if the string never leaks from constant evaluation time to runtime. constexpr_string is needed if you want to be able to read the results of a string at runtime.
	template <typename CharType = char, allocator_type Alloc = default_allocator>
	class basic_dynamic_string final : public dynamic_array<CharType, Alloc, default_factory<CharType>, true>
	{
	public:
		constexpr static size_type npos = static_cast<size_type>(-1);
		using base = dynamic_array<CharType, Alloc, default_factory<CharType>, true>;
		using container_base = typename base::container_base;
		using value_type = typename base::value_type;
		using view_type = typename base::view_type;
		using const_view_type = typename base::const_view_type;

		using dynamic_array<CharType, Alloc, default_factory<CharType>, true>::dynamic_array;
		using dynamic_array<CharType, Alloc, default_factory<CharType>, true>::operator=;

		[[rythe_always_inline]] constexpr basic_dynamic_string(const base& src)
			noexcept(container_base::copy_construct_noexcept);

		[[rythe_always_inline]] constexpr basic_dynamic_string(base&& src) noexcept;

		// operator+=
		[[rythe_always_inline]] constexpr basic_dynamic_string& operator+=(const_view_type rhs);
	};

	using dynamic_string = basic_dynamic_string<>;

	using dynamic_wstring = basic_dynamic_string<utf16>;

	dynamic_wstring to_utf16(dynamic_string::const_view_type str);

	template<typename T>
	[[nodiscard]] [[rythe_always_inline]] dynamic_string to_string(const T& value);

} // namespace rsl

#include "string.inl"

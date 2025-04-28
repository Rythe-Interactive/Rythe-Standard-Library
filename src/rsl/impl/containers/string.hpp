#pragma once

#include "../util/assert.hpp"
#include "../util/common.hpp"
#include "../util/primitives.hpp"

namespace rsl
{
	// TODO(Rowan): constexpr strings are still useful if the string never leaks from constant evaluation time to runtime. constexpr_string is needed if you want to be able to read the results of a string at runtime.
	template <allocator_type Alloc = default_allocator>
	struct string final : public dynamic_array<char, Alloc>
	{
	public:
		constexpr static size_type npos = static_cast<size_type>(-1);
		using base = dynamic_array<char, Alloc>;
		using value_type = typename base::value_type;
		using view_type = typename base::view_type;

		using dynamic_array<char, Alloc>::dynamic_array;

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

	// TODO(Rowan): This rly doesn't work. ig that's why std::basic_string exists. we could also do: using dynamic_string = string<default_allocator>; and use dynamic_string for everything instead of string<>.
	template<typename T>
	string(T) -> string<>;

} // namespace rsl

#include "string.inl"

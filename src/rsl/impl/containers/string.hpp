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

		[[rythe_always_inline]] string() noexcept(is_nothrow_constructible_v<base>) : base() {}
		[[rythe_always_inline]] string(const string& src) noexcept(base::container_base::copy_construct_noexcept) : base(src) {}
		[[rythe_always_inline]] string(string&& src) noexcept : base(src) {}
		template<size_type N>
		[[rythe_always_inline]] string(const value_type (&arr)[N]) noexcept(base::container_base::move_construct_noexcept) : base(arr) {}
		template<size_type N>
		[[rythe_always_inline]] string(value_type (&&arr)[N]) noexcept(base::container_base::copy_construct_noexcept): base(rsl::move(arr)) {}
		[[rythe_always_inline]] string(view_type src) noexcept(base::container_base::copy_construct_noexcept): base(src) {}
		[[rythe_always_inline]] string(size_type capacity) noexcept : base(capacity) {}

	public:
		//append
		[[rythe_always_inline]] string& append(view_type);

		// operator+=
		[[rythe_always_inline]] string& operator+=(view_type rhs);
		//replace
		[[rythe_always_inline]] string& replace(size_type, size_type, view_type);
	};

} // namespace rsl

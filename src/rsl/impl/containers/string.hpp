#pragma once
#include <string>
#include <string_view>

#include "../util/assert.hpp"
#include "../util/common.hpp"
#include "../util/primitives.hpp"

#include "contiguous_container_base.hpp"


namespace rsl
{
	template <allocator_type Alloc = default_allocator>
	struct string final : public contiguous_container_base<char, Alloc>
	{
	public:
		constexpr static size_type npos = size_type(-1);
		using container_base = contiguous_container_base<char, Alloc>;
		using mem_rsc = container_base::mem_rsc;
		using value_type = typename container_base::value_type;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const pointer;
		using iterator = typename container_base::iterator_type;
		using const_iterator = typename container_base::const_iterator_type;
		using reverse_iterator = typename container_base::reverse_iterator;
		using const_reverse_iterator = typename container_base::const_reverse_iterator_type;

	public:
		[[rythe_always_inline]] string& operator=(const string& rhs);
		[[rythe_always_inline]] string& operator=(string&& rhs);
		[[rythe_always_inline]] string& operator=(const_pointer rhs);
		[[rythe_always_inline]] string& operator=(value_type rhs);
		// [[rythe_always_inline]] string& operator=(rsl::initializer_list<char> ilist);
		//template<string_view_like StrView>
		// [[rythe_always_inline]] string& operator=(const StrView& t);
		[[rythe_always_inline]] string& operator=(rsl::nullptr_type) = delete;


		[[rythe_always_inline]] char* c_str() noexcept;

		[[rythe_always_inline]] string& operator+=(const string& rhs);
		[[rythe_always_inline]] string& operator+=(string&& rhs);
		[[rythe_always_inline]] string& operator+=(const_pointer rhs);
		[[rythe_always_inline]] string& operator+=(value_type rhs);


		[[rythe_always_inline]] string& operator-=(const value_type& rhs);
		[[rythe_always_inline]] string& operator-=(value_type&& rhs);
		[[rythe_always_inline]] string& operator-=(const string& rhs);
		[[rythe_always_inline]] string& operator-=(string&& rhs);

		[[rythe_always_inline]] void resize(size_type new_size);

		[[rythe_always_inline]] void reserve(size_type new_capacity);

		[[rythe_always_inline]] void clear();

		[[rythe_always_inline]] string& insert(iterator pos, value_type&& value);
		[[rythe_always_inline]] string& insert(const_iterator pos, const value_type& value);
		[[rythe_always_inline]] string& insert(size_type pos, value_type&& value);
		[[rythe_always_inline]] string& insert(size_type pos, const value_type& value);
		[[rythe_always_inline]] string& insert(size_type pos, const string& str);
		[[rythe_always_inline]] string& insert(size_type pos, string&& str);

		// [[rythe_always_inline]] void insert_range(size_type pos, const value_type& value);
		// [[rythe_always_inline]] void insert_range(size_type pos, value_type&& value);

		[[rythe_always_inline]] size_type erase(iterator pos, size_type count);
		[[rythe_always_inline]] size_type erase(const_iterator pos, size_type count);
		[[rythe_always_inline]] size_type erase(size_type pos) noexcept;
		[[rythe_always_inline]] size_type erase(size_type first, size_type last);
		template <typename Comp>
		[[rythe_always_inline]] constexpr size_type erase(Comp&& comparable) noexcept(container_base::move_construct_noexcept);
		template <typename Func>
		[[rythe_always_inline]] constexpr size_type erase(Func&& comparer) noexcept(container_base::move_construct_noexcept) requires invocable<Func, bool(const_iterator)>;

		[[rythe_always_inline]] string& push_back(const value_type& value);
		[[rythe_always_inline]] string& push_back(value_type&& value);

		[[rythe_always_inline]] string& pop_back();

		[[rythe_always_inline]] string& append(size_type count, value_type value);
		[[rythe_always_inline]] string& append(const_pointer s, size_type count = 1);
		[[rythe_always_inline]] string& append(const string& s, size_type count = 1);
		//template<string_view_like StrView>
		// [[rythe_always_inline]] string& append(const StrView& t);
		[[rythe_always_inline]] string& append(string&& s, size_type pos = npos, size_type count = npos);
		[[rythe_always_inline]] string& append(iterator start, iterator stop);
		[[rythe_always_inline]] string& append(const_iterator start, const_iterator stop);
		// [[rythe_always_inline]] string& append_range(const value_type& value);
		// [[rythe_always_inline]] string& append_range(value_type&& value);

		[[rythe_always_inline]] string& replace(size_type pos, size_type count, const value_type& value);
		[[rythe_always_inline]] string& replace(size_type pos, size_type count, value_type&& value);
		[[rythe_always_inline]] string& replace(size_type pos, size_type count, const string& value);
		[[rythe_always_inline]] string& replace(size_type pos, size_type count, string&& value);

		[[rythe_always_inline]] string& replace(size_type pos, size_type count, size_type pos2, size_type count2,const value_type& value);
		[[rythe_always_inline]] string& replace(size_type pos, size_type count,  size_type pos2, size_type count2,value_type&& value);
		[[rythe_always_inline]] string& replace(size_type pos, size_type count,  size_type pos2, size_type count2,const string& value);
		[[rythe_always_inline]] string& replace(size_type pos, size_type count, size_type pos2, size_type count2, string&& value);

		[[rythe_always_inline]] string& replace(iterator first, iterator last, const value_type& value);
		[[rythe_always_inline]] string& replace(iterator first, iterator last,  value_type&& value);
		[[rythe_always_inline]] string& replace(iterator first, iterator last,  const string& value);
		[[rythe_always_inline]] string& replace(iterator first, iterator last, string&& value);

		[[rythe_always_inline]] string& replace(const_iterator first, const_iterator last, const value_type& value);
		[[rythe_always_inline]] string& replace(const_iterator first, const_iterator last,  value_type&& value);
		[[rythe_always_inline]] string& replace(const_iterator first, const_iterator last,  const string& value);
		[[rythe_always_inline]] string& replace(const_iterator first, const_iterator last, string&& value);


	};
} // namespace rsl

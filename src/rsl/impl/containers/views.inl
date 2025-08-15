#pragma once

#include "views.hpp"

namespace rsl
{
	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr view<T, Iter, ConstIter>::view(const pointer ptr, const size_type count) noexcept
		: m_src(ptr), m_count(count) {}

	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	template <contiguous_iterator It>
	constexpr view<T, Iter, ConstIter>::view(It first, It last)
		noexcept(iter_noexcept_deref<It> && iter_noexcept_diff<It>) requires same_as<iter_pointer_t<It>, pointer>
		: m_src(&(*first)), m_count(last - first) {}

	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	template <size_type N>
	constexpr view<T, Iter, ConstIter>::view(value_type (&arr)[N]) noexcept : m_src(arr), m_count(N)
	{
		if constexpr (char_type<value_type>)
		{
			if (m_src[m_count - 1] == static_cast<value_type>('\0'))
			{
				--m_count;
			}
		}
	}

	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr view<T, Iter, ConstIter>::view(const view& other) noexcept : m_src(other.m_src), m_count(other.m_count) {}

	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr view<T, Iter, ConstIter>::view(value_type& src) noexcept : m_src(&src), m_count(1) {}

	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr view<T, Iter, ConstIter>::operator view<const T, const_iterator_type>() noexcept requires (!is_const_v<T>)
	{
		return view<const T, const_iterator_type>(m_src, m_count);
	}

	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr view<T, Iter, ConstIter> view<T, Iter, ConstIter>::from_string_length(T* str, const T terminator) noexcept
		requires char_type<T>
	{
		return view(str, string_length(str, terminator));
	}

	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr bool view<T, Iter, ConstIter>::operator==(const view& rhs)
	{
		for (size_type i = 0; i < this->size(); ++i)
		{
			if (this->at(i) != rhs.at(i))
			{
				return false;
			}
		}

		return true;
	}

	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr bool view<T, Iter, ConstIter>::operator!=(const view& rhs)
	{
		return !(*this == rhs);
	}

	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	template <size_type N>
	constexpr bool view<T, Iter, ConstIter>::operator==(const value_type (&rhs)[N])
	{
		bool result = true;
		for (size_type i = 0; i < this->size(); ++i)
		{
			result &= this->at(i) == rhs[i];
		}

		return result;
	}

	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	template <size_type N>
	constexpr bool view<T, Iter, ConstIter>::operator!=(const value_type (&rhs)[N])
	{
		return !(*this == rhs);
	}

	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename view<T, Iter, ConstIter>::iterator_type view<T, Iter, ConstIter>::begin() noexcept
	{
		return iterator_type(m_src);
	}

	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename view<T, Iter, ConstIter>::const_iterator_type view<T, Iter, ConstIter>::begin() const noexcept
	{
		return cbegin();
	}

	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename view<T, Iter, ConstIter>::const_iterator_type view<T, Iter, ConstIter>::cbegin() const noexcept
	{
		return const_iterator_type(m_src);
	}

	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename view<T, Iter, ConstIter>::iterator_type view<T, Iter, ConstIter>::end() noexcept
	{
		return iterator_type(m_src + m_count);
	}

	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename view<T, Iter, ConstIter>::const_iterator_type view<T, Iter, ConstIter>::end() const noexcept
	{
		return cend();
	}

	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename view<T, Iter, ConstIter>::const_iterator_type view<T, Iter, ConstIter>::cend() const noexcept
	{
		return const_iterator_type(m_src + m_count);
	}

	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename view<T, Iter, ConstIter>::reverse_iterator_type view<T, Iter, ConstIter>::rbegin() noexcept
	{
		return reverse_iterator(end());
	}

	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename view<T, Iter, ConstIter>::const_reverse_iterator_type view<T, Iter, ConstIter>::rbegin()
	const noexcept
	{
		return crbegin();
	}

	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename view<T, Iter, ConstIter>::const_reverse_iterator_type view<T, Iter, ConstIter>::crbegin()
	const noexcept
	{
		return const_reverse_iterator_type(cend());
	}

	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename view<T, Iter, ConstIter>::reverse_iterator_type view<T, Iter, ConstIter>::rend() noexcept
	{
		return reverse_iterator(begin());
	}

	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename view<T, Iter, ConstIter>::const_reverse_iterator_type view<T, Iter, ConstIter>::rend()
	const noexcept
	{
		return crend();
	}

	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename view<T, Iter, ConstIter>::const_reverse_iterator_type view<T, Iter, ConstIter>::crend()
	const noexcept
	{
		return const_reverse_iterator_type(cbegin());
	}

	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename view<T, Iter, ConstIter>::reference view<T, Iter, ConstIter>::front()
	{
		return at(0);
	}

	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename view<T, Iter, ConstIter>::reference view<T, Iter, ConstIter>::front() const
	{
		return at(0);
	}

	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename view<T, Iter, ConstIter>::reference view<T, Iter, ConstIter>::back()
	{
		return at(m_count - 1);
	}

	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename view<T, Iter, ConstIter>::reference view<T, Iter, ConstIter>::back() const
	{
		return at(m_count - 1);
	}

	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename view<T, Iter, ConstIter>::reference view<T, Iter, ConstIter>::at(size_type pos)
	{
		return *(begin() + pos);
	}

	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename view<T, Iter, ConstIter>::reference view<T, Iter, ConstIter>::at(size_type pos) const
	{
		return *(cbegin() + pos);
	}

	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename view<T, Iter, ConstIter>::reference view<T, Iter, ConstIter>::operator[](const size_type n)
	{
		return at(n);
	}

	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename view<T, Iter, ConstIter>::reference view<T, Iter, ConstIter>::operator[](const size_type n) const
	{
		return at(n);
	}

	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename view<T, Iter, ConstIter>::pointer view<T, Iter, ConstIter>::data() noexcept
	{
		return m_src;
	}

	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr typename view<T, Iter, ConstIter>::pointer view<T, Iter, ConstIter>::data() const noexcept
	{
		return m_src;
	}

	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr size_type view<T, Iter, ConstIter>::size() const noexcept
	{
		return m_count;
	}

	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr size_type view<T, Iter, ConstIter>::size_bytes() const noexcept
	{
		return m_count * sizeof(value_type);
	}

	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr bool view<T, Iter, ConstIter>::empty() const noexcept
	{
		return m_count == 0 || m_src == nullptr;
	}

	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr view<T, Iter, ConstIter> view<T, Iter, ConstIter>::subview(size_type pos, size_type n) const
	{
		const size_type maxCount = m_count - pos;
		if (n > maxCount)
		{
			n = maxCount;
		}

		return view<T, Iter, ConstIter>(m_src + pos, n);
	}

	//TODO(Rowan): Implement a better search algo
	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr size_type find_first_of(view<const T, Iter> str, view<const T, Iter> key, size_type pos) noexcept
	{
		for (auto iter = str.begin() + pos; iter != str.end(); ++iter)
		{
			for (auto it = key.begin(); it != key.end(); ++it)
			{
				if (*iter == *it)
				{
					return pos;
				}
			}
			++pos;
		}
		return npos;
	}

	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr size_type find_first_not_of(view<const T, Iter> str, view<const T, Iter> key, size_type pos) noexcept
	{
		for (auto iter = str.begin() + pos; iter != str.end(); ++iter)
		{
			bool found = true;
			for (auto it = key.begin(); it != key.end(); ++it)
			{
				if (*iter == *it)
				{
					found = false;
					break;
				}
			}

			if (found)
			{
				return pos;
			}

			++pos;
		}
		return npos;
	}

	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr size_type find_last_of(view<const T, Iter> str, view<const T, Iter> key, size_type pos) noexcept
	{
		using reverse_iter = typename view<T, Iter, ConstIter>::reverse_iterator_type;
		reverse_iter endIter = reverse_iter(str.begin() + pos);
		auto keyBegin = key.begin();
		auto keyEnd = key.end();

		for (auto iter = str.rbegin(); iter != endIter; ++iter)
		{
			++pos;
			for (auto it = keyBegin; it != keyEnd; ++it)
			{
				if (*iter == *it)
				{
					return str.size() - pos;
				}
			}
		}
		return npos;
	}

	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr size_type find_last_not_of(view<const T, Iter> str, view<const T, Iter> key,
	                                     [[maybe_unused]] size_type pos) noexcept
	{
		using reverse_iter = typename view<T, Iter, ConstIter>::reverse_iterator_type;
		reverse_iter endIter = reverse_iter(str.begin() + pos);
		auto keyBegin = key.begin();
		auto keyEnd = key.end();

		for (auto iter = str.rbegin(); iter != endIter; ++iter)
		{
			++pos;
			bool found = true;
			for (auto it = keyBegin; it != keyEnd; ++it)
			{
				if (*iter == *it)
				{
					found = false;
					break;
				}
			}

			if (found)
			{
				return str.size() - pos;
			}
		}
		return npos;
	}

	template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
	constexpr size_type find_first_of(view<const T, Iter> str, const T& key, size_type pos) noexcept
	{
		return find_first_of(str, view(&key, 1), pos);
	}

	template <typename T, contiguous_iterator Iter>
	constexpr size_type find_first_of([[maybe_unused]] view<const T, Iter> str, [[maybe_unused]] view<const T, Iter> key, [[maybe_unused]] size_type pos) noexcept
	{
		rsl_assert_unimplemented();
		return 0;
	}

	template <typename T, contiguous_iterator Iter>
	constexpr size_type find_first_not_of([[maybe_unused]] view<const T, Iter> str, [[maybe_unused]] view<const T, Iter> key, [[maybe_unused]] size_type pos) noexcept
	{
		rsl_assert_unimplemented();
		return 0;
	}

	template <typename T, contiguous_iterator Iter>
	constexpr size_type find_last_of([[maybe_unused]] view<const T, Iter> str, [[maybe_unused]] view<const T, Iter> key, [[maybe_unused]] size_type pos) noexcept
	{
		rsl_assert_unimplemented();
		return 0;
	}

	template <typename T, contiguous_iterator Iter>
	constexpr size_type find_last_not_of([[maybe_unused]] view<const T, Iter> str, [[maybe_unused]] view<const T, Iter> key, [[maybe_unused]] size_type pos) noexcept
	{
		rsl_assert_unimplemented();
		return 0;
	}

	template <typename T, contiguous_iterator Iter>
	constexpr size_type find_first_of([[maybe_unused]] view<const T, Iter> str, [[maybe_unused]] const T& key, [[maybe_unused]] size_type pos) noexcept
	{
		rsl_assert_unimplemented();
		return 0;
	}

	template <typename T, contiguous_iterator Iter, same_as<T> C>
	constexpr size_type find_first_not_of(view<const T, Iter> str, const C& key, size_type pos) noexcept
	{
		return find_first_not_of(str, view(&key, 1), pos);
	}

	template <typename T, contiguous_iterator Iter, same_as<T> C>
	constexpr size_type find_last_of(view<const T, Iter> str, const C& key, size_type pos) noexcept
	{
		return find_last_of(str, view(&key, 1), pos);
	}

	template <typename T, contiguous_iterator Iter, same_as<T> C>
	constexpr size_type find_last_not_of(view<const T, Iter> str, const C& key, size_type pos) noexcept
	{
		return find_last_not_of(str, view(&key, 1), pos);
	}

	template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
	constexpr iterator_view<T, Iter, ConstIter>::iterator_view(iterator_type start, iterator_type end) noexcept
		: m_start(start), m_end(end) {}

	template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
	constexpr iterator_view<T, Iter, ConstIter>::iterator_view(pointer ptr, size_type count) noexcept
		requires same_as<Iter, T*>
		: m_start(ptr), m_end(ptr + count) {}

	template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
	template <size_type N>
	constexpr iterator_view<T, Iter, ConstIter>::iterator_view(value_type (&arr)[N]) noexcept
		requires same_as<Iter, T*>
		: m_start(arr), m_end(arr + N) {}

	template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
	constexpr iterator_view<T, Iter, ConstIter>::iterator_view(const value_type& other) noexcept
		requires same_as<Iter, T*>
		: m_start(&other), m_end((&other) + 1u) {}

	template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
	constexpr iterator_view<T, Iter, ConstIter>::operator iterator_view<const T, ConstIter>() noexcept
		requires (!is_const_v<T>)
	{
		return iterator_view<const T, const_iterator_type>(m_start, m_end);
	}

	template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
	constexpr iterator_view<T, Iter, ConstIter>::operator contiguous_view_type() noexcept
		requires (contiguous_iterator<Iter> && contiguous_iterator<ConstIter>)
	{
		if constexpr (!is_void_v<contiguous_view_type>)
		{
			return contiguous_view_type(&*m_start, size());
		}
		else
		{
			return contiguous_view_type();
		}
	}

	template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
	constexpr iterator_view<T, Iter, ConstIter>
	iterator_view<T, Iter, ConstIter>::from_string_length(T* str, T terminator) noexcept
		requires (char_type<T> && same_as<Iter, T*>)
	{
		return iterator_view(str, str + string_length(str, terminator));
	}

	template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
	constexpr bool iterator_view<T, Iter, ConstIter>::operator==(const iterator_view& rhs)
	{
		return m_start == rhs.m_start && m_end == rhs.m_end;
	}

	template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
	constexpr bool iterator_view<T, Iter, ConstIter>::operator!=(const iterator_view& rhs)
	{
		return !(*this == rhs);
	}

	template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
	constexpr typename iterator_view<T, Iter, ConstIter>::iterator_type
	iterator_view<T, Iter, ConstIter>::begin() noexcept
	{
		return m_start;
	}

	template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
	constexpr typename iterator_view<T, Iter, ConstIter>::const_iterator_type
	iterator_view<T, Iter, ConstIter>::begin() const noexcept
	{
		return m_start;
	}

	template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
	constexpr typename iterator_view<T, Iter, ConstIter>::const_iterator_type
	iterator_view<T, Iter, ConstIter>::cbegin() const noexcept
	{
		return m_start;
	}

	template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
	constexpr typename iterator_view<T, Iter, ConstIter>::iterator_type
	iterator_view<T, Iter, ConstIter>::end() noexcept
	{
		return m_end;
	}

	template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
	constexpr typename iterator_view<T, Iter, ConstIter>::const_iterator_type
	iterator_view<T, Iter, ConstIter>::end() const noexcept
	{
		return m_end;
	}

	template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
	constexpr typename iterator_view<T, Iter, ConstIter>::const_iterator_type
	iterator_view<T, Iter, ConstIter>::cend() const noexcept
	{
		return m_end;
	}

	template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
	constexpr typename iterator_view<T, Iter, ConstIter>::reverse_iterator_type
	iterator_view<T, Iter, ConstIter>::rbegin() noexcept
	{
		return reverse_iterator(end());
	}

	template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
	constexpr typename iterator_view<T, Iter, ConstIter>::const_reverse_iterator_type
	iterator_view<T, Iter, ConstIter>::rbegin() const noexcept
	{
		return crbegin();
	}

	template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
	constexpr typename iterator_view<T, Iter, ConstIter>::const_reverse_iterator_type
	iterator_view<T, Iter, ConstIter>::crbegin() const noexcept
	{
		return const_reverse_iterator_type(cend());
	}

	template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
	constexpr typename iterator_view<T, Iter, ConstIter>::reverse_iterator_type
	iterator_view<T, Iter, ConstIter>::rend() noexcept
	{
		return reverse_iterator(begin());
	}

	template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
	constexpr typename iterator_view<T, Iter, ConstIter>::const_reverse_iterator_type
	iterator_view<T, Iter, ConstIter>::rend() const noexcept
	{
		return crend();
	}

	template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
	constexpr typename iterator_view<T, Iter, ConstIter>::const_reverse_iterator_type
	iterator_view<T, Iter, ConstIter>::crend() const noexcept
	{
		return const_reverse_iterator_type(cbegin());
	}

	template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
	constexpr typename iterator_view<T, Iter, ConstIter>::reference iterator_view<T, Iter, ConstIter>::front()
	{
		return *m_start;
	}

	template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
	constexpr typename iterator_view<T, Iter, ConstIter>::reference iterator_view<T, Iter, ConstIter>::front() const
	{
		return *m_start;
	}

	namespace internal
	{
		template<weakly_incrementable Iter>
		constexpr Iter find_last_iter(Iter start, Iter end) noexcept
		{
			if constexpr (bidirectional_iterator<Iter>)
			{
				--end;
				return end;
			}
			else
			{
				while (true)
				{
					Iter nextIter = start;
					++nextIter;
					if (nextIter == end) // peek ahead
					{
						return start;
					}
					start = nextIter;
				}
			}
		}
	}

	template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
	constexpr typename iterator_view<T, Iter, ConstIter>::reference iterator_view<T, Iter, ConstIter>::back()
	{
		return internal::find_last_iter(m_start, m_end);
	}

	template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
	constexpr typename iterator_view<T, Iter, ConstIter>::reference iterator_view<T, Iter, ConstIter>::back() const
	{
		return internal::find_last_iter(m_start, m_end);
	}

	template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
	constexpr size_type iterator_view<T, Iter, ConstIter>::size() const noexcept
	{
		return static_cast<size_type>(iterator_diff(m_start, m_end));
	}

	template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
	constexpr bool iterator_view<T, Iter, ConstIter>::empty() const noexcept
	{
		return m_start == m_end;
	}
}

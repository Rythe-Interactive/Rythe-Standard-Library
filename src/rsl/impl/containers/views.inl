#pragma once

#include "views.hpp"

namespace rsl
{
	template <typename T, contiguous_iterator Iter>
	constexpr view<T, Iter>::view() noexcept = default;

	template <typename T, contiguous_iterator Iter>
	template <typename It>
	constexpr view<T, Iter>::view(It first, const size_type count) : m_src(first), m_count(count) {}

	template <typename T, contiguous_iterator Iter>
	template <typename It>
	constexpr view<T, Iter>::view(It first, It last) : m_src(first), m_count(last - first) {}

	template <typename T, contiguous_iterator Iter>
	template <size_type N>
	constexpr view<T, Iter>::view(const value_type (&arr)[N]) noexcept : m_src(const_cast<value_type*>(&arr[0])),
		m_count(N) {}

	template <typename T, contiguous_iterator Iter>
	constexpr view<T, Iter>::view(const view& other) noexcept : m_src(other.m_src), m_count(other.m_count) {}

	template <typename T, contiguous_iterator Iter>
	constexpr view<T, Iter>::view(value_type&& src) noexcept : m_src(&src), m_count(1) {}

	template <typename T, contiguous_iterator Iter>
	template <size_type N>
	constexpr view<T, Iter>::view(string_literal<N> literal) noexcept requires same_as<T, const char> : view(literal.value, literal.size()) {}

	template <typename T, contiguous_iterator Iter>
	constexpr view<T, Iter> view<T, Iter>::from_string_length(T* str, const T terminator) noexcept requires char_type<T>
	{
		return view(str, string_length(str, terminator));
	}

	template <typename T, contiguous_iterator Iter>
	constexpr bool view<T, Iter>::operator==(const view& rhs)
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

	template <typename T, contiguous_iterator Iter>
	constexpr bool view<T, Iter>::operator!=(const view& rhs)
	{
		return !(*this == rhs);
	}

	template <typename T, contiguous_iterator Iter>
	template <size_type N>
	constexpr bool view<T, Iter>::operator==(const value_type (&rhs)[N])
	{
		bool result = true;
		for (size_type i = 0; i < this->size(); ++i)
		{
			result &= this->at(i) == rhs[i];
		}

		return result;
	}

	template <typename T, contiguous_iterator Iter>
	template <size_type N>
	constexpr bool view<T, Iter>::operator!=(const value_type (&rhs)[N])
	{
		return !(*this == rhs);
	}

	template <typename T, contiguous_iterator Iter>
	constexpr typename view<T, Iter>::iterator_type view<T, Iter>::begin() noexcept
	{
		return iterator_type(m_src);
	}

	template <typename T, contiguous_iterator Iter>
	constexpr typename view<T, Iter>::const_iterator_type view<T, Iter>::begin() const noexcept
	{
		return cbegin();
	}

	template <typename T, contiguous_iterator Iter>
	constexpr typename view<T, Iter>::const_iterator_type view<T, Iter>::cbegin() const noexcept
	{
		return const_iterator_type(m_src);
	}

	template <typename T, contiguous_iterator Iter>
	constexpr typename view<T, Iter>::iterator_type view<T, Iter>::end() noexcept
	{
		return iterator_type(m_src + m_count);
	}

	template <typename T, contiguous_iterator Iter>
	constexpr typename view<T, Iter>::const_iterator_type view<T, Iter>::end() const noexcept
	{
		return cend();
	}

	template <typename T, contiguous_iterator Iter>
	constexpr typename view<T, Iter>::const_iterator_type view<T, Iter>::cend() const noexcept
	{
		return const_iterator_type(m_src + m_count);
	}

	template <typename T, contiguous_iterator Iter>
	constexpr typename view<T, Iter>::reverse_iterator_type view<T, Iter>::rbegin() noexcept
	{
		return reverse_iterator(end());
	}

	template <typename T, contiguous_iterator Iter>
	constexpr typename view<T, Iter>::const_reverse_iterator_type view<T, Iter>::rbegin() const noexcept
	{
		return crbegin();
	}

	template <typename T, contiguous_iterator Iter>
	constexpr typename view<T, Iter>::const_reverse_iterator_type view<T, Iter>::crbegin() const noexcept
	{
		return const_reverse_iterator_type(cend());
	}

	template <typename T, contiguous_iterator Iter>
	constexpr typename view<T, Iter>::reverse_iterator_type view<T, Iter>::rend() noexcept
	{
		return reverse_iterator(begin());
	}

	template <typename T, contiguous_iterator Iter>
	constexpr typename view<T, Iter>::const_reverse_iterator_type view<T, Iter>::rend() const noexcept
	{
		return crend();
	}

	template <typename T, contiguous_iterator Iter>
	constexpr typename view<T, Iter>::const_reverse_iterator_type view<T, Iter>::crend() const noexcept
	{
		return const_reverse_iterator_type(cbegin());
	}

	template <typename T, contiguous_iterator Iter>
	constexpr typename view<T, Iter>::reference view<T, Iter>::front()
	{
		return at(0);
	}

	template <typename T, contiguous_iterator Iter>
	constexpr typename view<T, Iter>::reference view<T, Iter>::front() const
	{
		return at(0);
	}

	template <typename T, contiguous_iterator Iter>
	constexpr typename view<T, Iter>::reference view<T, Iter>::back()
	{
		return at(m_count - 1);
	}

	template <typename T, contiguous_iterator Iter>
	constexpr typename view<T, Iter>::reference view<T, Iter>::back() const
	{
		return at(m_count - 1);
	}

	template <typename T, contiguous_iterator Iter>
	constexpr typename view<T, Iter>::reference view<T, Iter>::at(size_type pos)
	{
		return *(begin() + pos);
	}

	template <typename T, contiguous_iterator Iter>
	constexpr typename view<T, Iter>::reference view<T, Iter>::at(size_type pos) const
	{
		return *(cbegin() + pos);
	}

	template <typename T, contiguous_iterator Iter>
	constexpr typename view<T, Iter>::reference view<T, Iter>::operator[](size_type n)
	{
		return at(n);
	}

	template <typename T, contiguous_iterator Iter>
	constexpr typename view<T, Iter>::reference view<T, Iter>::operator[](size_type n) const
	{
		return at(n);
	}

	template <typename T, contiguous_iterator Iter>
	constexpr typename view<T, Iter>::pointer view<T, Iter>::data() noexcept
	{
		return m_src;
	}

	template <typename T, contiguous_iterator Iter>
	constexpr typename view<T, Iter>::pointer view<T, Iter>::data() const noexcept
	{
		return m_src;
	}

	template <typename T, contiguous_iterator Iter>
	constexpr size_type view<T, Iter>::size() const noexcept
	{
		return m_count;
	}

	template <typename T, contiguous_iterator Iter>
	constexpr size_type view<T, Iter>::size_bytes() const noexcept
	{
		return m_count * sizeof(value_type);
	}

	template <typename T, contiguous_iterator Iter>
	constexpr bool view<T, Iter>::empty() const noexcept
	{
		return m_count == 0 || m_src == nullptr;
	}

	template <typename T, contiguous_iterator Iter>
	constexpr view<T, Iter> view<T, Iter>::subview(size_type pos, size_type n) const
	{
		const size_type maxCount = m_count - pos;
		if (n > maxCount)
		{
			n = maxCount;
		}

		return view<T, Iter>(m_src + pos, n);
	}

	//TODO(Rowan): Implement a better search algo
	template <typename T, contiguous_iterator Iter>
	constexpr size_type find_first_of(view<T, Iter> str, view<T, Iter> key, size_type pos) noexcept
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

	template <typename T, contiguous_iterator Iter>
	constexpr size_type find_first_not_of(view<T, Iter> str, view<T, Iter> key, size_type pos) noexcept
	{
		for (auto iter = str.begin() + pos; iter != str.end(); ++iter)
		{
			for (auto it = key.begin(); it != key.end(); ++it)
			{
				if (*iter != *it)
				{
					return pos;
				}
			}
			++pos;
		}
		return npos;
	}

	template <typename T, contiguous_iterator Iter>
	constexpr size_type find_last_of(view<T, Iter> str, view<T, Iter> key, size_type pos) noexcept
	{
		using reverse_iter = typename view<T, Iter>::reverse_iterator_type;
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

	template <typename T, contiguous_iterator Iter>
	constexpr size_type find_last_not_of(view<T, Iter> str, view<T, Iter> key, [[maybe_unused]] size_type pos) noexcept
	{
		using reverse_iter = typename view<T, Iter>::reverse_iterator_type;
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

	template <typename T, contiguous_iterator Iter>
	constexpr size_type find_first_of(view<T, Iter> str, add_const_t<T>& key, size_type pos) noexcept
	{
		return find_first_of(str, view(&key, 1), pos);
	}

	template <typename T, contiguous_iterator Iter>
	constexpr size_type find_first_not_of(view<T, Iter> str, add_const_t<T>& key, size_type pos) noexcept
	{
		return find_first_not_of(str, view(&key, 1), pos);
	}

	template <typename T, contiguous_iterator Iter>
	constexpr size_type find_last_of(view<T, Iter> str, add_const_t<T>& key, size_type pos) noexcept
	{
		return find_last_of(str, view(&key, 1), pos);
	}

	template <typename T, contiguous_iterator Iter>
	constexpr size_type find_last_not_of(view<T, Iter> str, add_const_t<T>& key, size_type pos) noexcept
	{
		return find_last_not_of(str, view(&key, 1), pos);
	}
}

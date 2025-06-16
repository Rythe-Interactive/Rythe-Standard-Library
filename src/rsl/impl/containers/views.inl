#pragma once

#include "views.hpp"

namespace rsl
{
	template <typename T, contiguous_iterator Iter>
	constexpr view<T, Iter>::view() noexcept = default;

	template <typename T, contiguous_iterator Iter>
	template <typename It>
	constexpr view<T, Iter>::view(It first, size_type count) : m_src(first), m_count(count) {}

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
	constexpr bool view<T, Iter>::operator==(const view& rhs)
	{
		bool result = true;
		for (size_type i = 0; i < this->size(); ++i)
		{
			result &= this->at(i) == rhs.at(i);
		}

		return result;
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

	template <typename T, contiguous_iterator Iter>
	constexpr size_type find_first_of(view<T, Iter> str, view<T, Iter> other, size_type pos) noexcept
	{
		size_type count = pos;
		for (auto iter = str.begin() + pos; iter != str.end(); ++iter)
		{
			++count;
			for (auto it = other.begin(); it != other.end(); ++it)
				if (*iter == *it)
					return count;
		}
		return npos;
	}

	template <typename T, contiguous_iterator Iter>
	constexpr size_type find_first_not_of(view<T, Iter> str, view<T, Iter> other, size_type pos) noexcept
	{
		size_type count = pos;
		for (auto iter = str.begin() + pos; iter != str.end(); ++iter)
		{
			++count;
			for (auto it = other.begin(); it != other.end(); ++it)
				if (*iter != *it)
					return count;
		}
		return npos;
	}

	template <typename T, contiguous_iterator Iter>
	constexpr size_type find_last_of(view<T, Iter> str, view<T, Iter> other, size_type pos) noexcept
	{
		size_type count = pos;
		for (auto iter = str.rbegin() + pos; iter != str.rend(); ++iter)
		{
			--count;
			for (auto it = other.begin(); it != other.end(); ++it)
			{
				if (*iter == *it)
				{
					return count;
				}
			}
		}
		return npos;
	}

	template <typename T, contiguous_iterator Iter>
	constexpr size_type find_last_not_of(view<T, Iter> str, view<T, Iter> other, [[maybe_unused]] size_type pos) noexcept
	{
		size_type count = str.size();
		for (auto iter = str.rbegin(); iter != str.rend() + pos; ++iter)
		{
			--count;
			for (auto it = other.begin(); it != other.end(); ++it)
				if (*iter != *it)
					return count;
		}
		return npos;
	}

	template <typename T, contiguous_iterator Iter>
	constexpr size_type find_first_of(view<T, Iter> str, add_const_t<T>& other, size_type pos) noexcept
	{
		return find_first_of(str, view(&other, 1), pos);
	}

	template <typename T, contiguous_iterator Iter>
	constexpr size_type find_first_not_of(view<T, Iter> str, add_const_t<T>& other, size_type pos) noexcept
	{
		return find_first_not_of(str, view(&other, 1), pos);
	}

	template <typename T, contiguous_iterator Iter>
	constexpr size_type find_last_of(view<T, Iter> str, add_const_t<T>& other, size_type pos) noexcept
	{
		return find_last_of(str, view(&other, 1), pos);
	}

	template <typename T, contiguous_iterator Iter>
	constexpr size_type find_last_not_of(view<T, Iter> str, add_const_t<T>& other, size_type pos) noexcept
	{
		return find_last_not_of(str, view(&other, 1), pos);
	}
}

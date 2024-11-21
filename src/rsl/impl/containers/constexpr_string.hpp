#pragma once

#include "../util/primitives.hpp"
#include "string_view.hpp"

namespace rsl
{
	template <size_type N>
	struct constexpr_string
	{
		char buffer[N];

		consteval constexpr_string() noexcept = default;
		consteval constexpr_string(const char (&arr)[N]) noexcept
		{
			for (size_type i = 0; i < N; ++i) { buffer[i] = arr[i]; }
		}

		[[nodiscard]] consteval size_type capacity() const noexcept { return N; }
		[[nodiscard]] consteval size_type size() const noexcept;

		constexpr char& operator[](size_type i) noexcept { return buffer[i]; }
		constexpr const char& operator[](size_type i) const noexcept { return buffer[i]; }

		constexpr const char* data() const noexcept { return buffer; }
		constexpr operator const char*() const noexcept { return data(); }

		template <typename F>
		[[nodiscard]] consteval constexpr_string<N> filter_if(F f) const noexcept;
		[[nodiscard]] consteval constexpr_string<N> filter(char x) const noexcept;

		template <size_type I>
		[[nodiscard]] consteval constexpr_string<I> refit() const noexcept;

        consteval void copy_from(string_view str) noexcept;
	};

	template <size_type A, size_type B>
	[[nodiscard]] consteval auto operator+(constexpr_string<A> lhs, constexpr_string<B> rhs) noexcept
	{
		constexpr_string<A + B - 1> retval;
		// copy up to first nil in lhs:
		for (size_type i = 0; i < lhs.size(); ++i) { retval[i] = lhs[i]; }
		// copy entire rhs buffer, including trailing nils:
		for (size_type i = 0; i < B; ++i) { retval[lhs.size() + i] = rhs[i]; }
		// zero out the leftovers, if any:
		for (size_type i = lhs.size() + B; i < A + B - 1; ++i) { retval[i] = 0; }

		return retval;
	}

	template <size_type N>
	template <typename F>
	inline consteval constexpr_string<N> constexpr_string<N>::filter_if(F f) const noexcept
	{
		constexpr_string<N> ret{};
		size_type j = 0;
		for (size_type i = 0; i < N; ++i)
		{
			ret[j] = buffer[i];
			if (f(buffer[i]))
			{
				++j;
			}
		}

		for (j = j + 1; j < N; ++j) { ret[j] = 0; }
		return ret;
	}

	template <size_type N>
	template <size_type I>
	inline consteval constexpr_string<I> constexpr_string<N>::refit() const noexcept
	{
		constexpr_string<I> ret{};

		constexpr size_type minSize = I < N ? I : N;
		for (size_type i = 0; i < minSize; i++) { ret[i] = buffer[i]; }
		for (size_type i = minSize; i < I; i++) { ret[i] = '\0'; }
		return ret;
	}

	template <size_type N>
	inline consteval size_type constexpr_string<N>::size() const noexcept
	{
		size_type s = 0ull;
		while (s + 1 < N && buffer[s]) { ++s; }
		return s;
	}

	template <size_type N>
	inline consteval constexpr_string<N> constexpr_string<N>::filter(char x) const noexcept
	{
		return filter_if([x](char y) { return x != y; });
	}

	template <size_type N>
	inline consteval void constexpr_string<N>::copy_from(string_view str) noexcept
	{
		size_type minSize = N < str.size() ? N : str.size();
		for (size_type i = 0; i < minSize; ++i) { buffer[i] = str[i]; }
		for (size_type i = minSize; i < N; ++i) { buffer[i] = '\0'; }
	}

} // namespace rsl

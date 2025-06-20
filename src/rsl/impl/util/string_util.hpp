#pragma once
#include "primitives.hpp"
#include "concepts.hpp"

namespace rsl
{
	template <size_type N>
	struct string_literal
	{
		const char (&value)[N];

		constexpr string_literal(const char (&str)[N]) noexcept
			: value(str)
		{
		}

		constexpr string_literal(const string_literal& src) noexcept
			: value(src.value)
		{
		}
		constexpr string_literal(string_literal&& src) noexcept
			: value(src.value)
		{
		}

		constexpr string_literal& operator=(const string_literal& src) noexcept { value = src.value; return *this; }
		constexpr string_literal& operator=(string_literal&& src) noexcept { value = src.value; return *this; }

		constexpr size_type size() const noexcept { return N - 1; }

		constexpr operator rsl::cstring() const noexcept { return value; }
	};

	template <size_type N>
	string_literal(const char (&)[N]) -> string_literal<N>;

	template<char_type T>
	[[nodiscard]] constexpr size_type string_length(add_const_t<T>* str, T terminator = T{}) noexcept
	{
		size_type length = 0;
		while (*str != terminator)
		{
			++length;
			++str;
		}

		return length;
	}

	template <class T, size_type N>
	constexpr size_type arr_length(T[N])
	{
		return N;
	}
} // namespace rsl

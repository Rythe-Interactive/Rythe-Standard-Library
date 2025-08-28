#pragma once
#include "../util/hash.hpp"
#include "../util/primitives.hpp"

#include "views.hpp"

// TODO(Glyn): Revisit these!

namespace rsl
{
	template <typename Alloc>
	struct basic_hashed_string;

	using hashed_string = basic_hashed_string<default_allocator>;

	struct hashed_string_view
	{
		u64 value = 0;
		string_view str;

		constexpr hashed_string_view() noexcept = default;

		constexpr hashed_string_view(const hashed_string& str) noexcept;
		constexpr hashed_string_view(cstring str) noexcept
			: value(hash_string(string_view::from_string_length(str))),
			  str(string_view::from_string_length(str))
		{
		}
		constexpr hashed_string_view(string_view str) noexcept
			: value(hash_string(str)),
			  str(str)
		{
		}

		template <typename Alloc>
		constexpr hashed_string_view(const std::basic_string<char, std::char_traits<char>, Alloc>& str) noexcept
			: value(hash_string(str)),
			  str(str)
		{
		}

		constexpr cstring data() const noexcept { return str.data(); }
		constexpr string_view view() const noexcept { return str; }

		constexpr operator u64() const noexcept { return value; }
		constexpr operator string_view() const noexcept { return str; }
	};

	constexpr auto operator<=>(hashed_string_view lhs, hashed_string_view rhs) noexcept
	{
		return lhs.value <=> rhs.value;
	}

	constexpr auto operator==(hashed_string_view lhs, hashed_string_view rhs) noexcept
	{
		return lhs.value == rhs.value;
	}

	namespace hashed_string_literals
	{
		consteval hashed_string_view operator""_hsv(cstring str, size_type size) noexcept
		{
			return hashed_string_view(string_view(str, size));
		}
	} // namespace hashed_string_literals

	template <typename Alloc>
	class basic_hashed_string
	{
	public:
		using string_type = basic_dynamic_string<char, Alloc>;

		u64 value = 0;
		string_type str;

		constexpr basic_hashed_string() noexcept = default;

		explicit constexpr basic_hashed_string(cstring str) noexcept
			: value(hash_string(string_view::from_string_length(str))),
			  str(string_type::from_string_length(str))
		{
		}
		explicit constexpr basic_hashed_string(string_view str) noexcept
			: value(hash_string(str)),
			  str(string_type::from_view(str))
		{
		}
		explicit constexpr basic_hashed_string(hashed_string_view other) noexcept
			: value(other.value),
			  str(other)
		{
		}
		explicit constexpr basic_hashed_string(const string_type& str) noexcept
			: value(hash_string(str)),
			  str(str)
		{
		}
		explicit constexpr basic_hashed_string(string_type&& str) noexcept
			: value(hash_string(str)),
			  str(std::move(str))
		{
		}

		constexpr const char* data() const noexcept { return str.data(); }
		constexpr string_view view() const noexcept { return str; }

		constexpr operator u64() const noexcept { return value; }
		constexpr operator string_type&() noexcept { return str; }
		constexpr operator const string_type&() const noexcept { return str; }
		constexpr operator string_view() const noexcept { return str; }
	};

	constexpr hashed_string_view::hashed_string_view(const hashed_string& other) noexcept
		: value(other.value),
		  str(other.str)
	{
	}

	constexpr auto operator<=>(const hashed_string& lhs, const hashed_string& rhs) noexcept
	{
		return lhs.value <=> rhs.value;
	}

	constexpr auto operator==(const hashed_string& lhs, const hashed_string& rhs) noexcept
	{
		return lhs.value == rhs.value;
	}

	constexpr auto operator<=>(hashed_string_view lhs, const hashed_string& rhs) noexcept
	{
		return lhs.value <=> rhs.value;
	}

	constexpr auto operator==(const hashed_string& lhs, hashed_string_view rhs) noexcept
	{
		return lhs.value == rhs.value;
	}

	namespace hashed_string_literals
	{
		consteval hashed_string operator""_hs(cstring str, size_type size) noexcept
		{
			return hashed_string(string_view(str, size));
		}
	} // namespace hashed_string_literals

	template <>
	struct hash<hashed_string>
	{
		[[rythe_always_inline]] constexpr id_type operator()(const hashed_string& val) const noexcept
		{
			return val.value;
		}
	};

	template <>
	struct hash<hashed_string_view>
	{
		[[rythe_always_inline]] constexpr id_type operator()(const hashed_string_view& val) const noexcept
		{
			return val.value;
		}
	};
} // namespace rsl

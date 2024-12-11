#pragma once
#include "../../logging/spdlog_include.hpp"

#include "../buffered_string.hpp"

namespace fmt
{
	template <rsl::size_type maxSize, typename CharType>
	struct formatter<rsl::buffered_string<maxSize, CharType>>
	{
		constexpr const char* parse(format_parse_context& ctx)
		{
			return formatter<typename rsl::buffered_string<maxSize, CharType>::view_type>{}.parse(ctx);
		}

		template <typename FormatContext>
		auto format(const rsl::buffered_string<maxSize, CharType>& str, FormatContext& ctx)
		{
			return format_to(ctx.out(), format_string<typename rsl::buffered_string<maxSize, CharType>::view_type>("{}"), str.view());
		}
	};
} // namespace fmt

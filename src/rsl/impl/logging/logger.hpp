#pragma once

#include "../util/source_location.hpp"
#include "../memory/unique_object.hpp"
#include "../containers/string.hpp"

#include "severity.hpp"

namespace rsl
{
	struct format_string
	{
		[[rythe_always_inline]] constexpr format_string(
			string_view s,
			const source_location loc = source_location::current())
			noexcept : str(s), srcLoc(loc) {}

		string_view str;
		source_location srcLoc;
	};

	class logger
	{
	public:
		explicit logger(string_view name) : m_name(dynamic_string::from_view(name)) {}

		template <typename... Args>
		[[rythe_always_inline]] constexpr void log(log::severity s, format_string format, Args&&... args) noexcept;

		void log(log::severity s, format_string format, fmt::format_args args) noexcept;

	private:
		dynamic_string m_name;

		struct impl;
		unique_object<impl> m_impl;
	};

	template <typename... Args>
	constexpr void logger::log(const log::severity s, format_string format, Args&&... args) noexcept
	{
		log(s, format, fmt::make_format_args(rsl::forward<Args>(args)...));
	}
} // namespace rsl

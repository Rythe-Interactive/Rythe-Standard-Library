#pragma once

#include "../memory/unique_object.hpp"

#include "severity.hpp"

namespace rsl
{
	class logger
	{
	public:
		template <typename... Args, typename FormatString>
		[[rythe_always_inline]] constexpr void log(log::severity s, const FormatString& format, Args&&... args) noexcept;

	private:
		struct impl;
		unique_object<impl> m_impl;

		spdlog::logger* get_underlying();
	};

	template <typename ... Args, typename FormatString>
	constexpr void logger::log(const log::severity s, const FormatString& format, Args&&... args) noexcept
	{
		get_underlying()->log(log::internal::rythe_to_spdlog(s), format, rsl::forward<Args>(args)...);
	}
} // namespace rsl

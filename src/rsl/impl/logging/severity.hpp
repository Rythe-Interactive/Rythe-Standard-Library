#pragma once

#include "spdlog_include.hpp"

namespace rsl::log
{
	enum struct severity : uint8
	{
		trace = 0, // lowest severity
		debug,
		info,
		warn,
		error,
		fatal, // highest severity
		#if defined(RYTHE_DEBUG)
		default_severity = debug
		#else
		default_severity = info
		#endif
	};

	namespace internal
	{
		[[nodiscard]] [[rythe_always_inline]] constexpr static spdlog::level::level_enum rythe_to_spdlog(const severity s)
		{
			switch (s)
			{
				case severity::trace: return spdlog::level::trace;
				case severity::debug: return spdlog::level::debug;
				case severity::info: return spdlog::level::info;
				case severity::warn: return spdlog::level::warn;
				case severity::error: return spdlog::level::err;
				case severity::fatal: return spdlog::level::critical;
			}
			rsl_assert_unreachable();
			return spdlog::level::critical;
		}
	}
}

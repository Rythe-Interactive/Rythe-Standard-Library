#pragma once

#include "spdlog_include.hpp"

namespace rsl::log
{
	enum struct severity
	{
		trace, // lowest severity
		debug,
		info,
		warn,
		error,
		fatal // highest severity
	};

	// TODO(Glyn): Remove
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
			return spdlog::level::err;
		}
	}
}

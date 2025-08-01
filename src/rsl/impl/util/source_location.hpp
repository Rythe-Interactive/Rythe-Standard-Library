#pragma once

#include "primitives.hpp"

namespace rsl
{
	struct source_location
	{
		[[no_discard]] static consteval source_location current(
			const uint32 line = __builtin_LINE(),
			const uint32 column = __builtin_COLUMN(),
			cstring const file = __builtin_FILE(),
			cstring const function = __builtin_FUNCTION()
		) noexcept
		{
			source_location result;
			result.m_line = line;
			result.m_column = column;
			result.m_file = file;
			result.m_function = function;
			return result;
		}

		[[no_discard]] constexpr source_location() noexcept = default;

		[[no_discard]] constexpr uint32 line() const noexcept
		{
			return m_line;
		}

		[[no_discard]] constexpr uint32 column() const noexcept
		{
			return m_column;
		}

		[[no_discard]] constexpr cstring file_name() const noexcept
		{
			return m_file;
		}

		[[no_discard]] constexpr cstring function_name() const noexcept
		{
			return m_function;
		}

	private:
		uint32 m_line{};
		uint32 m_column{};
		cstring m_file = "";
		cstring m_function = "";
	};
}

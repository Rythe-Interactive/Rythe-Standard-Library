#pragma once
#include "../time/time_point.hpp"
#include "../containers/string.hpp"
#include "fmt_include.hpp"

namespace rsl::log
{
	struct message;

	class formatter
	{
	public:
		NO_DTOR_RULE5_CONSTEXPR_NOEXCEPT(formatter)
		virtual ~formatter() = default;
		virtual void format(const message& msg, fmt::memory_buffer& dest) = 0;
	};

	class flag_formatter
	{
	public:
		NO_DTOR_RULE5_CONSTEXPR_NOEXCEPT(flag_formatter)
		virtual ~flag_formatter() = default;
		virtual void format(const message& msg, const time::point32 time, fmt::memory_buffer& dest) = 0;
	};

	class genesis_flag_formatter : public flag_formatter
	{
	public:
		void format(const message& msg, const time::point32 time, fmt::memory_buffer& dest) override;
	};

	class logger_name_formatter : public flag_formatter
	{
	public:
		void format(const message& msg, const time::point32 time, fmt::memory_buffer& dest) override;
	};

	class thread_name_formatter_flag : public flag_formatter
	{
	public:
		void format(const message& msg, const time::point32 time, fmt::memory_buffer& dest) override;
	};
} // namespace rsl

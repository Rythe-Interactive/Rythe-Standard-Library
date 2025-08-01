#pragma once
#include "time/time_point.hpp"

#include "fmt_include.hpp"

#include <iterator>

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
} // namespace rsl

#pragma once
#include "severity.hpp"

namespace rsl::log
{
	struct message;
	class formatter;

	class sink
	{
	public:
		NO_DTOR_RULE5_CONSTEXPR_NOEXCEPT(sink)
		virtual ~sink() = default;

		virtual void log(const message& msg) = 0;
		virtual void flush() = 0;
		virtual void set_pattern(string_view pattern) = 0;
		virtual void set_formatter(formatter&& sinkFormatter) = 0;

        [[rythe_always_inline]] void filter(severity s) noexcept;
		[[nodiscard]] [[rythe_always_inline]] severity filter_severity() const noexcept;
		[[nodiscard]] [[rythe_always_inline]] bool should_log(severity s) const noexcept;

	private:
		severity m_severity = severity::default_severity;
	};
}

#include "sink.inl"

#pragma once
#include "../memory/unique_object.hpp"

#include "severity.hpp"
#include "formatter.hpp"

namespace rsl::log
{
	struct message;

	class sink
	{
	public:
		NO_DTOR_RULE5_CONSTEXPR_NOEXCEPT(sink)
		virtual ~sink() = default;

		virtual void log(const message& msg) = 0;
		virtual void flush() = 0;
		[[rythe_always_inline]] void set_formatter(formatter&& sinkFormatter);

        [[rythe_always_inline]] void filter(severity s) noexcept;
		[[nodiscard]] [[rythe_always_inline]] severity filter_severity() const noexcept;

	private:
		severity m_severity = severity::default_severity;
		unique_object<formatter> m_formatter;
	};
}

#include "sink.inl"

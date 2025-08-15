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
		constexpr sink() noexcept = default;
		constexpr sink(sink&&) noexcept = default;
		constexpr sink& operator=(sink&&) noexcept = default;

		virtual ~sink() = default;

		virtual void log(const message& msg) = 0;
		virtual void flush() = 0;
		[[rythe_always_inline]] void set_formatter(temporary_object<formatter>&& sinkFormatter);
		template <derived_from<formatter> FormatterType, typename... Args>
		[[rythe_always_inline]] void set_formatter(Args&&... args);

        [[rythe_always_inline]] void filter(severity s) noexcept;
		[[nodiscard]] [[rythe_always_inline]] severity filter_severity() const noexcept;
	private:
		severity m_severity = severity::default_severity;
		unique_object<formatter> m_formatter;
	};
}

#include "sink.inl"

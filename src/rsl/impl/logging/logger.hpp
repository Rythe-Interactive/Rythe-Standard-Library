#pragma once

#include "../util/source_location.hpp"
#include "../containers/string.hpp"

#include "severity.hpp"

namespace rsl::log
{
	class sink;
	struct message;

	struct format_string
	{
		[[rythe_always_inline]] constexpr format_string(
			string_view s,
			const source_location loc = source_location::current())
			noexcept : str(s), srcLoc(loc) {}

		string_view str;
		source_location srcLoc;
	};

	class basic_logger
	{
	public:
		explicit basic_logger(string_view name, log::severity severity = log::severity::default_severity,
		                      log::severity flushSeverity = log::severity::default_flush_severity);

		virtual ~basic_logger() = default;

		template <typename... Args>
		[[rythe_always_inline]] constexpr void log(log::severity s, format_string format, Args&&... args) noexcept;

		void log(log::severity s, format_string format, fmt::format_args args) noexcept;

		void flush();

		[[rythe_always_inline]] void set_sinks(view<sink*> sinks);
		[[nodiscard]] [[rythe_always_inline]] view<sink* const> view_sinks() const noexcept;

		[[rythe_always_inline]] void filter(severity s) noexcept;
		[[nodiscard]] [[rythe_always_inline]] severity filter_severity() const noexcept;

		[[rythe_always_inline]] void flush_at(severity s) noexcept;
		[[nodiscard]] [[rythe_always_inline]] severity flush_severity() const noexcept;
	protected:
		virtual void log(const log::message& message) = 0;

		dynamic_string m_name;
		log::severity m_severity;
		log::severity m_flushSeverity;
		dynamic_array<sink*> m_sinks;
	};

	class synchronous_logger final : public basic_logger
	{
	public:
		using basic_logger::basic_logger;

	protected:
		void log(const log::message& message) override;
	};

	using logger = synchronous_logger;
} // namespace rsl::log

#include "logger.inl"

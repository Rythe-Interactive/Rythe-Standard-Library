#pragma once
#include "../containers/string.hpp"
#include "../containers/views.hpp"
#include "../memory/unique_object.hpp"
#include "../time/time_point.hpp"

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
		virtual void format(const message& msg, time::point32 time, fmt::memory_buffer& dest) = 0;
	    virtual void set_flag_options(string_view) {}
	};

	class pattern_formatter final : public formatter
	{
	public:
		template <derived_from<flag_formatter>... FlagFormatterTypes>
		explicit pattern_formatter(string_view pattern, FlagFormatterTypes&&... flagFormatters);

		void format(const message& msg, fmt::memory_buffer& dest) override;

		template <derived_from<flag_formatter>... FlagFormatterTypes>
		void set_pattern(string_view pattern, FlagFormatterTypes&&... flagFormatters);

	private:
		void compile_pattern(array_view<temporary_object<flag_formatter>> flagFormatters);

		dynamic_string m_pattern;
		using flag_formatter_ptr = unique_object<flag_formatter>;
		dynamic_array<flag_formatter_ptr> m_formatters;
	};

	class genesis_flag_formatter final : public flag_formatter
	{
	public:
		void format(const message& msg, const time::point32 time, fmt::memory_buffer& dest) override;
	};

	class logger_name_formatter final : public flag_formatter
	{
	public:
		void format(const message& msg, const time::point32 time, fmt::memory_buffer& dest) override;
	};

	class thread_name_formatter_flag final : public flag_formatter
	{
	public:
		void format(const message& msg, const time::point32 time, fmt::memory_buffer& dest) override;
	};
} // namespace rsl

#include "formatter.inl"

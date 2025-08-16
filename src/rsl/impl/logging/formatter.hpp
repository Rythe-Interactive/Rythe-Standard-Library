#pragma once
#include "../time/time_point.hpp"
#include "../containers/string.hpp"
#include "../containers/views.hpp"
#include "../memory/unique_object.hpp"

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

	class pattern_formatter final : public formatter
	{
	public:
		using input_flag_pair = pair<char, temporary_object<flag_formatter>>;

		template <typename Iter = input_flag_pair*, typename ConstIter = internal::select_const_iter<input_flag_pair, Iter>>
		explicit pattern_formatter(string_view pattern, iterator_view<input_flag_pair, Iter, ConstIter> flags = {});

		void format(const message& msg, fmt::memory_buffer& dest) override;

		template <derived_from<flag_formatter>... Formatters>
		void set_pattern(string_view pattern, Formatters&&... formatters);

	private:
		void compile_pattern(view<temporary_object<flag_formatter>> formatters);

		dynamic_string m_pattern;

		using flag_formatter_ptr = unique_object<flag_formatter>;
		using formatters_map = dynamic_map<char, flag_formatter_ptr>;
		formatters_map m_flags;
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

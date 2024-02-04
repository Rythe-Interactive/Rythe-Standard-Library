#pragma once
#include "spdlog_include.hpp"

namespace rsl
{
	class genesis_formatter_flag : public spdlog::custom_flag_formatter
	{
	public:
		void format(const spdlog::details::log_msg& msg, const std::tm& tm_time, spdlog::memory_buf_t& dest) override;

		// generates a new formatter flag
		[[nodiscard]] std::unique_ptr<custom_flag_formatter> clone() const override { return spdlog::details::make_unique<genesis_formatter_flag>(); }
	};
} // namespace rsl

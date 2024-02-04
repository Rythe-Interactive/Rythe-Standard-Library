#include "formatter.hpp"
#include "../time/stopwatch.hpp"

namespace rsl
{
	void genesis_formatter_flag::format([[maybe_unused]] const spdlog::details::log_msg& msg, [[maybe_unused]] const std::tm& tm_time, spdlog::memory_buf_t& dest)
	{
		// get seconds since engine start
		const auto timeSinceGenesis = main_clock.current_point() - genesis;
		const auto seconds = timeSinceGenesis.seconds();

		// convert to "--s.ms---"
		const auto str = std::to_string(seconds);

		// append to data
		dest.append(str.data(), str.data() + str.size());
	}
} // namespace rsl

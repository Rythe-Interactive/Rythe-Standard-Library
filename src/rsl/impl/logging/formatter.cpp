#include "formatter.hpp"

namespace rsl::log
{
	void genesis_flag_formatter::format([[maybe_unused]] const message& msg, const time::point32 time,
	                                    fmt::memory_buffer& dest)
	{
		// get seconds since engine start
		const auto timeSinceGenesis = time - time::genesis;
		const auto seconds = timeSinceGenesis.seconds();

		// convert to "--s.ms---"
		const auto str = to_string(seconds);

		// append to data
		dest.append(str.data(), str.data() + str.size());
	}
} // namespace rsl

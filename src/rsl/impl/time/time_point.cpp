#include "time_point.hpp"
#include "stopwatch.hpp"

namespace rsl
{
	const time_point<> genesis = main_clock.start_point();
}

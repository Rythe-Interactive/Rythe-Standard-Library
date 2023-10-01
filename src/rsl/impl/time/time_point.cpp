#include "time_point.hpp"
#include "stopwatch.hpp"

namespace rsl {
    const time_point<> genesis = mainClock.start_point();
}

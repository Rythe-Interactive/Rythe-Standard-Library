#pragma once
#include <chrono>

#include "../defines.hpp"
#include "../util/primitives.hpp"

#include "time_span.hpp"


namespace rsl
{
    template<typename Clock>
    concept chrono_clock = requires {
        { Clock::now() } -> specialization_of<std::chrono::time_point>;
        requires specialization_of<typename Clock::duration, std::chrono::duration>;
    };

    template<time_duration_rep precision = fast_time, chrono_clock clock_t = std::chrono::high_resolution_clock>
    struct stopwatch
    {
    public:
        using time_type = precision;
        using span_type = time_span<time_type>;
        using clock_type = clock_t;

    private:
        std::chrono::time_point<clock_type> m_start = clock_type::now();
    public:

        always_inline void start() noexcept { m_start = clock_type::now(); }

        always_inline void fast_forward(span_type time) { m_start -= std::chrono::duration_cast<typename clock_type::duration>(time.duration); }

        always_inline void rewind(span_type time) { m_start += std::chrono::duration_cast<typename clock_type::duration>(time.duration); }

        always_inline span_type start_point() const noexcept { return span_type(m_start.time_since_epoch()); }

        always_inline span_type elapsed_time() const noexcept { return span_type(clock_type::now() - m_start); }

        always_inline span_type end() const noexcept { return span_type(clock_type::now() - m_start); }

        always_inline span_type restart() noexcept
        {
            auto startTime = clock_type::now();
            span_type time(startTime - m_start);
            m_start = startTime;
            return time;
        }
    };

    using timer = stopwatch<>;
}

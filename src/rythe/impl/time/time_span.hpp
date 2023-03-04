#pragma once
#include <chrono>

#include "../util/primitives.hpp"
#include "../util/concepts.hpp"

namespace rsl
{
    template<typename Rep>
    concept time_duration_rep = !is_specialization_v<Rep, std::chrono::duration>;

    template<time_duration_rep precision = fast_time>
    struct time_span
    {
        using time_type = precision;
        using duration_type = std::chrono::duration<time_type>;

        duration_type duration = duration_type::zero();

        constexpr time_span() noexcept = default;

        constexpr time_span(time_type other) noexcept : duration(other) {}

        template<time_duration_rep other_time, typename ratio>
        constexpr explicit time_span(const std::chrono::duration<other_time, ratio>& other) noexcept
            : duration(std::chrono::duration_cast<duration_type>(other)) {}

        template<time_duration_rep other_time>
        requires (!std::same_as<other_time, time_type>)
        constexpr explicit time_span(const time_span<other_time>& other) noexcept : duration(other.duration) {}

        template<time_duration_rep other_time, ratio_type ratio>
        constexpr explicit time_span(std::chrono::duration<other_time, ratio>&& other) noexcept
            : duration(std::chrono::duration_cast<duration_type>(other)) {}

        template<time_duration_rep other_time>
        requires (!std::same_as<other_time, time_type>)
        constexpr explicit time_span(time_span<other_time>&& other) noexcept : duration(std::move(other.duration)) {}

        template<time_duration_rep other_time>
        requires (!std::same_as<other_time, time_type>)
        constexpr explicit time_span(other_time other) noexcept : duration(other) {}

        template<time_duration_rep other_time>
        constexpr time_span<other_time> cast() const noexcept { return { *this }; }

        template<time_duration_rep T>
        [[nodiscard]] constexpr T hours() const noexcept(std::is_arithmetic_v<time_type>&& std::is_arithmetic_v<T>) { return std::chrono::duration_cast<std::chrono::duration<T, std::ratio<3600>>>(duration).count(); }
        template<time_duration_rep T>
        [[nodiscard]] constexpr T minutes() const noexcept(std::is_arithmetic_v<time_type>&& std::is_arithmetic_v<T>) { return std::chrono::duration_cast<std::chrono::duration<T, std::ratio<60>>>(duration).count(); }
        template<time_duration_rep T>
        [[nodiscard]] constexpr T seconds() const noexcept(std::is_arithmetic_v<time_type>&& std::is_arithmetic_v<T>) { return std::chrono::duration_cast<std::chrono::duration<T>>(duration).count(); }
        template<time_duration_rep T>
        [[nodiscard]] constexpr T milliseconds() const noexcept(std::is_arithmetic_v<time_type>&& std::is_arithmetic_v<T>) { return std::chrono::duration_cast<std::chrono::duration<T, std::milli>>(duration).count(); }
        template<time_duration_rep T>
        [[nodiscard]] constexpr T microseconds() const noexcept(std::is_arithmetic_v<time_type>&& std::is_arithmetic_v<T>) { return std::chrono::duration_cast<std::chrono::duration<T, std::micro>>(duration).count(); }
        template<time_duration_rep T>
        [[nodiscard]] constexpr T nanoseconds() const noexcept(std::is_arithmetic_v<time_type> && std::is_arithmetic_v<T>) { return std::chrono::duration_cast<std::chrono::duration<T, std::nano>>(duration).count(); }

        [[nodiscard]] constexpr time_type hours() const noexcept(std::is_arithmetic_v<time_type>) { return std::chrono::duration_cast<std::chrono::duration<time_type, std::ratio<3600>>>(duration).count(); }
        [[nodiscard]] constexpr time_type minutes() const noexcept(std::is_arithmetic_v<time_type>) { return std::chrono::duration_cast<std::chrono::duration<time_type, std::ratio<60>>>(duration).count(); }
        [[nodiscard]] constexpr time_type seconds() const noexcept(std::is_arithmetic_v<time_type>) { return duration.count(); }
        [[nodiscard]] constexpr time_type milliseconds() const noexcept(std::is_arithmetic_v<time_type>) { return std::chrono::duration_cast<std::chrono::duration<time_type, std::milli>>(duration).count(); }
        [[nodiscard]] constexpr time_type microseconds() const noexcept(std::is_arithmetic_v<time_type>) { return std::chrono::duration_cast<std::chrono::duration<time_type, std::micro>>(duration).count(); }
        [[nodiscard]] constexpr time_type nanoseconds() const noexcept(std::is_arithmetic_v<time_type>) { return std::chrono::duration_cast<std::chrono::duration<time_type, std::nano>>(duration).count(); }

        [[nodiscard]] constexpr operator duration_type&() noexcept { return duration; }
        [[nodiscard]] constexpr operator const duration_type&() const noexcept { return duration; }
        [[nodiscard]] constexpr operator time_type() const noexcept { return duration.count(); }

        constexpr time_span& operator++() noexcept(std::is_arithmetic_v<time_type>) /* strengthened */
        {
            ++duration;
            return *this;
        }

        constexpr time_span operator++(int) noexcept(std::is_arithmetic_v<time_type>) /* strengthened */
        {
            return time_span(duration++);
        }

        constexpr time_span& operator--() noexcept(std::is_arithmetic_v<time_type>) /* strengthened */
        {
            --duration;
            return *this;
        }

        constexpr time_span operator--(int) noexcept(std::is_arithmetic_v<time_type>) /* strengthened */
        {
            return time_span(duration--);
        }

        template<time_duration_rep other_time>
        constexpr time_span& operator+=(const time_span<other_time>& rhs) noexcept(std::is_arithmetic_v<time_type>) /* strengthened */
        {
            duration += std::chrono::duration_cast<duration_type>(rhs.duration);
            return *this;
        }

        template<time_duration_rep other_time>
        constexpr time_span& operator-=(const time_span<other_time>& rhs) noexcept(std::is_arithmetic_v<time_type>) /* strengthened */
        {
            duration -= std::chrono::duration_cast<duration_type>(rhs.duration);
            return *this;
        }

        template<time_duration_rep other_time>
        constexpr time_span& operator*=(const time_span<other_time>& rhs) noexcept(std::is_arithmetic_v<time_type>) /* strengthened */
        {
            duration *= std::chrono::duration_cast<duration_type>(rhs.duration);
            return *this;
        }

        template<time_duration_rep other_time>
        constexpr time_span& operator/=(const time_span<other_time>& rhs) noexcept(std::is_arithmetic_v<time_type>) /* strengthened */
        {
            duration /= std::chrono::duration_cast<duration_type>(rhs.duration);
            return *this;
        }

        template<time_duration_rep other_time>
        constexpr time_span& operator%=(const time_span<other_time>& rhs) noexcept(std::is_arithmetic_v<time_type>) /* strengthened */
        {
            duration %= std::chrono::duration_cast<duration_type>(rhs.duration);
            return *this;
        }

        // get zero value
        [[nodiscard]] static constexpr time_span zero() noexcept { return time_span(0); }

        // get minimum value
        [[nodiscard]] static constexpr time_span min() noexcept { return time_span((std::chrono::duration_values<time_type>::min)()); }

        // get maximum value
        [[nodiscard]] static constexpr time_span max() noexcept { return time_span((std::chrono::duration_values<time_type>::max)()); }
    };

    using span = time_span<>;

    template<rsl::time_duration_rep precisionLHS, rsl::time_duration_rep precisionRHS>
    using common_time_span = time_span<std::common_type_t<precisionLHS, precisionRHS>>;
}

template<rsl::time_duration_rep precisionLHS, rsl::time_duration_rep precisionRHS>
[[nodiscard]] constexpr auto operator+(const rsl::time_span<precisionLHS>& lhs, const rsl::time_span<precisionRHS>& rhs)
noexcept(std::is_arithmetic_v<precisionLHS>&& std::is_arithmetic_v<precisionRHS>) /* strengthened */
{
    return rsl::common_time_span<precisionLHS, precisionRHS>(lhs.duration + rhs.duration);
}

template<rsl::time_duration_rep precisionLHS, rsl::time_duration_rep precisionRHS>
[[nodiscard]] constexpr auto operator-(const rsl::time_span<precisionLHS>& lhs, const rsl::time_span<precisionRHS>& rhs)
noexcept(std::is_arithmetic_v<precisionLHS>&& std::is_arithmetic_v<precisionRHS>) /* strengthened */
{
    return rsl::common_time_span<precisionLHS, precisionRHS>(lhs.duration - rhs.duration);
}

template<rsl::time_duration_rep precisionLHS, rsl::time_duration_rep precisionRHS>
[[nodiscard]] constexpr auto operator*(const rsl::time_span<precisionLHS>& lhs, const rsl::time_span<precisionRHS>& rhs)
noexcept(std::is_arithmetic_v<precisionLHS>&& std::is_arithmetic_v<precisionRHS>) /* strengthened */
{
    return rsl::common_time_span<precisionLHS, precisionRHS>(lhs.duration * rhs.duration);
}

template<rsl::time_duration_rep precisionLHS, rsl::time_duration_rep precisionRHS>
[[nodiscard]] constexpr auto operator/(const rsl::time_span<precisionLHS>& lhs, const rsl::time_span<precisionRHS>& rhs)
noexcept(std::is_arithmetic_v<precisionLHS>&& std::is_arithmetic_v<precisionRHS>) /* strengthened */
{
    return rsl::common_time_span<precisionLHS, precisionRHS>(lhs.duration / rhs.duration);
}

template<rsl::time_duration_rep precisionLHS, rsl::time_duration_rep precisionRHS>
[[nodiscard]] constexpr auto operator%(const rsl::time_span<precisionLHS>& lhs, const rsl::time_span<precisionRHS>& rhs)
noexcept(std::is_arithmetic_v<precisionLHS>&& std::is_arithmetic_v<precisionRHS>) /* strengthened */
{
    return rsl::common_time_span<precisionLHS, precisionRHS>(lhs.duration % rhs.duration);
}

template<rsl::time_duration_rep precisionLHS, rsl::time_duration_rep precisionRHS>
[[nodiscard]] constexpr auto operator<=>(const rsl::time_span<precisionLHS>& lhs, const rsl::time_span<precisionRHS>& rhs)
noexcept(std::is_arithmetic_v<precisionLHS>&& std::is_arithmetic_v<precisionRHS>) /* strengthened */
{
    return lhs.duration.count() <=> rhs.duration.count();
}

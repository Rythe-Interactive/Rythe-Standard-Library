#pragma once
#include <chrono>

#include "../util/concepts.hpp"
#include "../util/primitives.hpp"

namespace rsl::time
{
	template <typename Rep>
	concept duration_rep = !is_specialization_v<Rep, std::chrono::duration>;

	template <duration_rep precision = nano_seconds>
	using nano_seconds_duration = std::chrono::duration<precision, std::nano>;

	template <duration_rep precision = micro_seconds>
	using micro_seconds_duration = std::chrono::duration<precision, std::micro>;

	template <duration_rep precision = milli_seconds>
	using milli_seconds_duration = std::chrono::duration<precision, std::milli>;

	template <duration_rep precision = seconds>
	using seconds_duration = std::chrono::duration<precision, std::ratio<1>>;

	template <duration_rep precision = minutes>
	using minutes_duration = std::chrono::duration<precision, std::ratio<60>>;

	template <duration_rep precision = hours>
	using hours_duration =
		std::chrono::duration<precision, std::ratio_multiply<std::ratio<60>, std::chrono::minutes::period>>;

	template <duration_rep precision = days>
	using days_duration =
		std::chrono::duration<precision, std::ratio_multiply<std::ratio<24>, std::chrono::hours::period>>;

	template <duration_rep precision = time32>
	struct span
	{
		using time_type = precision;
		using duration_type = std::chrono::duration<time_type>;

		duration_type duration{duration_type::zero()};

		constexpr span() noexcept = default;

		constexpr span(time_type other) noexcept
			: duration(other)
		{
		}

		template <duration_rep other_time, ratio_type ratio>
		constexpr explicit span(const std::chrono::duration<other_time, ratio>& other) noexcept
			: duration(std::chrono::duration_cast<duration_type>(other))
		{
		}

		template <duration_rep other_time>
			requires(!std::same_as<other_time, time_type>)
		constexpr explicit span(const span<other_time>& other) noexcept
			: duration(other.duration)
		{
		}

		template <duration_rep other_time, ratio_type ratio>
		constexpr explicit span(std::chrono::duration<other_time, ratio>&& other) noexcept
			: duration(std::chrono::duration_cast<duration_type>(other))
		{
		}

		template <duration_rep other_time>
			requires(!std::same_as<other_time, time_type>)
		constexpr explicit span(span<other_time>&& other) noexcept
			: duration(std::move(other.duration))
		{
		}

		template <duration_rep other_time>
			requires(!std::same_as<other_time, time_type>)
		constexpr explicit span(other_time other) noexcept
			: duration(other)
		{
		}

		template <duration_rep other_time>
		constexpr span<other_time> cast() const noexcept
		{
			return {*this};
		}

		template <duration_rep T>
		[[nodiscard]] [[rythe_always_inline]] constexpr T hours() const
			noexcept(rsl::is_arithmetic_v<time_type> && rsl::is_arithmetic_v<T>)
		{
			return std::chrono::duration_cast<hours_duration<T>>(duration).count();
		}
		template <duration_rep T>
		[[nodiscard]] [[rythe_always_inline]] constexpr T minutes() const
			noexcept(rsl::is_arithmetic_v<time_type> && rsl::is_arithmetic_v<T>)
		{
			return std::chrono::duration_cast<minutes_duration<T>>(duration).count();
		}
		template <duration_rep T>
		[[nodiscard]] [[rythe_always_inline]] constexpr T seconds() const
			noexcept(rsl::is_arithmetic_v<time_type> && rsl::is_arithmetic_v<T>)
		{
			return std::chrono::duration_cast<seconds_duration<T>>(duration).count();
		}
		template <duration_rep T>
		[[nodiscard]] [[rythe_always_inline]] constexpr T milliseconds() const
			noexcept(rsl::is_arithmetic_v<time_type> && rsl::is_arithmetic_v<T>)
		{
			return std::chrono::duration_cast<milli_seconds_duration<T>>(duration).count();
		}
		template <duration_rep T>
		[[nodiscard]] [[rythe_always_inline]] constexpr T microseconds() const
			noexcept(rsl::is_arithmetic_v<time_type> && rsl::is_arithmetic_v<T>)
		{
			return std::chrono::duration_cast<micro_seconds_duration<T>>(duration).count();
		}
		template <duration_rep T>
		[[nodiscard]] [[rythe_always_inline]] constexpr T nanoseconds() const
			noexcept(rsl::is_arithmetic_v<time_type> && rsl::is_arithmetic_v<T>)
		{
			return std::chrono::duration_cast<nano_seconds_duration<T>>(duration).count();
		}

		[[nodiscard]] [[rythe_always_inline]] constexpr time_type hours() const
			noexcept(rsl::is_arithmetic_v<time_type>)
		{
			return std::chrono::duration_cast<hours_duration<time_type>>(duration).count();
		}
		[[nodiscard]] [[rythe_always_inline]] constexpr time_type minutes() const
			noexcept(rsl::is_arithmetic_v<time_type>)
		{
			return std::chrono::duration_cast<minutes_duration<time_type>>(duration).count();
		}
		[[nodiscard]] [[rythe_always_inline]] constexpr time_type seconds() const
			noexcept(rsl::is_arithmetic_v<time_type>)
		{
			return duration.count();
		}
		[[nodiscard]] [[rythe_always_inline]] constexpr time_type milliseconds() const
			noexcept(rsl::is_arithmetic_v<time_type>)
		{
			return std::chrono::duration_cast<milli_seconds_duration<time_type>>(duration).count();
		}
		[[nodiscard]] [[rythe_always_inline]] constexpr time_type microseconds() const
			noexcept(rsl::is_arithmetic_v<time_type>)
		{
			return std::chrono::duration_cast<micro_seconds_duration<time_type>>(duration).count();
		}
		[[nodiscard]] [[rythe_always_inline]] constexpr time_type nanoseconds() const
			noexcept(rsl::is_arithmetic_v<time_type>)
		{
			return std::chrono::duration_cast<nano_seconds_duration<time_type>>(duration).count();
		}

		[[nodiscard]] [[rythe_always_inline]] constexpr operator duration_type&() noexcept { return duration; }
		[[nodiscard]] [[rythe_always_inline]] constexpr operator const duration_type&() const noexcept
		{
			return duration;
		}
		[[nodiscard]] [[rythe_always_inline]] constexpr operator time_type() const noexcept { return duration.count(); }

		[[rythe_always_inline]] constexpr span& operator++()
			noexcept(rsl::is_arithmetic_v<time_type>) /* strengthened */
		{
			++duration;
			return *this;
		}
		[[rythe_always_inline]] constexpr span operator++(int)
			noexcept(rsl::is_arithmetic_v<time_type>) /* strengthened */
		{
			return span(duration++);
		}
		[[rythe_always_inline]] constexpr span& operator--()
			noexcept(rsl::is_arithmetic_v<time_type>) /* strengthened */
		{
			--duration;
			return *this;
		}
		[[rythe_always_inline]] constexpr span operator--(int)
			noexcept(rsl::is_arithmetic_v<time_type>) /* strengthened */
		{
			return span(duration--);
		}

		template <duration_rep other_time>
		[[rythe_always_inline]] constexpr span& operator+=(const span<other_time>& rhs)
			noexcept(rsl::is_arithmetic_v<time_type>) /* strengthened */
		{
			duration += std::chrono::duration_cast<duration_type>(rhs.duration);
			return *this;
		}

		template <duration_rep other_time>
		[[rythe_always_inline]] constexpr span& operator-=(const span<other_time>& rhs)
			noexcept(rsl::is_arithmetic_v<time_type>) /* strengthened */
		{
			duration -= std::chrono::duration_cast<duration_type>(rhs.duration);
			return *this;
		}

		template <duration_rep other_time>
		[[rythe_always_inline]] constexpr span& operator*=(const span<other_time>& rhs)
			noexcept(rsl::is_arithmetic_v<time_type>) /* strengthened */
		{
			duration *= std::chrono::duration_cast<duration_type>(rhs.duration);
			return *this;
		}

		template <duration_rep other_time>
		[[rythe_always_inline]] constexpr span& operator/=(const span<other_time>& rhs)
			noexcept(rsl::is_arithmetic_v<time_type>) /* strengthened */
		{
			duration /= std::chrono::duration_cast<duration_type>(rhs.duration);
			return *this;
		}

		template <duration_rep other_time>
		[[rythe_always_inline]] constexpr span& operator%=(const span<other_time>& rhs)
			noexcept(rsl::is_arithmetic_v<time_type>) /* strengthened */
		{
			duration %= std::chrono::duration_cast<duration_type>(rhs.duration);
			return *this;
		}

		[[nodiscard]] [[rythe_always_inline]] static constexpr span zero() noexcept { return span(0); }
		[[nodiscard]] [[rythe_always_inline]] static constexpr span min() noexcept
		{
			return span(std::chrono::duration_values<time_type>::min());
		}
		[[nodiscard]] [[rythe_always_inline]] static constexpr span max() noexcept
		{
			return span(std::chrono::duration_values<time_type>::max());
		}
	};

	template <duration_rep precisionLHS, duration_rep precisionRHS>
	using common_span = time::span<std::common_type_t<precisionLHS, precisionRHS>>;

	using span32 = span<time32>;
	using span64 = span<time64>;
} // namespace rsl::time

template <rsl::time::duration_rep precisionLHS, rsl::time::duration_rep precisionRHS>
[[nodiscard]] [[rythe_always_inline]] constexpr auto
operator+(const rsl::time::span<precisionLHS>& lhs, const rsl::time::span<precisionRHS>& rhs)
	noexcept(rsl::is_arithmetic_v<precisionLHS> && rsl::is_arithmetic_v<precisionRHS>) /* strengthened */
{
	return rsl::time::common_span<precisionLHS, precisionRHS>(lhs.duration + rhs.duration);
}

template <rsl::time::duration_rep precisionLHS, rsl::time::duration_rep precisionRHS>
[[nodiscard]] [[rythe_always_inline]] constexpr auto
operator-(const rsl::time::span<precisionLHS>& lhs, const rsl::time::span<precisionRHS>& rhs)
	noexcept(rsl::is_arithmetic_v<precisionLHS> && rsl::is_arithmetic_v<precisionRHS>) /* strengthened */
{
	return rsl::time::common_span<precisionLHS, precisionRHS>(lhs.duration - rhs.duration);
}

template <rsl::time::duration_rep precisionLHS, rsl::time::duration_rep precisionRHS>
[[nodiscard]] [[rythe_always_inline]] constexpr auto
operator*(const rsl::time::span<precisionLHS>& lhs, const rsl::time::span<precisionRHS>& rhs)
	noexcept(rsl::is_arithmetic_v<precisionLHS> && rsl::is_arithmetic_v<precisionRHS>) /* strengthened */
{
	return rsl::time::common_span<precisionLHS, precisionRHS>(lhs.duration * rhs.duration);
}

template <rsl::time::duration_rep precisionLHS, rsl::time::duration_rep precisionRHS>
[[nodiscard]] [[rythe_always_inline]] constexpr auto
operator/(const rsl::time::span<precisionLHS>& lhs, const rsl::time::span<precisionRHS>& rhs)
	noexcept(rsl::is_arithmetic_v<precisionLHS> && rsl::is_arithmetic_v<precisionRHS>) /* strengthened */
{
	return rsl::time::common_span<precisionLHS, precisionRHS>(lhs.duration / rhs.duration);
}

template <rsl::time::duration_rep precisionLHS, rsl::time::duration_rep precisionRHS>
[[nodiscard]] [[rythe_always_inline]] constexpr auto
operator%(const rsl::time::span<precisionLHS>& lhs, const rsl::time::span<precisionRHS>& rhs)
	noexcept(rsl::is_arithmetic_v<precisionLHS> && rsl::is_arithmetic_v<precisionRHS>) /* strengthened */
{
	return rsl::time::common_span<precisionLHS, precisionRHS>(lhs.duration % rhs.duration);
}

template <rsl::time::duration_rep precisionLHS, rsl::time::duration_rep precisionRHS>
[[nodiscard]] [[rythe_always_inline]] constexpr auto
operator<=>(const rsl::time::span<precisionLHS>& lhs, const rsl::time::span<precisionRHS>& rhs)
	noexcept(rsl::is_arithmetic_v<precisionLHS> && rsl::is_arithmetic_v<precisionRHS>) /* strengthened */
{
	return lhs.duration.count() <=> rhs.duration.count();
}

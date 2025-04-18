#pragma once
#include <memory>

#include "../../util/primitives.hpp"

#include "random_number_engine.hpp"

namespace rsl::math
{
	class RandomNumberGenerator
	{
		static std::unique_ptr<random_number_engine_base> engine;

	public:
		template <
			typename T, typename... Args,
			std::enable_if_t<std::is_base_of_v<random_number_engine_base, T>, bool> = true>
		static void setRNGEngine(Args&&... args);

		static void setRNGEngine(std::unique_ptr<random_number_engine_base>&& newEngine) noexcept;

		static void setSeed(size_type seed) noexcept;

		static void setSeed(std::seed_seq& seeds) noexcept;

		static void discard(size_type n) noexcept;

		template <typename Integer = int32>
		[[nodiscard]] static Integer generateRandomInteger() noexcept;

		template <typename Float = float32>
		[[nodiscard]] static Float generateRandomFloat() noexcept;

		[[nodiscard]] static uint_max generate() noexcept;
	};
} // namespace rsl::math

#include "randomnumbergenerator.inl"

#include "randomnumbergenerator.hpp"

namespace rsl::math
{
	std::unique_ptr<random_number_engine_base> RandomNumberGenerator::engine =
		::std::make_unique<rythe_default_engine>();

	void RandomNumberGenerator::setRNGEngine(std::unique_ptr<random_number_engine_base>&& newEngine) noexcept
	{
		engine = std::move(newEngine);
	}

	void RandomNumberGenerator::setSeed(size_type seed) noexcept
	{
		engine->seed(seed);
	}

	void RandomNumberGenerator::setSeed(std::seed_seq& seeds) noexcept
	{
		engine->seed(seeds);
	}

	void RandomNumberGenerator::discard(size_type n) noexcept
	{
		engine->discard(n);
	}

	[[nodiscard]] uint_max RandomNumberGenerator::generate() noexcept
	{
#if defined(LEGION_VALIDATE)
		if (!engine)
		{
			return 0; // should report an error with improved error system down the line.
		}
#endif
		return engine->generate();
	}
} // namespace rsl::math

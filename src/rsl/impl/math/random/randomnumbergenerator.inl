#pragma once
#include "randomnumbergenerator.hpp"

namespace rsl::math
{
	template <typename T, typename... Args, std::enable_if_t<std::is_base_of_v<random_number_engine_base, T>, bool>>
	inline void random_number_generator::set_rng_engine(Args&&... args)
	{
		m_engine = unique_object<T>(forward<Args>(args)...);
	}

	template <typename Integer>
	[[nodiscard]] inline Integer random_number_generator::generate_random_integer() noexcept
	{
		if constexpr (::std::is_signed_v<Integer>)
		{
			using Unsigned = ::std::make_unsigned_t<Integer>;
			static_assert(
				sizeof(Unsigned) == sizeof(Integer),
				"Somehow the size of the unsigned version is not the same as the original integer type?"
			);

			Unsigned raw = static_cast<Unsigned>(generate() % static_cast<size_type>(limits<Unsigned>::max));
			return *reinterpret_cast<Integer*>(&raw);
		}
		else
		{
			return static_cast<Integer>(generate() % static_cast<size_type>(limits<Integer>::max));
		}
	}

	template <typename Float>
	[[nodiscard]] inline Float random_number_generator::generate_random_float() noexcept
	{
		auto integer = generate_random_integer<uint_max>();
		return integer / static_cast<Float>(::std::numeric_limits<uint_max>::max());
	}
} // namespace rsl::math

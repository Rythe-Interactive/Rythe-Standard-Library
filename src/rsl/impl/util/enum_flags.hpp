#pragma once

#include "concepts.hpp"

namespace rsl
{
	namespace enum_flags
	{
		template <enum_type Enum>
		[[rythe_always_inline]] constexpr Enum make_zero()
		{
			return static_cast<Enum>(0);
		}

		template <enum_type Enum, same_as<Enum>... T>
		[[rythe_always_inline]] constexpr Enum combine_flags(Enum first, Enum second, T... rest);

		template <enum_type T>
		[[rythe_always_inline]] constexpr bool has_flag(T value, T flag)
		{
			using BaseType = std::underlying_type_t<T>;
			return (static_cast<BaseType>(value) & static_cast<BaseType>(flag)) != 0;
		}

		template <enum_type T>
		[[rythe_always_inline]] constexpr bool has_all_flags(T value, T flag)
		{
			using BaseType = std::underlying_type_t<T>;
			return (static_cast<BaseType>(value) & static_cast<BaseType>(flag)) == static_cast<BaseType>(flag);
		}

		template <enum_type Enum, std::same_as<Enum>... T>
		[[rythe_always_inline]] constexpr bool has_any_flags(Enum value, Enum first, Enum second, T... rest)
		{
			using BaseType = std::underlying_type_t<Enum>;
			Enum flag = combine_flags(first, second, rest...);
			return (static_cast<BaseType>(value) & static_cast<BaseType>(flag)) != 0;
		}

		template <enum_type T>
		[[rythe_always_inline]] constexpr T invert_flag(T flag)
		{
			using BaseType = std::underlying_type_t<T>;
			return static_cast<T>(~static_cast<BaseType>(flag));
		}

		template <enum_type Enum, std::same_as<Enum>... T>
		[[rythe_always_inline]] constexpr Enum unflag(Enum value, Enum first, T... rest)
		{
			using BaseType = std::underlying_type_t<Enum>;
			return static_cast<Enum>(
				static_cast<BaseType>(value) & ~((static_cast<BaseType>(first) | ... | static_cast<BaseType>(rest)))
			);
		}

		template <enum_type Enum, std::same_as<Enum>... T>
		[[rythe_always_inline]] constexpr Enum combine_flags(Enum first, Enum second, T... rest)
		{
			using BaseType = std::underlying_type_t<Enum>;
			return static_cast<Enum>(
				static_cast<BaseType>(first) | (static_cast<BaseType>(rest) | ... | static_cast<BaseType>(second))
			);
		}

		template <enum_type Enum, std::same_as<Enum>... T>
		[[rythe_always_inline]] constexpr Enum exclude_flags(Enum first, Enum second, T... rest)
		{
			using BaseType = std::underlying_type_t<Enum>;
			return static_cast<Enum>(
				static_cast<BaseType>(first) & (static_cast<BaseType>(rest) & ... & static_cast<BaseType>(second))
			);
		}

		template <enum_type Enum, std::same_as<Enum>... T>
		[[rythe_always_inline]] constexpr Enum toggle_flags(Enum first, Enum second, T... rest)
		{
			using BaseType = std::underlying_type_t<Enum>;
			return static_cast<Enum>(
				static_cast<BaseType>(first) ^ (static_cast<BaseType>(second) ^ ... ^ static_cast<BaseType>(rest))
			);
		}

		template <enum_type T>
		[[rythe_always_inline]] constexpr T set_flag(T value, T flag, bool isSet = true)
		{
			if (isSet != has_flag(value, flag))
			{
				return toggle_flags(value, flag);
			}

			return value;
		}
	} // namespace enum_flags
} // namespace rsl

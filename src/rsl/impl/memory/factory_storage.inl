#pragma once
#include "factory_storage.hpp"

namespace rsl
{
	template <factory_type Factory>
	inline constexpr factory_storage<Factory>::factory_storage(const Factory& factory)
		noexcept(is_nothrow_copy_constructible_v<Factory>)
		: value(factory)
	{
	}

	template <factory_type Factory>
	inline constexpr factory_storage<Factory>::factory_storage(Factory&& factory)
		noexcept(is_nothrow_move_constructible_v<Factory>)
		: value(std::move(factory))
	{
	}

	template <typename T>
	inline constexpr factory_storage<default_factory<T>>::factory_storage(const default_factory<T>& factory) noexcept
		: value(factory)
	{
	}
} // namespace rsl

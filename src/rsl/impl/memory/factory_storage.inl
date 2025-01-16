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

	template <factory_type Factory>
	template <factory_type Other>
	inline constexpr factory_storage<Factory>::factory_storage(const factory_storage<Other>& other)
		noexcept(is_nothrow_constructible_v<Factory, const Other&>)
		: value(other.value)
	{
	}

	template <factory_type Factory>
	template <factory_type Other>
	inline constexpr factory_storage<Factory>::factory_storage(factory_storage<Other>&& other)
		noexcept(is_nothrow_constructible_v<Factory, Other&&>)
		: value(std::move(other.value))
	{
	}

	inline constexpr factory_storage<polymorphic_factory>::factory_storage(polymorphic_factory& factory) noexcept
		: value(&factory)
	{
	}
} // namespace rsl

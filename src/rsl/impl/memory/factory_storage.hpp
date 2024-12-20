#pragma once
#include "factory.hpp"

namespace rsl
{
	template <factory_type Factory>
	struct factory_storage final
	{
		[[rythe_always_inline]] constexpr factory_storage() noexcept(is_nothrow_constructible_v<Factory>) = default;

		[[rythe_always_inline]] constexpr factory_storage(const Factory& factory)
			noexcept(is_nothrow_copy_constructible_v<Factory>);
		[[rythe_always_inline]] constexpr factory_storage(Factory&& factory)
			noexcept(is_nothrow_move_constructible_v<Factory>);

		template <factory_type Other>
		[[rythe_always_inline]] constexpr factory_storage(const factory_storage<Other>& other)
			noexcept(is_nothrow_constructible_v<Factory, const Other&>);
		template <factory_type Other>
		[[rythe_always_inline]] constexpr factory_storage(factory_storage<Other>&& other)
			noexcept(is_nothrow_constructible_v<Factory, Other&&>);

		[[rythe_always_inline]] constexpr Factory& operator*() noexcept { return value; }
		[[rythe_always_inline]] constexpr const Factory& operator*() const noexcept { return value; }
		[[rythe_always_inline]] constexpr Factory* operator->() noexcept { return &value; }
		[[rythe_always_inline]] constexpr const Factory* operator->() const noexcept { return &value; }

		Factory value;
	};
} // namespace rsl

#include "factory_storage.inl"

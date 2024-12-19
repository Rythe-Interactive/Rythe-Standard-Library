#pragma once
#include "memory.hpp"

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

		[[rythe_always_inline]] constexpr Factory& operator*() noexcept { return value; }
		[[rythe_always_inline]] constexpr const Factory& operator*() const noexcept { return value; }
		[[rythe_always_inline]] constexpr Factory* operator->() noexcept { return &value; }
		[[rythe_always_inline]] constexpr const Factory* operator->() const noexcept { return &value; }

		Factory value;
	};

	template <typename T>
	struct factory_storage<default_factory<T>> final
	{
		[[rythe_always_inline]] factory_storage() noexcept = default;
		[[rythe_always_inline]] constexpr factory_storage(const factory_storage&) noexcept = default;
		[[rythe_always_inline]] constexpr factory_storage& operator=(const factory_storage&) noexcept = default;

		[[rythe_always_inline]] constexpr factory_storage(const default_factory<T>& factory) noexcept;

		[[rythe_always_inline]] constexpr default_factory<T>& operator*() noexcept { return *value; }
		[[rythe_always_inline]] constexpr const default_factory<T>& operator*() const noexcept { return *value; }
		[[rythe_always_inline]] constexpr default_factory<T>* operator->() noexcept { return value; }
		[[rythe_always_inline]] constexpr const default_factory<T>* operator->() const noexcept { return value; }

		default_factory<T> value;
	};
} // namespace rsl

#include "factory_storage.inl"

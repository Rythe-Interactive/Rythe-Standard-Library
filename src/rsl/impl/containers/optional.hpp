#pragma once

#include "../memory/factory_storage.hpp"
#include "../util/type_traits.hpp"

namespace rsl
{
	/// Will either be T with all decoration signals applied, or void in the case T is void.
	template <typename T, typename... DecorationSignals>
	struct optional_param
	{
		using type = decorate_type<T, DecorationSignals...>::type;
	};

	template <typename... DecorationSignals>
	struct optional_param<void, DecorationSignals...>
	{
		using type = void;
	};

	template <typename T, typename... DecorationSignals>
	using optional_param_t = optional_param<T, DecorationSignals...>::type;

	/// Empty struct when T is void. When empty the size will be 1 byte, otherwise the size of T.
	template <typename T>
	struct optional_storage
	{
		constexpr static bool holds_value = true;

		using value_type = T;
		using ref_type = add_lval_ref_t<T>;
		using const_ref_type = decorate_type_t<T, const_signal, lval_ref_signal>;
		using ptr_type = add_pointer_t<T>;
		using const_ptr_type = decorate_type_t<T, const_signal, pointer_signal>;

		T value;

		const_ref_type operator*() const noexcept { return &value; }
		ref_type operator*() noexcept { return &value; }
		const_ptr_type operator->() const noexcept { return &value; }
		ptr_type operator->() noexcept { return &value; }
	};

	template <>
	struct optional_storage<void>
	{
		constexpr static bool holds_value = false;
	};

	template <bool Condition, typename T>
	using conditional_storage = conditional<Condition, optional_storage<T>, optional_storage<void>>::type;

	template <typename T, factory_type Factory = default_factory<T>>
	class optional
	{
	public:
		using value_type = T;
		using factory_storage_type = factory_storage<Factory>;
		using factory_t = Factory;

		[[rythe_always_inline]] constexpr optional() noexcept;
		[[rythe_always_inline]] constexpr optional(nullptr_type) noexcept;
		[[rythe_always_inline]] constexpr optional(const optional& other
		) noexcept(is_nothrow_copy_constructible_v<factory_storage_type> && is_nothrow_copy_constructible_v<value_type>);
		[[rythe_always_inline]] constexpr optional(optional&& other
		) noexcept(is_nothrow_move_constructible_v<factory_storage_type> && is_nothrow_move_constructible_v<value_type>);
		[[rythe_always_inline]] explicit constexpr optional(const factory_storage_type& factoryStorage)
			noexcept(is_nothrow_copy_constructible_v<factory_storage_type>);

		template <typename... Args>
		[[rythe_always_inline]] constexpr optional(in_place_signal_type, Args&&... args)
			noexcept(is_nothrow_constructible_v<value_type, Args...>);
		template <typename... Args>
		[[rythe_always_inline]] constexpr optional(
			const factory_storage_type& factoryStorage, in_place_signal_type, Args&&... args
		) noexcept(is_nothrow_constructible_v<value_type, Args...>);

		[[rythe_always_inline]] constexpr optional(const value_type& value)
			noexcept(is_nothrow_copy_constructible_v<value_type>);
		[[rythe_always_inline]] constexpr optional(const factory_storage_type& factoryStorage, const value_type& value)
			noexcept(is_nothrow_copy_constructible_v<value_type>);

		[[rythe_always_inline]] constexpr optional(value_type&& value)
			noexcept(is_nothrow_move_constructible_v<value_type>);
		[[rythe_always_inline]] constexpr optional(const factory_storage_type& factoryStorage, value_type&& value)
			noexcept(is_nothrow_move_constructible_v<value_type>);

		[[rythe_always_inline]] constexpr ~optional() noexcept;

		[[rythe_always_inline]] constexpr optional& operator=(nullptr_type) noexcept;
		[[rythe_always_inline]] constexpr optional& operator=(const optional& other)
			noexcept(is_nothrow_copy_assignable_v<factory_storage_type> && is_nothrow_copy_constructible_v<value_type>);
		[[rythe_always_inline]] constexpr optional& operator=(optional&& other)
			noexcept(is_nothrow_move_assignable_v<factory_storage_type> && is_nothrow_move_constructible_v<value_type>);
		[[rythe_always_inline]] constexpr optional& operator=(const value_type& value)
			noexcept(is_nothrow_copy_constructible_v<value_type>);
		[[rythe_always_inline]] constexpr optional& operator=(value_type&& value)
			noexcept(is_nothrow_move_constructible_v<value_type>);

		[[nodiscard]] [[rythe_always_inline]] constexpr value_type* operator->() noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const value_type* operator->() const noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr value_type& operator*() & noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const value_type& operator*() const& noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr value_type&& operator*() && noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const value_type&& operator*() const&& noexcept;

		[[nodiscard]] [[rythe_always_inline]] constexpr value_type& value() & noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const value_type& value() const& noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr value_type&& value() && noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const value_type&& value() const&& noexcept;

		[[nodiscard]] [[rythe_always_inline]] constexpr bool holds_value() noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr operator bool() noexcept;

		template <typename... Args>
		[[rythe_always_inline]] constexpr value_type& emplace(Args&&... args)
			noexcept(is_nothrow_constructible_v<value_type, Args...>);

		[[rythe_always_inline]] constexpr void reset() noexcept;

	private:
		factory_storage_type m_factory;
		bool m_hasValue;

		union
		{
			value_type m_value;
			byte m_dummy;
		};
	};
} // namespace rsl

#include "optional.inl"

#pragma once

#include "../util/type_util.hpp"

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
    template<typename T>
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

    template<>
    struct optional_storage<void>
	{
		constexpr static bool holds_value = false;
    };
}

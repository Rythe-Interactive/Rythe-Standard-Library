#pragma once
#include <concepts>

#include "common.hpp"

namespace rsl
{
	template <class T>
	concept enum_type = std::is_enum_v<T>;

	template <typename Func>
	concept invocable_undetermined = is_invocable_any_v<Func>;

	template <typename Func>
	concept function_ptr = is_function_ptr_v<Func>;

	template <typename Func>
	concept functor = is_functor_v<Func>;

	template <typename Func>
	concept member_function_ptr = std::is_member_function_pointer_v<Func>;

	template <typename Type>
	concept ratio_type = is_ratio_v<Type>;

	template <typename Type, template <typename...> typename Template>
	concept specialization_of = is_specialization_v<Type, Template>;

    template<bool value>
	concept invert = !value;

    template<typename T>
	concept arithmetic_type = is_arithmetic_v<T>;

	template <typename T1, typename T2>
	concept same_as = ::rsl::is_same_v<T1, T2>;

	template <typename T1, typename T2>
	concept not_same_as = !::rsl::is_same_v<T1, T2>;
} // namespace rsl

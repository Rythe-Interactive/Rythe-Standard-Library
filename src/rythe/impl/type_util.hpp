#pragma once
#include <utility>

#include "../primitives"

namespace rsl {
    template<template<typename...>typename T, typename U, size_type I, typename... Args>
    struct make_sequence : make_sequence<T, U, I - 1, Args..., U> {};

    template<template<typename...>typename T, typename U, typename... Args>
    struct make_sequence<T, U, 0, Args...> {
        using type = T<Args...>;
    };

    template<template<typename...>typename T, typename U, size_type I, typename... Args>
    using make_sequence_t = typename make_sequence<T, U, I, Args...>::type;

    template<typename... Types>
    struct type_sequence {
        using tuple_type = std::tuple<Types...>;
        constexpr static size_type size = sizeof...(Types);
    };

    namespace detail {
        struct any_type {
            template<class T>
            constexpr operator T(); // implicit conversion to any type.
        };

        template<typename Func, size_type... paramCounts>
        constexpr bool test_invocable_impl([[maybe_unused]]std::integer_sequence<size_type, paramCounts...> int_seq) {
            return ((make_sequence_t<std::is_invocable, any_type, paramCounts, Func>::value) || ...);
        }
    }

    template<typename Func, size_type maxParams = 32>
    struct is_invocable {
        static constexpr bool value = detail::test_invocable_impl<Func>(std::make_index_sequence<maxParams>{});
    };

    template<typename Func, size_type maxParams = 32>
    constexpr bool is_invocable_v = is_invocable<Func, maxParams>::value;

    template<typename Func, size_type maxParams = 32>
    constexpr bool is_function_ptr_v = (std::is_empty_v<Func> || std::is_pointer_v<Func>) && is_invocable_v<Func, maxParams>;

    template<typename Func, size_type maxParams = 32>
    constexpr bool is_functor_v = requires { &Func::operator(); } && is_invocable_v<Func, maxParams>;
}

#pragma once
#include <utility>
#include <ratio>
#include <concepts>

#include "../defines.hpp"
#include "primitives.hpp"

namespace rsl {
    template<template<typename...>typename T, typename U, size_type I, typename... Args>
    struct make_sequence : make_sequence<T, U, I - 1, Args..., U> {};

    template<template<typename...>typename T, typename U, typename... Args>
    struct make_sequence<T, U, 0, Args...> {
        using type = T<Args...>;
    };

    template<template<typename...>typename T, typename U, size_type I, typename... Args>
    using make_sequence_t = typename make_sequence<T, U, I, Args...>::type;

    template<typename Type, template<typename...> typename Template>
    constexpr bool is_specialization_v = false; // true if and only if Type is a specialization of Template
    template<template<typename...> typename Template, typename... Types>
    constexpr bool is_specialization_v<Template<Types...>, Template> = true;

    template<typename Type, template<typename...> typename Template>
    struct is_specialization : std::bool_constant<is_specialization_v<Type, Template>> {};

    template<typename... Types>
    struct type_sequence {
        using tuple_type = std::tuple<Types...>;
        constexpr static size_type size = sizeof...(Types);
    };

    template<typename T>
    concept type_sequence_c = is_specialization_v<T, type_sequence>;

    template<typename...>
    struct concat_sequence;

    template<typename... A, typename... B, type_sequence_c C, type_sequence_c... Rest>
    struct concat_sequence<type_sequence<A...>, type_sequence<B...>, C, Rest...> {
        using type = typename concat_sequence<type_sequence<A..., B...>, C, Rest...>::type;
    };

    template<typename... A, typename... B>
    struct concat_sequence<type_sequence<A...>, type_sequence<B...>> {
        using type = type_sequence<A..., B...>;
    };

    template<type_sequence_c... Sequences>
    using concat_sequence_t = typename concat_sequence<Sequences...>::type;

    template<type_sequence_c Sequence, typename T>
    struct type_sequence_contains;

    template<typename... Types, typename T>
    struct type_sequence_contains<type_sequence<Types...>, T> : std::disjunction<std::is_same<T, Types>...> {};

    template<type_sequence_c Sequence, typename T>
    constexpr bool type_sequence_contains_v = type_sequence_contains<Sequence, T>::value;

    namespace internal {
        struct any_type {
            template<typename T>
            constexpr operator T(); // implicit conversion to any type.
        };

        template<typename Func, size_type... paramCounts>
        constexpr bool test_invocable_impl([[maybe_unused]] std::integer_sequence<size_type, paramCounts...> int_seq) {
            return ((make_sequence_t<std::is_invocable, any_type, paramCounts, Func>::value) || ...);
        }
    }

    template<typename Func, size_type maxParams = 32>
    struct is_invocable {
        static constexpr bool value = internal::test_invocable_impl<Func>(std::make_index_sequence<maxParams>{});
    };

    template<typename Func, size_type maxParams = 32>
    constexpr bool is_invocable_v = is_invocable<Func, maxParams>::value;

    template<typename Func, size_type maxParams = 32>
    constexpr bool is_function_ptr_v = (std::is_empty_v<Func> || std::is_pointer_v<Func>) && is_invocable_v<Func, maxParams>;

    template<typename Func, size_type maxParams = 32>
    constexpr bool is_functor_v = requires { &Func::operator(); } && is_invocable_v<Func, maxParams>;

    template<typename Type>
    constexpr bool is_ratio_v = false; // test for ratio type

    template<int_max numerator, int_max denominator>
    constexpr bool is_ratio_v<std::ratio<numerator, denominator>> = true;

    template<typename Type>
    struct is_ratio : std::bool_constant<is_ratio_v<Type>> {};

#if defined(RYTHE_MSVC)

    template<typename T>
    constexpr std::string_view type_name() noexcept {
        std::string_view funcName(__FUNCSIG__);
        auto first = funcName.find_first_of(' ', funcName.find("type_name<") + 11) + 1;
        return funcName.substr(first, funcName.find_last_of('>') - first);
    }

#elif defined(RYTHE_GCC)

    template<typename T>
    constexpr std::string_view type_name() noexcept {
        std::string_view funcName(__PRETTY_FUNCTION__);
        auto first = funcName.find_first_not_of(' ', funcName.find_first_of('=') + 1);
        return funcName.substr(first, funcName.find_last_of(';') - first);
    }

#elif defined(RYTHE_CLANG)

    template<typename T>
    constexpr std::string_view type_name() noexcept {
        std::string_view funcName(__PRETTY_FUNCTION__);
        auto first = funcName.find_first_not_of(' ', funcName.find_first_of('=') + 1);
        return funcName.substr(first, funcName.find_last_of(']') - first);
    }

#endif

    template<typename T>
    constexpr std::string_view type_name(T&&) noexcept {
        return type_name<std::remove_cvref_t<T>>();
    }
}

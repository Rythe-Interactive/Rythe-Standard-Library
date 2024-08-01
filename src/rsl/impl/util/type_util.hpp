#pragma once
#include <concepts>
#include <ratio>
#include <utility>

#include "common.hpp"

#include "../defines.hpp"
#include "string_util.hpp"

namespace rsl
{
	// I'm choosing to go with the constraint option instead of SFINAE, the Microsoft STL has choosen different.
	// See https://github.com/microsoft/STL/issues/2620 and https://github.com/microsoft/STL/pull/2624 for further
	// context. Theoretically the constraint should be superior cross compilers, but bugs inside MSVC has in the past
	// prevented it from working propperly in all edge cases. I don't care about these edge cases nor do I care about
	// past versions of MSVC.
	template <typename T, typename... Args>
		requires requires(void* ptr, Args&&... args) { ::new (ptr) T(static_cast<Args&&>(args)...); }
	constexpr T* construct_at(T* const location, Args&&... args)
		noexcept(noexcept(::new(static_cast<void*>(location)) T(::rsl::forward<Args>(args)...)))
	{
		return ::new (static_cast<void*>(location)) T(::rsl::forward<Args>(args)...);
	}

	namespace internal
	{
		template <class NoThrowFwdIt, class NoThrowSentinel>
		constexpr void destroy_range(NoThrowFwdIt first, const NoThrowSentinel last) noexcept;

		template <class T>
		constexpr void destroy_in_place(T& val) noexcept
		{
			if constexpr (is_array_v<T>)
			{
				destroy_range(val, val + extent_v<T>);
			}
			else
			{
				val.~T();
			}
		}

		template <class NoThrowFwdIt, class NoThrowSentinel>
		constexpr void destroy_range(NoThrowFwdIt first, const NoThrowSentinel last) noexcept
		{
			// note that this is an optimization for debug mode codegen; in release mode the BE removes all of this
			if constexpr (!::std::is_trivially_destructible_v<::std::iter_value_t<NoThrowFwdIt>>)
			{
				for (; first != last; ++first) { destroy_in_place(*first); }
			}
		}
	} // namespace internal

	template <typename T, size_type Size>
	[[nodiscard]] constexpr T* begin(T (&arr)[Size]) noexcept;

	template <typename T, size_type Size>
	[[nodiscard]] constexpr T* end(T (&arr)[Size]) noexcept;

	template <typename T>
	constexpr void destroy_at(T* const location) noexcept
	{
		if constexpr (is_array_v<T>)
		{
			internal::destroy_range(::rsl::begin(*location), ::rsl::end(*location));
		}
		else
		{
			location->~T();
		}
	}

	template <template <typename...> typename T, typename U, size_type I, typename... Args>
	struct make_sequence : make_sequence<T, U, I - 1, Args..., U>
	{
	};

	template <template <typename...> typename T, typename U, typename... Args>
	struct make_sequence<T, U, 0, Args...>
	{
		using type = T<Args...>;
	};

	template <template <typename...> typename T, typename U, size_type I, typename... Args>
	using make_sequence_t = typename make_sequence<T, U, I, Args...>::type;

	template <typename Type, template <typename...> typename Template>
	constexpr bool is_specialization_v = false; // true if and only if Type is a specialization of Template

	template <template <typename...> typename Template, typename... Types>
	constexpr bool is_specialization_v<Template<Types...>, Template> = true;

	template <typename Type, template <typename...> typename Template>
	struct is_specialization : bool_constant<is_specialization_v<Type, Template>>
	{
	};

	template <typename... Types>
	struct type_sequence
	{
		using tuple_type = ::std::tuple<Types...>; // TODO: Make our own tuple type.
		constexpr static size_type size = sizeof...(Types);
	};

	template <typename T>
	concept type_sequence_c = is_specialization_v<T, type_sequence>;

	template <typename...>
	struct concat_sequence;

	template <typename... A, typename... B, type_sequence_c C, type_sequence_c... Rest>
	struct concat_sequence<type_sequence<A...>, type_sequence<B...>, C, Rest...>
	{
		using type = typename concat_sequence<type_sequence<A..., B...>, C, Rest...>::type;
	};

	template <typename... A, typename... B>
	struct concat_sequence<type_sequence<A...>, type_sequence<B...>>
	{
		using type = type_sequence<A..., B...>;
	};

	template <type_sequence_c... Sequences>
	using concat_sequence_t = typename concat_sequence<Sequences...>::type;

	template <type_sequence_c Sequence, typename T>
	struct type_sequence_contains;

	template <typename... Types, typename T>
	struct type_sequence_contains<type_sequence<Types...>, T> : disjunction<is_same<T, Types>...>
	{
	};

	template <type_sequence_c Sequence, typename T>
	constexpr bool type_sequence_contains_v = type_sequence_contains<Sequence, T>::value;

	template <typename T, T... Vals>
	struct integer_sequence
	{ // sequence of integer parameters
		static_assert(is_integral_v<T>, "integer_sequence<T, I...> requires T to be an integral type.");

		using value_type = T;

		[[nodiscard]] static constexpr size_type size() noexcept { return sizeof...(Vals); }
	};

	template <typename T, T Size>
	using make_integer_sequence = __make_integer_seq<integer_sequence, T, Size>;

	template <size_type... Vals>
	using index_sequence = integer_sequence<size_type, Vals...>;

	template <size_type Size>
	using make_index_sequence = make_integer_sequence<size_type, Size>;

	template <typename... Types>
	using index_sequence_for = make_index_sequence<sizeof...(Types)>;

	namespace internal
	{
		template <typename From, typename To, typename = void>
		struct invoke_convertible : false_type
		{
		};

		template <typename From, typename To>
		struct invoke_convertible<From, To, void_t<decltype(fake_copy_init<To>(returns_exactly<From>()))>> : true_type
		{
		};

		template <typename From, typename To>
		struct invoke_nothrow_convertible : bool_constant<noexcept(fake_copy_init<To>(returns_exactly<From>()))>
		{
		};

		template <typename ReturnType, bool Nothrow>
		struct invoke_traits
		{
			using type = ReturnType;
			using is_invocable = true_type;
			using is_nothrow_invocable = bool_constant<Nothrow>;

			template <typename RetType>
			using is_invocable_ret = bool_constant<disjunction_v<is_void<RetType>, invoke_convertible<type, RetType>>>;

			template <typename RetType>
			using is_nothrow_invocable_ret = bool_constant<conjunction_v<
				is_nothrow_invocable,
				disjunction<
					is_void<RetType>,
					conjunction<invoke_convertible<type, RetType>, invoke_nothrow_convertible<type, RetType>>>>>;
		};

		template <typename Void, typename Callable>
		struct invoke_traits_zero
		{
			// Selected when Callable isn't callable with zero Args.
			using is_invocable = false_type;
			using is_nothrow_invocable = false_type;

			template <typename RetType>
			using is_invocable_ret = false_type;

			template <typename RetType>
			using is_nothrow_invocable_ret = false_type;
		};

		template <typename Callable>
		using decltype_invoke_zero = decltype(declval<Callable>()());

		template <typename Callable>
		struct invoke_traits_zero<void_t<decltype_invoke_zero<Callable>>, Callable> :
			invoke_traits<decltype_invoke_zero<Callable>, noexcept(declval<Callable>()())>
		{
		};

		template <typename Void, typename... Types>
		struct invoke_traits_nonzero
		{
			// Selected when Callable isn't callable with nonzero Args.
			using is_invocable = false_type;
			using is_nothrow_invocable = false_type;

			template <typename RetType>
			using is_invocable_ret = false_type;

			template <typename RetType>
			using is_nothrow_invocable_ret = false_type;
		};

		template <typename Callable, typename Ty1, typename... Types2>
		using decltype_invoke_nonzero =
			decltype(::std::invoke(declval<Callable>(), declval<Ty1>(), declval<Types2>()...));

		template <typename Callable, typename Ty1, typename... Types2>
		struct invoke_traits_nonzero<
			void_t<decltype_invoke_nonzero<Callable, Ty1, Types2...>>, Callable, Ty1, Types2...> :
			invoke_traits<
				decltype_invoke_nonzero<Callable, Ty1, Types2...>,
				noexcept(::std::invoke(declval<Callable>(), declval<Ty1>(), declval<Types2>()...))>
		{
		};

		template <typename Callable, typename... Args>
		using select_invoke_traits = conditional_t<
			sizeof...(Args) == 0, invoke_traits_zero<void, Callable>, invoke_traits_nonzero<void, Callable, Args...>>;

		template <typename ReturnType, typename Callable, typename... Args>
		using is_invocable_ret_impl =
			typename select_invoke_traits<Callable, Args...>::template is_invocable_ret<ReturnType>;
	} // namespace internal

	template <typename Callable, typename... Args>
	using invoke_result_t = typename internal::select_invoke_traits<Callable, Args...>::type;

	template <typename Callable, typename... Args>
	struct invoke_result
	{
		using type = invoke_result_t<Callable, Args...>;
	};

	template <typename Callable, typename... Args>
	struct is_invocable_args : internal::select_invoke_traits<Callable, Args...>::is_invocable
	{
		// Determines whether Callable is callable with Args
	};

	template <typename Callable, typename... Args>
	inline constexpr bool is_invocable_args_v =
		internal::select_invoke_traits<Callable, Args...>::is_invocable_args::value;

	template <typename Callable, typename... Args>
	struct is_nothrow_invocable_args : internal::select_invoke_traits<Callable, Args...>::is_nothrow_invocable
	{
		// Determines whether Callable is nothrow-callable with Args
	};

	template <typename Callable, typename... Args>
	inline constexpr bool is_nothrow_invocable_args_v =
		internal::select_invoke_traits<Callable, Args...>::is_nothrow_invocable::value;

	template <typename ReturnType, typename Callable, typename... Args>
	struct is_invocable_args_ret : internal::is_invocable_ret_impl<ReturnType, Callable, Args...>
	{
		// Determines whether Callable is callable with Args and return type ReturnType
	};

	template <typename ReturnType, typename Callable, typename... Args>
	inline constexpr bool is_invocable_args_ret_v =
		internal::is_invocable_ret_impl<ReturnType, Callable, Args...>::value;

	template <typename ReturnType, typename Callable, typename... Args>
	struct is_nothrow_invocable_args_ret :
		internal::select_invoke_traits<Callable, Args...>::template is_nothrow_invocable_ret<ReturnType>
	{
		// Determines whether Callable is nothrow-callable with Args and return type ReturnType
	};

	template <typename ReturnType, typename Callable, typename... Args>
	inline constexpr bool is_nothrow_invocable_args_ret_v =
		internal::select_invoke_traits<Callable, Args...>::template is_nothrow_invocable_ret<ReturnType>::value;

	struct any_type
	{
		template <typename T>
		constexpr operator T&(); // implicit conversion to any type.

		template <typename T>
		constexpr any_type(const T&); // implicit conversion from any type.
	};

	namespace internal
	{
		template <typename Func, size_type... paramCounts>
		constexpr bool test_invocable_impl([[maybe_unused]] integer_sequence<size_type, paramCounts...> int_seq)
		{
			return ((make_sequence_t<is_invocable_args, any_type, paramCounts, Func>::value) || ...);
		}
	} // namespace internal

	template <typename Func, size_type maxParams = 32>
	struct is_invocable
	{
		static constexpr bool value = internal::test_invocable_impl<Func>(make_index_sequence<maxParams>{});
	};

	template <typename Func, size_type maxParams = 32>
	constexpr bool is_invocable_v = is_invocable<Func, maxParams>::value;

	template <typename Func, size_type maxParams = 32>
	constexpr bool is_function_ptr_v = (is_empty_v<Func> || is_pointer_v<Func>) && is_invocable_v<Func, maxParams>;

	template <typename Func, size_type maxParams = 32>
	constexpr bool is_functor_v = requires { &Func::operator(); } && is_invocable_v<Func, maxParams>;

	template <typename Type>
	constexpr bool is_ratio_v = false; // test for ratio type

	template <int_max numerator, int_max denominator>
	constexpr bool is_ratio_v<::std::ratio<numerator, denominator>> = true;

	template <typename Type>
	struct is_ratio : bool_constant<is_ratio_v<Type>>
	{
	};

	template <typename derived_type, typename base_type>
	using inherits_from = typename enable_if<::rsl::is_base_of_v<base_type, derived_type>, int>::type;

	template <typename derived_type, typename base_type>
	using doesnt_inherit_from = typename enable_if<!::rsl::is_base_of_v<base_type, derived_type>, int>::type;

	template <template <typename> typename Compare, typename T, T A, T B>
	struct do_compare
	{
		static constexpr inline Compare<T> comp{};
		static constexpr inline bool value = comp(A, B);
	};

	template <template <typename> typename Compare, typename T, T A, T B>
	inline constexpr bool do_compare_v = do_compare<Compare, T, A, B>::value;

	template <rsl::size_type I, typename Type, typename... Types>
	struct element_at : element_at<I - 1, Types...>
	{
	};

	template <typename Type, typename... Types>
	struct element_at<0, Type, Types...>
	{
		using type = Type;
	};

	template <rsl::size_type I, typename Type, typename... Types>
	using element_at_t = typename element_at<I, Type, Types...>::type;

	template <typename T, typename... Args>
	struct is_brace_constructible
	{
		static constexpr bool value = requires(Args&&... args) { T{::rsl::forward<Args>(args)...}; };
	};

	template <typename T, typename... Args>
	inline constexpr bool is_brace_constructible_v = is_brace_constructible<T, Args...>::value;


	template <rsl::size_type I, typename Check, typename...>
	struct element_at_is_same_as;

	template <rsl::size_type I, typename Check, typename Type, typename... Types>
	struct element_at_is_same_as<I, Check, Type, Types...> : element_at_is_same_as<I - 1, Check, Type, Types...>
	{
	};

	template <typename Check, typename Type, typename... Types>
	struct element_at_is_same_as<0, Check, Type, Types...>
	{
		static constexpr bool value = is_same_v<Check, Type>;
	};

	template <rsl::size_type I, typename Check>
	struct element_at_is_same_as<I, Check>
	{
		static constexpr bool value = false;
	};

	template <rsl::size_type I, typename Check, typename... Types>
	inline constexpr bool element_at_is_same_as_v = element_at_is_same_as<I, Check, Types...>::value;

	struct lval_ref_signal
	{
	};

	using ref_signal = lval_ref_signal;

	struct rval_ref_signal
	{
	};

	using move_signal = rval_ref_signal;

	struct const_signal
	{
	};

	struct pointer_signal
	{
	};

	template <typename T, typename... DecorationSignals>
	struct decorate_type;

	template <typename T, typename DecorationSignal>
	struct decorate_type<T, DecorationSignal>
	{
		static_assert(false, "Uknown signal.");
	};

	template <typename T>
	struct decorate_type<T>
	{
		using type = T;
	};

	template <typename T, typename DecorationSignal, typename... Rest>
	struct decorate_type<T, DecorationSignal, Rest...>
	{
		using type = decorate_type<typename decorate_type<T, DecorationSignal>::type, Rest...>::type;
	};

	template <typename T>
	struct decorate_type<T, lval_ref_signal>
	{
		using type = typename add_lval_ref<T>::type;
	};

	template <typename T>
	struct decorate_type<T, rval_ref_signal>
	{
		using type = typename add_rval_ref<T>::type;
	};

	template <typename T>
	struct decorate_type<T, const_signal>
	{
		using type = typename add_const<T>::type;
	};

	template <typename T>
	struct decorate_type<T, pointer_signal>
	{
		using type = typename add_pointer<T>::type;
	};

	template <typename T, typename... DecorationSignals>
	using decorate_type_t = decorate_type<T, DecorationSignals...>::type;

	template <typename T, typename = void>
	struct is_always_equal
	{
		using type = bool_constant<::rsl::is_empty_v<T>>;
	};

	template <typename T>
	struct is_always_equal<T, void_t<typename T::is_always_equal>>
	{
		using type = typename T::is_always_equal;
	};

	template <typename T>
	using is_always_equal_t = is_always_equal<T>::type;

	template <typename T1, typename T2>
	concept same_as = ::rsl::is_same_v<T1, T2>;

	template <typename Derived, typename Base>
	concept derived_from =
		::rsl::is_base_of_v<Base, Derived> && ::rsl::is_convertible_v<const volatile Derived*, const volatile Base*>;

	template <typename From, typename To>
	concept convertible_to = ::rsl::is_convertible_v<From, To> && requires { static_cast<To>(::rsl::declval<From>()); };

	template <typename LHS, typename RHS>
	concept assignable_from =
		::rsl::is_lvalue_reference_v<LHS> &&
		::std::common_reference_with<const ::rsl::remove_reference_t<LHS>&, const ::rsl::remove_reference_t<RHS>&> &&
		requires(LHS lhs, RHS&& rhs) {
			{ lhs = static_cast<RHS&&>(rhs) } -> same_as<LHS>;
		};

	template <class _Ty>
	concept destructible = __is_nothrow_destructible(_Ty);

	template <class _Ty, class... _ArgTys>
	concept constructible_from = destructible<_Ty> && __is_constructible(_Ty, _ArgTys...);

	template <class _Ty>
	concept default_initializable = constructible_from<_Ty> && requires {
		_Ty{};
		::new (static_cast<void*>(nullptr)) _Ty; // is-default-initializable<_Ty>
	};

	template <class _Ty>
	concept move_constructible = constructible_from<_Ty, _Ty> && convertible_to<_Ty, _Ty>;

	template <class _Ty>
	concept swappable = requires(_Ty& __x, _Ty& __y) { _RANGES swap(__x, __y); };

	template <class _Ty1, class _Ty2>
	concept swappable_with = _STD common_reference_with<_Ty1, _Ty2> && requires(_Ty1&& __t, _Ty2&& __u) {
		_RANGES swap(static_cast<_Ty1&&>(__t), static_cast<_Ty1&&>(__t));
		_RANGES swap(static_cast<_Ty2&&>(__u), static_cast<_Ty2&&>(__u));
		_RANGES swap(static_cast<_Ty1&&>(__t), static_cast<_Ty2&&>(__u));
		_RANGES swap(static_cast<_Ty2&&>(__u), static_cast<_Ty1&&>(__t));
	};

	template <class _Ty>
	concept copy_constructible =
		move_constructible<_Ty> && constructible_from<_Ty, _Ty&> && convertible_to<_Ty&, _Ty> &&
		constructible_from<_Ty, const _Ty&> && convertible_to<const _Ty&, _Ty> && constructible_from<_Ty, const _Ty> &&
		convertible_to<const _Ty, _Ty>;

	template <class _Ty>
	concept _Boolean_testable_impl = convertible_to<_Ty, bool>;

	template <class _Ty>
	concept _Boolean_testable = _Boolean_testable_impl<_Ty> && requires(_Ty&& __t) {
		{ !static_cast<_Ty&&>(__t) } -> _Boolean_testable_impl;
	};

	template <class _Ty1, class _Ty2>
	concept _Half_equality_comparable =
		requires(const remove_reference_t<_Ty1>& __x, const remove_reference_t<_Ty2>& __y) {
			{ __x == __y } -> _Boolean_testable;
			{ __x != __y } -> _Boolean_testable;
		};

	template <class _Ty1, class _Ty2>
	concept _Weakly_equality_comparable_with =
		_Half_equality_comparable<_Ty1, _Ty2> && _Half_equality_comparable<_Ty2, _Ty1>;

	template <class _Ty>
	concept equality_comparable = _Half_equality_comparable<_Ty, _Ty>;

	template <class _Ty>
	concept movable = is_object_v<_Ty> && move_constructible<_Ty> && assignable_from<_Ty&, _Ty> && swappable<_Ty>;

	template <class _Ty>
	concept copyable = copy_constructible<_Ty> && movable<_Ty> && assignable_from<_Ty&, _Ty&> &&
					   assignable_from<_Ty&, const _Ty&> && assignable_from<_Ty&, const _Ty>;

	template <class _Ty>
	concept semiregular = copyable<_Ty> && default_initializable<_Ty>;

	template <class _Ty>
	concept regular = semiregular<_Ty> && equality_comparable<_Ty>;

	template <class _FTy, class... _ArgTys>
	concept invocable = requires(_FTy&& _Fn, _ArgTys&&... _Args) {
		_STD invoke(static_cast<_FTy&&>(_Fn), static_cast<_ArgTys&&>(_Args)...);
	};

	template <class _FTy, class... _ArgTys>
	concept regular_invocable = invocable<_FTy, _ArgTys...>;

	template <class _FTy, class... _ArgTys>
	concept predicate = regular_invocable<_FTy, _ArgTys...> && _Boolean_testable<invoke_result_t<_FTy, _ArgTys...>>;

	template <class _FTy, class _Ty1, class _Ty2>
	concept relation = predicate<_FTy, _Ty1, _Ty1> && predicate<_FTy, _Ty2, _Ty2> && predicate<_FTy, _Ty1, _Ty2> &&
					   predicate<_FTy, _Ty2, _Ty1>;

	template <class _FTy, class _Ty1, class _Ty2>
	concept equivalence_relation = relation<_FTy, _Ty1, _Ty2>;

	template <class _FTy, class _Ty1, class _Ty2>
	concept strict_weak_order = relation<_FTy, _Ty1, _Ty2>;

#define RYTHE_HAS_FUNC(x)                                                                                              \
	namespace internal                                                                                                 \
	{                                                                                                                  \
		template <typename, typename T>                                                                                \
		struct RYTHE_CONCAT(has_, RYTHE_CONCAT(x, _impl))                                                              \
		{                                                                                                              \
			static_assert(                                                                                             \
				::rsl::integral_constant<bool_t<T>, false>::value,                                                     \
				"Second template param needs to be of function type."                                                  \
			);                                                                                                         \
		};                                                                                                             \
                                                                                                                       \
		template <typename C, typename Ret, typename... Args>                                                          \
		struct RYTHE_CONCAT(has_, RYTHE_CONCAT(x, _impl))<C, Ret(Args...)>                                             \
		{                                                                                                              \
			static constexpr bool value = requires(C& val, Args&&... args) {                                           \
				{ val.x(::rsl::forward<Args>(args)...) } -> ::rsl::convertible_to<Ret>;                                \
			};                                                                                                         \
		};                                                                                                             \
	}                                                                                                                  \
                                                                                                                       \
	template <typename C, typename F>                                                                                  \
	constexpr bool RYTHE_CONCAT(has_, RYTHE_CONCAT(x, _v)) =                                                           \
		RYTHE_CONCAT(internal::has_, RYTHE_CONCAT(x, _impl))<C, F>::value;                                             \
                                                                                                                       \
	template <typename C, typename F>                                                                                  \
	concept RYTHE_CONCAT(has_, x) = RYTHE_CONCAT(has_, RYTHE_CONCAT(x, _v))<C, F>;                                     \
                                                                                                                       \
	namespace internal                                                                                                 \
	{                                                                                                                  \
		template <typename, typename T>                                                                                \
		struct RYTHE_CONCAT(has_static_, RYTHE_CONCAT(x, _impl))                                                       \
		{                                                                                                              \
			static_assert(                                                                                             \
				::rsl::integral_constant<bool_t<T>, false>::value,                                                     \
				"Second template param needs to be of function type."                                                  \
			);                                                                                                         \
		};                                                                                                             \
                                                                                                                       \
		template <typename C, typename Ret, typename... Args>                                                          \
		struct RYTHE_CONCAT(has_static_, RYTHE_CONCAT(x, _impl))<C, Ret(Args...)>                                      \
		{                                                                                                              \
			static constexpr bool value = requires(Args&&... args) {                                                   \
				{ C::x(::rsl::forward<Args>(args)...) } -> ::rsl::convertible_to<Ret>;                                 \
			};                                                                                                         \
		};                                                                                                             \
	}                                                                                                                  \
                                                                                                                       \
	template <typename C, typename F>                                                                                  \
	constexpr bool RYTHE_CONCAT(has_static_, RYTHE_CONCAT(x, _v)) =                                                    \
		RYTHE_CONCAT(internal::has_static_, RYTHE_CONCAT(x, _impl))<C, F>::value;                                      \
                                                                                                                       \
	template <typename C, typename F>                                                                                  \
	concept RYTHE_CONCAT(has_static_, x) = RYTHE_CONCAT(has_static_, RYTHE_CONCAT(x, _v))<C, F>;

#if defined(RYTHE_MSVC)

	template <typename T>
	constexpr std::string_view type_name() noexcept
	{
		std::string_view funcName(__FUNCSIG__);
		auto first = funcName.find_first_of(' ', funcName.find("type_name<") + 11) + 1;
		return funcName.substr(first, funcName.find_last_of('>') - first);
	}

#elif defined(RYTHE_GCC)

	template <typename T>
	constexpr std::string_view type_name() noexcept
	{
		std::string_view funcName(__PRETTY_FUNCTION__);
		auto first = funcName.find_first_not_of(' ', funcName.find_first_of('=') + 1);
		return funcName.substr(first, funcName.find_last_of(';') - first);
	}

#elif defined(RYTHE_CLANG)

	template <typename T>
	constexpr std::string_view type_name() noexcept
	{
		std::string_view funcName(__PRETTY_FUNCTION__);
		auto first = funcName.find_first_not_of(' ', funcName.find_first_of('=') + 1);
		return funcName.substr(first, funcName.find_last_of(']') - first);
	}

#endif

	template <typename T>
	constexpr std::string_view type_name(T&&) noexcept
	{
		return type_name<remove_cvr_t<T>>();
	}

#if defined(RYTHE_MSVC) || defined(RYTHE_CLANG_MSVC)
	namespace detail
	{
		template <typename T>
		std::string getNameOfTypeImpl()
		{
			const std::string name = typeid(T).name();
			std::vector<std::string> vec = split_string_at<' '>(name);
			if (vec.size() < 2)
			{
				return name;
			}

			std::string result;
			for (auto i = 1; i < vec.size(); i++)
			{
				if (ends_with(vec[i], "struct"))
					result += vec[i].substr(0, vec[i].size() - 6);
				else if (ends_with(vec[i], "class"))
					result += vec[i].substr(0, vec[i].size() - 5);
				else
					result += vec[i];
			}
			return result;
		}
	} // namespace detail
#elif defined(RYTHE_GCC)

	#include <cxxabi.h>
	namespace detail
	{
		template <typename T>
		std::string getNameOfTypeImpl()
		{
			// Demangle the typename generated by GCC
			int status;
			char* realName = abi::__cxa_demangle(typeid(T).name(), 0, 0, &status);
			std::string result = {realName};
			free(realName);
			return result;
		}
	} // namespace detail
#elif defined(RYTHE_CLANG_GCC)

	#include <cxxabi.h>
	namespace detail
	{
		template <typename T>
		std::string getNameOfTypeImpl()
		{
			// Demangle the typename generated by GCC
			int status;
			char* realName = __cxxabiv1::__cxa_demangle(typeid(T).name(), 0, 0, &status);
			std::string result = {realName};
			free(realName);
			return result;
		}
	} // namespace detail
#endif

	/**@brief Returns compile-time evaluable type name.
	 * @warning This version is not compiler agnostic! If you need it to be compiler agnostic use `nameOfType`.
	 */
	template <typename T>
	constexpr std::string_view localNameOfType() noexcept
	{
#if defined(RYTHE_CLANG) || defined(RYTHE_GCC)
		cstring p = __PRETTY_FUNCTION__;

		while (*p++ != '=');

		for (; *p == ' '; ++p);

		cstring p2 = p;
		int count = 1;

		for (;; ++p2)
		{
			switch (*p2)
			{
				case '[': ++count; break;
				case ']':
				case ';':
					--count;
					if (!count)
						return {p, size_type(p2 - p)};
			}
		}

		return {};
#elif defined(RYTHE_MSVC)
		cstring p = __FUNCSIG__;

		while (*p != 'T' || *(p + 1) != 'y' || *(p + 2) != 'p' || *(p + 3) != 'e' || *(p + 4) != '<') p++;

		while (*p++ != ' ');

		cstring p2 = p;
		int count = 1;
		size_type size = 0;

		for (; size == 0; ++p2)
		{
			switch (*p2)
			{
				case '<': ++count; break;
				case '>':
					--count;
					if (!count)
					{
						size = (p2 - p);
					}
			}
		}

		return {p, size};

#else
	#error unknown compiler
#endif
	}


	/**@brief Returns type name with namespaces other than that it's undecorated.
	 * @tparam T type of which you want the name.
	 */
	template <typename T>
	cstring nameOfType()
	{
		static std::string name = detail::getNameOfTypeImpl<T>();
		return name.c_str();
	}

	/**@brief Returns type name with namespaces other than that it's undecorated.
	 * @tparam T type of which you want the name.
	 * @param expr Variable of which you wish to auto deduct type.
	 */
	template <typename T>
	cstring nameOfType(T&& expr)
	{
		return nameOfType<decay_t<T>>();
	}

	/**@brief Returns hash of a certain string
	 * @tparam N Length of the string literal
	 * @param name Name you wish to hash
	 * @note Since this version takes a const char[] it can only really be used with data coming from a string literal.
	 *       Because it takes in a const char[] this function is able to be constexpr and thus have minimal overhead.
	 */
	template <size_type N>
	constexpr id_type nameHash(const char (&name)[N]) noexcept
	{
		id_type hash = 0xcbf29ce484222325;
		constexpr uint64 prime = 0x00000100000001b3;

		size_type size = N;
		if (name[size - 1] == '\0')
			size--;

		for (size_type i = 0; i < size; i++)
		{
			hash = hash ^ static_cast<const byte>(name[i]);
			hash *= prime;
		}

		return hash;
	}

	/**@brief Returns hash of a certain string
	 * @tparam N Length of the string literal
	 * @param name Name you wish to hash
	 * @note Since this version takes a const char[] it can only really be used with data coming from a string literal.
	 *       Because it takes in a const char[] this function is able to be constexpr and thus have minimal overhead.
	 */
	template <size_type N>
	constexpr id_type nameHash(const std::array<char, N>& name) noexcept
	{
		id_type hash = 0xcbf29ce484222325;
		constexpr uint64 prime = 0x00000100000001b3;

		size_type size = N;
		if (name[size - 1] == '\0')
			size--;

		for (size_type i = 0; i < size; i++)
		{
			hash = hash ^ static_cast<const byte>(name[i]);
			hash *= prime;
		}

		return hash;
	}

	/**@brief Returns hash of a certain string
	 * @param name Name you wish to hash
	 */
	constexpr id_type nameHash(cstring name) noexcept
	{
		id_type hash = 0xcbf29ce484222325;
		constexpr uint64 prime = 0x00000100000001b3;

		for (size_type i = 0; i < rsl::constexpr_strlen(name); i++)
		{
			hash = hash ^ static_cast<const byte>(name[i]);
			hash *= prime;
		}

		return hash;
	}

	/**@brief Returns hash of a certain string
	 * @param name Name you wish to hash
	 */
	constexpr id_type nameHash(const std::string_view& name) noexcept
	{
		id_type hash = 0xcbf29ce484222325;
		constexpr uint64 prime = 0x00000100000001b3;

		size_type size = name.size();

		if (name[size - 1] == '\0')
			size--;

		for (size_type i = 0; i < size; i++)
		{
			hash = hash ^ static_cast<const byte>(name[i]);
			hash *= prime;
		}

		return hash;
	}
	//
	//    /**@brief Returns hash of a certain string
	// * @param name Name you wish to hash
	// */
	//    id_type nameHash(const std::string& name)
	//    {
	// #if defined(RYTHE_MSVC) || defined(RYTHE_CLANG_MSVC)
	//        static std::hash<std::string> hasher{};
	//        if (!name.empty() && name[name.size() - 1] == '\0')
	//            return nameHash(std::string_view(name));
	//
	//        return hasher(name);
	// #else
	//        // std::hash returns a different hash on GCC and Clang on Linux for certain CPU architectures.
	//        // These certain different hashes are faster to compute but can create issues if they aren't the same.
	//        return nameHash(std::string_view(name));
	// #endif
	//    }


	/**@brief Returns compile-time evaluable hash of the type name.
	 * @warning This version is not compiler agnostic! If you need it to be compiler agnostic use `typeHash`.
	 */
	template <typename T>
	constexpr id_type localTypeHash() noexcept
	{
		return nameHash(localNameOfType<T>());
	}

} // namespace rsl

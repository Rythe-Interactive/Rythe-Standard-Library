#pragma once
#include <type_traits>

#include "primitives.hpp"

namespace rsl
{
	template <typename T, T Val>
	struct integral_constant
	{
		static constexpr T value = Val;

		using value_type = T;
		using type = integral_constant;

		constexpr operator value_type() const noexcept { return value; }

		[[nodiscard]] constexpr value_type operator()() const noexcept { return value; }
	};

	template <typename>
	inline constexpr bool always_false_v = false; // False value attached to a dependent name (for static_assert).

	template <bool Val>
	using bool_constant = integral_constant<bool, Val>;

	using true_type = bool_constant<true>;
	using false_type = bool_constant<false>;

	template <bool Test, typename T = void>
	struct enable_if
	{
	};

	template <typename T>
	struct enable_if<true, T>
	{
		using type = T;
	};

	template <bool Test, typename T = void>
	using enable_if_t = typename enable_if<Test, T>::type;

	template <typename T>
	inline constexpr bool is_empty_v = ::std::is_empty_v<T>; // Compiler magic behind the scenes.

	template <typename T>
	struct is_empty : bool_constant<is_empty_v<T>>
	{
	};

	template <typename>
	inline constexpr bool is_pointer_v = false;

	template <typename T>
	inline constexpr bool is_pointer_v<T*> = true;

	template <typename T>
	inline constexpr bool is_pointer_v<T* const> = true;

	template <typename T>
	inline constexpr bool is_pointer_v<T* volatile> = true;

	template <typename T>
	inline constexpr bool is_pointer_v<T* const volatile> = true;

	template <typename T>
	struct is_pointer : bool_constant<is_pointer_v<T>>
	{
	};

	template <typename, typename>
	inline constexpr bool is_same_v = false;

	template <typename T>
	inline constexpr bool is_same_v<T, T> = true;

	template <typename LHS, typename RHS>
	struct is_same : bool_constant<is_same_v<LHS, RHS>>
	{
	};

	template <bool Test, typename TrueType, typename FalseType>
	struct conditional
	{
		using type = TrueType;
	};

	template <typename TrueType, typename FalseType>
	struct conditional<false, TrueType, FalseType>
	{
		using type = FalseType;
	};

	template <bool Test, typename TrueType, typename FalseType>
	using conditional_t = typename conditional<Test, TrueType, FalseType>::type;

	// Implementation details of conjunction<...> and disjunction<...>
	namespace internal
	{
		template <bool First_value, typename First, typename... Rest>
		struct conjunction_impl
		{ // Handle false trait or last trait.
			using type = First;
		};

		template <typename True, typename Next, typename... Rest>
		struct conjunction_impl<true, True, Next, Rest...>
		{ // The first trait is true, try the next one.
			using type = typename conjunction_impl<Next::value, Next, Rest...>::type;
		};

		template <bool First_value, typename First, typename... Rest>
		struct disjuction_impl
		{ // Handle true trait or last trait.
			using type = First;
		};

		template <typename False, typename Next, typename... Rest>
		struct disjuction_impl<false, False, Next, Rest...>
		{ // First trait is false, try the next trait.
			using type = typename disjuction_impl<Next::value, Next, Rest...>::type;
		};
	} // namespace internal

	template <typename... Traits>
	struct conjunction : true_type
	{
	}; // If Traits is empty, true_type.

	template <typename First, typename... Rest>
	struct conjunction<First, Rest...> : internal::conjunction_impl<First::value, First, Rest...>::type
	{
		// The first false trait in Traits, or the last trait if none are false.
	};

	template <typename... Traits>
	inline constexpr bool conjunction_v = conjunction<Traits...>::value;

	template <typename Trait>
	struct negation : bool_constant<!static_cast<bool>(Trait::value)>
	{
	}; // The negated result of Trait

	template <typename Trait>
	inline constexpr bool negation_v = negation<Trait>::value;

	template <typename... Traits>
	struct disjunction : false_type
	{
	}; // If Traits is empty, false_type.

	template <typename First, typename... Rest>
	struct disjunction<First, Rest...> : internal::disjuction_impl<First::value, First, Rest...>::type
	{
		// The first true trait in Traits, or the last trait if none are true.
	};

	template <typename... Traits>
	inline constexpr bool disjunction_v = disjunction<Traits...>::value;

	template <typename T, typename... Types>
	inline constexpr bool is_any_of_v = (is_same_v<T, Types> || ...); // True if and only if T is in Types.

	template <typename T>
	struct remove_reference
	{
		using type = T;
	};

	template <typename T>
	struct remove_reference<const T&>
	{
		using type = const T;
	};

	template <typename T>
	struct remove_reference<T&>
	{
		using type = T;
	};

	template <typename T>
	struct remove_reference<T&&>
	{
		using type = T;
	};

	template <typename T>
	using remove_reference_t = typename remove_reference<T>::type;

	template <typename T>
	struct remove_cv
	{
		using type = T;
	};

	template <typename T>
	struct remove_cv<const T>
	{
		using type = T;
	};

	template <typename T>
	struct remove_cv<volatile T>
	{
		using type = T;
	};

	template <typename T>
	struct remove_cv<const volatile T>
	{
		using type = T;
	};

	template <typename T>
	using remove_cv_t = typename remove_cv<T>::type;

	template <typename T>
	using remove_cvr = remove_cv<remove_reference_t<T>>;

	template <typename T>
	using remove_cvr_t = typename remove_cvr<T>::type;

	template <typename T>
	inline constexpr bool is_void_v = is_same_v<remove_cv_t<T>, void>;

	template <typename T>
	struct is_void : bool_constant<is_void_v<T>>
	{
	};

	template <typename... Types>
	using void_t = void;

	template <typename... Types>
	using bool_t = bool;

	template <typename T>
	inline constexpr bool is_integral_v = is_any_of_v<
		remove_cv_t<T>, bool, char, signed char, unsigned char, wchar_t, char8_t, char16_t, char32_t, short,
		unsigned short, int, unsigned int, long, unsigned long, long long, unsigned long long>;

	template <typename T>
	struct is_integral : bool_constant<is_integral_v<T>>
	{
	};

	template <typename T>
	inline constexpr bool is_floating_point_v = is_any_of_v<remove_cv_t<T>, float, double, long double>;

	template <typename T>
	struct is_floating_point : bool_constant<is_floating_point_v<T>>
	{
	};

	template <typename T>
	inline constexpr bool is_arithmetic_v = // determine whether T is an arithmetic type
		is_integral_v<T> || is_floating_point_v<T>;

	template <typename T>
	struct is_arithmetic : bool_constant<is_arithmetic_v<T>>
	{
	};

	template <typename>
	inline constexpr bool is_const_v = false;

	template <typename T>
	inline constexpr bool is_const_v<const T> = true;

	template <typename T>
	struct is_const : bool_constant<is_const_v<T>>
	{
	};

	template <typename>
	inline constexpr bool is_lvalue_reference_v = false; // determine whether type argument is an lvalue reference

	template <typename T>
	inline constexpr bool is_lvalue_reference_v<T&> = true;

	template <typename T>
	struct is_lvalue_reference : bool_constant<is_lvalue_reference_v<T>>
	{
	};

	template <typename>
	inline constexpr bool is_rvalue_reference_v = false; // determine whether type argument is an rvalue reference

	template <typename T>
	inline constexpr bool is_rvalue_reference_v<T&&> = true;

	template <typename T>
	struct is_rvalue_reference : bool_constant<is_rvalue_reference_v<T>>
	{
	};

	template <typename>
	inline constexpr bool is_reference_v = false;

	template <typename T>
	inline constexpr bool is_reference_v<T&> = true;

	template <typename T>
	inline constexpr bool is_reference_v<T&&> = true;

	template <typename T>
	struct is_reference : bool_constant<is_reference_v<T>>
	{
	};

	// Implementation details of add_XXXX<...>
	namespace internal
	{
		// pointer type cannot be formed
		template <typename T, typename = void>
		struct add_pointer_impl
		{
			using type = T;
		};

		// pointer type can be formed
		template <typename T>
		struct add_pointer_impl<T, void_t<remove_reference_t<T>*>>
		{
			using type = remove_reference<T>::type*;
		};

		template <typename T>
		struct add_const_impl
		{
			using type = const T;
		};

		template <typename T>
		struct add_const_impl<T&>
		{
			using type = const T&;
		};

		template <typename T>
		struct add_const_impl<T&&>
		{
			using type = T&&;
		};

		template <typename T>
		struct add_const_impl<T*>
		{
			using type = const T*;
		};
	} // namespace internal

	template <typename T>
	struct add_pointer
	{
		using type = typename internal::add_pointer_impl<T>::type;
	};

	template <typename T>
	using add_pointer_t = typename internal::add_pointer_impl<T>::type;

	template <typename T>
	struct add_const
	{
		using type = typename internal::add_const_impl<T>::type;
	};

	template <typename T>
	using add_const_t = typename add_const<T>::type;

	template <typename T, typename = void>
	struct add_ref
	{
		using lvalue = T;
		using rvalue = T;
	};

	template <typename T>
	struct add_ref<T, void_t<T&>>
	{
		using lvalue = T&;
		using rvalue = T&&;
	};

	template <typename T>
	struct add_ref<const T, void_t<const T&>>
	{
		using lvalue = const T&;
		using rvalue = const T&&;
	};

	template <typename T>
	struct add_lval_ref
	{
		using type = typename add_ref<T>::lvalue;
	};

	template <typename T>
	using add_lval_ref_t = typename add_lval_ref<T>::type;

	template <typename T>
	struct add_rval_ref
	{
		using type = typename add_ref<T>::rvalue;
	};

	template <typename T>
	using add_rval_ref_t = typename add_rval_ref<T>::type;

	// Only function types and reference types can't be const qualified.
	template <typename T>
	inline constexpr bool is_function_v = !is_const_v<const T> && !is_reference_v<T>;

	template <typename T>
	struct is_function : bool_constant<is_function_v<T>>
	{
	};

	// Only function types and reference types can't be const qualified.
	template <typename T>
	inline constexpr bool is_object_v = is_const_v<const T> && !is_void_v<T>;

	template <typename T>
	struct is_object : bool_constant<is_object_v<T>>
	{
	};

	template <typename T, typename... Args>
	inline constexpr bool is_constructible_v =
		::std::is_constructible_v<T, Args...>; // Uses compiler magic behind the scenes.

	template <typename T, typename... Args>
	struct is_constructible : bool_constant<is_constructible_v<T, Args...>>
	{
		// determine whether T can be direct-initialized with Args...
	};

	template <typename T>
	struct is_copy_constructible :
		::rsl::is_constructible<T, typename ::rsl::add_lval_ref<typename ::rsl::add_const<T>::type>::type>
	{
	};

	template <typename>
	inline constexpr bool is_array_v = false;

	template <typename T, size_type I>
	inline constexpr bool is_array_v<T[I]> = true;

	template <typename T>
	inline constexpr bool is_array_v<T[]> = true;

	template <typename T>
	struct is_array : bool_constant<is_array_v<T>>
	{
	};

	template <typename T, unsigned int I = 0>
	inline constexpr size_type extent_v = 0; // Determine extent of dimension I of array T.

	template <typename T, size_type N>
	inline constexpr size_type extent_v<T[N], 0> = N;

	template <typename T, unsigned int I, size_type N>
	inline constexpr size_type extent_v<T[N], I> = extent_v<T, I - 1>;

	template <typename T, unsigned int I>
	inline constexpr size_type extent_v<T[], I> = extent_v<T, I - 1>;

	template <typename T, unsigned int I = 0>
	struct extent : integral_constant<size_type, extent_v<T, I>>
	{
	};

	// remove array extent
	template <typename T>
	struct remove_extent
	{
		using type = T;
	};

	template <typename T, size_type I>
	struct remove_extent<T[I]>
	{
		using type = T;
	};

	template <typename T>
	struct remove_extent<T[]>
	{
		using type = T;
	};

	template <typename T>
	using remove_extent_t = typename remove_extent<T>::type;

	// Implementation details of decay_t<...>
	namespace internal
	{
		template <typename T>
		struct decay_impl
		{
			using T1 = remove_reference_t<T>;
			using T2 = conditional_t<is_function_v<T1>, add_pointer<T1>, remove_cv<T1>>;
			using type = typename conditional_t<is_array_v<T1>, add_pointer<remove_extent_t<T1>>, T2>::type;
		};
	} // namespace internal


	template <typename T>
	T returns_exactly() noexcept; // not defined

	template <typename T>
	[[nodiscard]] T fake_copy_init(T) noexcept;
	// fake_copy_init<T>(E):
	// (1) has type T [decay_t<decltype((E))> if T is deduced],
	// (2) is well-formed if and only if E is implicitly convertible to T and T is destructible, and
	// (3) is non-throwing if and only if both conversion from decltype((E)) to T and destruction of T are non-throwing.

	template <typename T>
	add_rval_ref_t<T> declval() noexcept
	{
		static_assert(always_false_v<T>, "Calling declval is ill-formed.");
	}

	template <typename T>
	struct decay
	{
		using type = internal::decay_impl<T>::type;
	};

	template <typename T>
	using decay_t = typename internal::decay_impl<T>::type;

	template <typename T>
	using is_union = ::std::is_union<T>; // Compiler magic behind the scenes.

	template <typename T>
	inline constexpr bool is_union_v = is_union<T>::value;

	// Implementation details of is_class<...>
	namespace internal
	{
		// Test if member object pointers of this type are valid.
		template <typename T>
		bool_constant<!is_union<T>::value> test_is_class(int T::*);

		template <typename>
		false_type test_is_class(...);
	} // namespace internal

	template <typename T>
	struct is_class : decltype(internal::test_is_class<T>(nullptr))
	{
	};

	template <typename T>
	inline constexpr bool is_class_v = is_class<T>::value;

	// Implementation details of is_base_of<...>
	namespace internal
	{
		template <typename Base>
		true_type test_ptr_conv(const volatile Base*);
		template <typename>
		false_type test_ptr_conv(const volatile void*);

		template <typename Base, typename Derived>
		auto test_is_base_of(int) -> decltype(test_ptr_conv<Base>(static_cast<Derived*>(nullptr)));
		template <typename, typename>
		auto test_is_base_of(...) -> true_type; // Private or ambiguous base.
	} // namespace internal

	template <typename Base, typename Derived>
	struct is_base_of :
		bool_constant<
			is_class_v<Base> && is_class_v<Derived>&& decltype(internal::test_is_base_of<Base, Derived>(0))::value>
	{
	};

	template <typename Base, typename Derived>
	inline constexpr bool is_base_of_v = is_base_of<Base, Derived>::value;

	// Implementation details of is_convertible<...>
	namespace internal
	{
		template <typename T>
		auto test_returnable(int) -> decltype(void(static_cast<T (*)()>(nullptr)), true_type{});
		template <typename>
		auto test_returnable(...) -> false_type;

		template <typename From, typename To>
		auto test_implicitly_convertible(int
		) -> decltype(void(::rsl::declval<void (&)(To)>()(::rsl::declval<From>())), true_type{});
		template <typename, typename>
		auto test_implicitly_convertible(...) -> false_type;
	} // namespace internal

	template <typename From, typename To>
	struct is_convertible :
		bool_constant<
			(decltype(internal::test_returnable<To>(0))::value &&
			 decltype(internal::test_implicitly_convertible<From, To>(0))::value) ||
			(is_void_v<From> && is_void_v<To>)>
	{
	};

	template <typename From, typename To>
	inline constexpr bool is_convertible_v = is_convertible<From, To>::value;

	template <typename... T>
	struct common_type;

	template <typename... T>
	using common_type_t = typename common_type<T...>::type;

	template <>
	struct common_type<>
	{
	};

	template <typename T1>
	struct common_type<T1> : common_type<T1, T1>
	{
	};

    // Implementation details of common_type<...>
    namespace internal
	{
        // Type of the outcome of a ternary statment if a ternary statement is valid.
		template <typename T1, typename T2>
		using ternary_result_test = decltype(false ? ::rsl::declval<T1>() : ::rsl::declval<T2>());

		template <typename T1, typename T2, typename = void>
		struct ternary_result_type
		{
            // No common type can be found, ternary statement isn't valid.
		};

		template <typename T1, typename T2>
		struct ternary_result_type<T1, T2, void_t<ternary_result_test<const T1&, const T2&>>>
		{
            // Common type if valid.
			using type = remove_cvr_t<ternary_result_test<const T1&, const T2&>>;
		};

		template <typename T1, typename T2, typename = void>
		struct decayed_ternary_result_type : ternary_result_type<T1, T2>
		{
		};

		template <typename T1, typename T2>
		struct decayed_ternary_result_type<T1, T2, void_t<ternary_result_test<T1, T2>>>
		{
            // Decayed common type.
			using type = decay_t<ternary_result_test<T1, T2>>;
		};

		template <typename T1, typename T2, typename _Decayed1 = decay_t<T1>, typename _Decayed2 = decay_t<T2>>
		struct common_type_2_types : common_type<_Decayed1, _Decayed2>
		{
		};

		template <typename T1, typename T2>
		struct common_type_2_types<T1, T2, T1, T2> : decayed_ternary_result_type<T1, T2>
		{
		};

		template <typename _Void, typename T1, typename T2, typename... Rest>
		struct common_type_multi
		{
		};

		template <typename T1, typename T2, typename... Rest>
		struct common_type_multi<void_t<common_type_t<T1, T2>>, T1, T2, Rest...> :
			common_type<common_type_t<T1, T2>, Rest...>
		{
		};
	} // namespace internal

	template <typename T1, typename T2>
	struct common_type<T1, T2> : internal::common_type_2_types<T1, T2>
	{
	};

	template <typename T1, typename T2, typename... Rest>
	struct common_type<T1, T2, Rest...> : internal::common_type_multi<void, T1, T2, Rest...>
	{
	};

	// Implementation details of copy_qualifiers<...>
	namespace internal
	{
		template <typename From>
		struct copy_qualifiers_impl
		{
			template <typename To>
			using result = To;
		};
		template <typename From>
		struct copy_qualifiers_impl<const From>
		{
			template <typename To>
			using result = const To;
		};
		template <typename From>
		struct copy_qualifiers_impl<volatile From>
		{
			template <typename To>
			using result = volatile To;
		};
		template <typename From>
		struct copy_qualifiers_impl<const volatile From>
		{
			template <typename To>
			using result = const volatile To;
		};
	} // namespace internal

	template <typename From, typename To>
	using copy_qualifiers_t = internal::copy_qualifiers_impl<From>::template result<To>;

	template <typename From, typename To>
	struct copy_qualifiers
	{
		using type = internal::copy_qualifiers_impl<From>::template result<To>;
	};

	template <typename, typename, template <typename> typename, template <typename> typename>
	struct basic_common_reference
	{
	};

	template <typename...>
	struct common_reference;

	template <typename... _Types>
	using common_reference_t = common_reference<_Types...>::type;

	template <>
	struct common_reference<>
	{
	};

	template <typename T>
	struct common_reference<T>
	{
		using type = T;
	};

	namespace internal
	{
		template <typename _Ty1>
		struct _Add_qualifiers
		{ // _Add_qualifiers<A>::template _Apply is XREF(A) from N4950 [meta.trans.other]/2.2
			template <typename _Ty2>
			using _Apply = copy_qualifiers_t<_Ty1, _Ty2>;
		};
		template <typename _Ty1>
		struct _Add_qualifiers<_Ty1&>
		{
			template <typename _Ty2>
			using _Apply = add_lval_ref_t<copy_qualifiers_t<_Ty1, _Ty2>>;
		};
		template <typename _Ty1>
		struct _Add_qualifiers<_Ty1&&>
		{
			template <typename _Ty2>
			using _Apply = add_rval_ref_t<copy_qualifiers_t<_Ty1, _Ty2>>;
		};

		template <typename _Ty1, typename _Ty2>
		using _Cond_res = // N4950 [meta.trans.other]/2.4
			decltype(false ? ::rsl::returns_exactly<_Ty1>() : ::rsl::returns_exactly<_Ty2>());
		// N4950 [meta.trans.other]/5.3: "...if sizeof...(T) is two..."

		// N4950 [meta.trans.other]/5.3.4: "if common_type_t<T1, T2> is well-formed..."
		// N4950 [meta.trans.other]/5.3.5: "Otherwise, there shall be no member type."
		template <typename _Ty1, typename _Ty2, typename = void>
		struct _Common_reference2C : common_type<_Ty1, _Ty2>
		{
		};

		// N4950 [meta.trans.other]/5.3.3: "if COND_RES(T1, T2) is well-formed..."
		template <typename _Ty1, typename _Ty2>
		struct _Common_reference2C<_Ty1, _Ty2, void_t<_Cond_res<_Ty1, _Ty2>>>
		{
			using type = _Cond_res<_Ty1, _Ty2>;
		};

		// N4950 [meta.trans.other]/5.3.2: "if basic_common_reference<[...]>::type is well-formed..."
		template <typename _Ty1, typename _Ty2>
		using _Basic_specialization = basic_common_reference<
			remove_cvr_t<_Ty1>, remove_cvr_t<_Ty2>, _Add_qualifiers<_Ty1>::template _Apply,
			_Add_qualifiers<_Ty2>::template _Apply>::type;

		template <typename _Ty1, typename _Ty2, typename = void>
		struct _Common_reference2B : _Common_reference2C<_Ty1, _Ty2>
		{
		};

		template <typename _Ty1, typename _Ty2>
		struct _Common_reference2B<_Ty1, _Ty2, void_t<_Basic_specialization<_Ty1, _Ty2>>>
		{
			using type = _Basic_specialization<_Ty1, _Ty2>;
		};

		// N4950 [meta.trans.other]/5.3.1: "Let R be COMMON-REF(T1, T2). If T1 and T2 are reference types, R is
		// well-formed, and is_convertible_v<add_pointer_t<T1>, add_pointer_t<R>> && is_convertible_v<add_pointer_t<T2>,
		// add_pointer_t<R>> is true, then the member typedef type denotes R."
		template <typename _Ty1, typename _Ty2, typename = void>
		struct _Common_reference2A : _Common_reference2B<_Ty1, _Ty2>
		{
		};

		template <
			typename _Ty1, typename _Ty2,
			typename _Result = _Cond_res<copy_qualifiers_t<_Ty1, _Ty2>&, copy_qualifiers_t<_Ty2, _Ty1>&>,
			enable_if_t<is_lvalue_reference_v<_Result>, int> = 0>
		using _LL_common_ref = _Result;

		template <typename _Ty1, typename _Ty2, typename = void>
		struct _Common_reference2AX
		{
		};

		template <typename _Ty1, typename _Ty2>
		struct _Common_reference2AX<_Ty1&, _Ty2&, void_t<_LL_common_ref<_Ty1, _Ty2>>>
		{
			using type = _LL_common_ref<_Ty1, _Ty2>; // "both lvalues" case from N4950 [meta.trans.other]/2.5
		};

		template <typename _Ty1, typename _Ty2>
		struct _Common_reference2AX<
			_Ty1&&, _Ty2&, enable_if_t<is_convertible_v<_Ty1&&, _LL_common_ref<const _Ty1, _Ty2>>>>
		{
			using type = _LL_common_ref<const _Ty1, _Ty2>; // "rvalue and lvalue" case from N4950 [meta.trans.other]/2.7
		};

		template <typename _Ty1, typename _Ty2>
		struct _Common_reference2AX<
			_Ty1&, _Ty2&&, enable_if_t<is_convertible_v<_Ty2&&, _LL_common_ref<const _Ty2, _Ty1>>>>
		{
			using type = _LL_common_ref<const _Ty2, _Ty1>; // "lvalue and rvalue" case from N4950 [meta.trans.other]/2.8
		};

		template <typename _Ty1, typename _Ty2>
		using _RR_common_ref = remove_reference_t<_LL_common_ref<_Ty1, _Ty2>>&&;

		template <typename _Ty1, typename _Ty2>
		struct _Common_reference2AX<
			_Ty1&&, _Ty2&&,
			enable_if_t<
				is_convertible_v<_Ty1&&, _RR_common_ref<_Ty1, _Ty2>> &&
				is_convertible_v<_Ty2&&, _RR_common_ref<_Ty1, _Ty2>>>>
		{
			using type = _RR_common_ref<_Ty1, _Ty2>; // "both rvalues" case from N4950 [meta.trans.other]/2.6
		};

		template <typename _Ty1, typename _Ty2>
		using _Common_ref_2AX_t = _Common_reference2AX<_Ty1, _Ty2>::type;

		template <typename _Ty1, typename _Ty2>
		struct _Common_reference2A<
			_Ty1, _Ty2,
			enable_if_t<
				is_convertible_v<add_pointer_t<_Ty1>, add_pointer_t<_Common_ref_2AX_t<_Ty1, _Ty2>>> &&
				is_convertible_v<add_pointer_t<_Ty2>, add_pointer_t<_Common_ref_2AX_t<_Ty1, _Ty2>>>>>
		{
			using type = _Common_ref_2AX_t<_Ty1, _Ty2>;
		};

		// N4950 [meta.trans.other]/5.4: "if sizeof...(T) is greater than two..."
		template <typename _Void, typename _Ty1, typename _Ty2, typename... _Types>
		struct _Fold_common_reference
		{
		};
		template <typename _Ty1, typename _Ty2, typename... _Types>
		struct _Fold_common_reference<void_t<common_reference_t<_Ty1, _Ty2>>, _Ty1, _Ty2, _Types...> :
			common_reference<common_reference_t<_Ty1, _Ty2>, _Types...>
		{
		};
	} // namespace internal

	template <typename _Ty1, typename _Ty2>
	struct common_reference<_Ty1, _Ty2> : internal::_Common_reference2A<_Ty1, _Ty2>
	{
	};

	template <typename _Ty1, typename _Ty2, typename _Ty3, typename... Rest>
	struct common_reference<_Ty1, _Ty2, _Ty3, Rest...> : internal::_Fold_common_reference<void, _Ty1, _Ty2, _Ty3, Rest...>
	{
	};

	template <typename T>
	[[nodiscard]] constexpr T* addressof(T& val) noexcept
	{
		return ::std::addressof(val); // Compiler magic behind the scenes.
	}

	template <typename T>
	const T* addressof(const T&&) = delete;

	template <typename T>
	[[nodiscard]] constexpr T&& forward(remove_reference_t<T>& val) noexcept
	{
		return static_cast<T&&>(val);
	}

	template <typename T>
	[[nodiscard]] constexpr T&& forward(remove_reference_t<T>&& val) noexcept
	{
		static_assert(!is_lvalue_reference_v<T>, "bad forward call");
		return static_cast<T&&>(val);
	}

	template <typename T>
	[[nodiscard]] constexpr remove_reference_t<T>&& move(T&& val) noexcept
	{
		return static_cast<remove_reference_t<T>&&>(val);
	}

	template <typename T>
	[[nodiscard]] constexpr conditional_t<
		!::std::is_nothrow_move_constructible_v<T> && ::std::is_copy_constructible_v<T>, const T&, T&&>
	move_if_noexcept(T& val) noexcept
	{
		return ::rsl::move(val);
	}
} // namespace rsl

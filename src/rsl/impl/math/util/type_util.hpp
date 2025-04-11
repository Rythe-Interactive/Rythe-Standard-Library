#pragma once

#include "../../util/common.hpp"
#include "../../util/type_util.hpp"
#include "../matrix/matrix_base.hpp"
#include "../vector/swizzle/swizzle_base.hpp"
#include "../vector/vector_base.hpp"

namespace rsl::math
{
	template <arithmetic_type Scalar, storage_mode Mode>
	struct quaternion;

	template <arithmetic_type Scalar, size_type RowCount, size_type ColCount, size_type ColIdx, storage_mode Mode>
	struct row;

	namespace internal
	{
		template <typename T>
		struct _is_vector_impl : false_type
		{
		};

		template <typename Scalar, size_type Size, storage_mode Mode>
		struct _is_vector_impl<vector<Scalar, Size, Mode>> : true_type
		{
		};

		template <typename Scalar, size_type Size, storage_mode Mode, size_type... args>
		struct _is_vector_impl<swizzle<Scalar, Size, Mode, args...>> : true_type
		{
		};

		template <typename Scalar, size_type RowCount, size_type ColCount, size_type ColIdx, storage_mode Mode>
		struct _is_vector_impl<row<Scalar, RowCount, ColCount, ColIdx, Mode>> : true_type
		{
		};
	} // namespace internal

	template <typename T>
	struct is_vector : internal::_is_vector_impl<remove_cvr_t<T>>
	{
	};

	template <typename T>
	constexpr static bool is_vector_v = is_vector<T>::value;

	template <typename T>
	concept vector_type = is_vector_v<T>;

	namespace internal
	{
		template <typename T>
		[[nodiscard]] constexpr auto _extract_item_(T&& v, size_type i)
		{
			if constexpr (is_vector_v<T>)
			{
				return v[i];
			}
			else
			{
				return forward<T>(v);
			}
		}

		template <typename T>
		auto _make_vector_impl()
		{
			if constexpr (is_vector_v<T>)
			{
				return vector<typename T::scalar, T::size>{};
			}
			else if constexpr (is_arithmetic_v<T>)
			{
				return vector<T, 1>{};
			}
			else
			{
				static_assert(always_false_v<T>, "Type cannot be made into a vector.");
			}
		}

		template <typename T>
		auto _decay_vector_impl()
		{
			if constexpr (is_vector_v<T>)
			{
				if constexpr (T::size == 1)
				{
					return typename T::scalar{};
				}
				else
				{
					return T{};
				}
			}
			else
			{
				return T{};
			}
		}
	} // namespace internal

	template <typename T>
	struct make_vector
	{
		using type = remove_cvr_t<decltype(internal::_make_vector_impl<remove_cvr_t<T>>())>;
	};

	template <typename T>
	using make_vector_t = typename make_vector<T>::type;

	template <typename T>
	struct is_vector_or_scalar
	{
		constexpr static bool value = !is_same_v<void, typename make_vector_t<T>::scalar>;
	};

	template <typename T>
	constexpr static bool is_vector_or_scalar_v = is_vector_or_scalar<T>::value;

	template <typename T>
	struct vector_scalar
	{
		using type = typename make_vector_t<T>::scalar;
	};

	template <typename T>
	using vector_scalar_t = typename vector_scalar<T>::type;


	template <typename T>
	struct vector_size
	{
		constexpr static size_type value = make_vector_t<T>::size;
	};

	template <typename T>
	constexpr static size_type vector_size_v = vector_size<T>::value;

	template <typename T>
	struct decay_vector
	{
		using type = remove_cvr_t<decltype(internal::_decay_vector_impl<remove_cvr_t<T>>())>;
	};

	template <typename T>
	using decay_vector_t = typename decay_vector<T>::type;

	namespace internal
	{
		template <typename T>
		struct _is_matrix_impl : false_type
		{
		};

		template <arithmetic_type Scalar, size_type RowCount, size_type ColCount, storage_mode Mode>
		struct _is_matrix_impl<matrix<Scalar, RowCount, ColCount, Mode>> : true_type
		{
		};
	} // namespace internal

	template <typename T>
	struct is_matrix : internal::_is_matrix_impl<remove_cvr_t<T>>
	{
	};

	template <typename T>
	constexpr static bool is_matrix_v = is_matrix<T>::value;

	template <typename T>
	concept matrix_type = is_matrix_v<T>;

	namespace internal
	{
		template <typename T>
		auto _make_matrix_impl()
		{
			if constexpr (is_matrix_v<T>)
			{
				return matrix<typename T::scalar, T::row_count, T::col_count>{};
			}
			else if constexpr (is_arithmetic_v<T>)
			{
				return matrix<T, 1, 1>{};
			}
			else
			{
				static_assert(always_false_v<T>, "Type cannot be made into a matrix.");
			}
		}
	} // namespace internal

	template <typename T>
	struct make_matrix
	{
		using type = remove_cvr_t<decltype(internal::_make_matrix_impl<remove_cvr_t<T>>())>;
	};

	template <typename T>
	using make_matrix_t = typename make_matrix<T>::type;

	namespace internal
	{
		template <typename T>
		struct _is_quat_impl : false_type
		{
		};

		template <arithmetic_type Scalar, storage_mode Mode>
		struct _is_quat_impl<quaternion<Scalar, Mode>> : true_type
		{
		};
	} // namespace internal

	template <typename T>
	struct is_quat : internal::_is_quat_impl<remove_cvr_t<T>>
	{
	};

	template <typename T>
	constexpr static bool is_quat_v = is_quat<T>::value;

	template <typename T>
	concept quat_type = is_quat_v<T>;

	namespace internal
	{
		template <typename T>
		auto _make_quat_impl()
		{
			if constexpr (is_quat_v<T>)
			{
				return quaternion<typename T::scalar, T::mode>{};
			}
			else if constexpr (is_arithmetic_v<T>)
			{
				return quaternion<T, storage_mode::defaultp>{};
			}
			else
			{
				static_assert(always_false_v<T>, "Type cannot be made into a quaternion.");
			}
		}
	} // namespace internal

	template <typename T>
	struct make_quat
	{
		using type = remove_cvr_t<decltype(internal::_make_quat_impl<remove_cvr_t<T>>())>;
	};

	template <typename T>
	using make_quat_t = typename make_quat<T>::type;

	template <typename T>
	struct is_linear_algebraic_construct
	{
		constexpr static bool value = is_vector_v<T> || is_matrix_v<T> || is_quat_v<T>;
	};

	template <typename T>
	constexpr bool is_linear_algebraic_construct_v = is_linear_algebraic_construct<T>::value;

	template <typename T>
	concept linear_algebraic_construct = is_linear_algebraic_construct_v<T>;

	namespace internal
	{
		template <arithmetic_type T, size_type v>
		struct _uniform_value_impl_;

		template <arithmetic_type Scalar, size_type Size, storage_mode Mode, size_type v>
		struct _uniform_value_impl_<vector<Scalar, Size, Mode>, v>
		{
			constexpr static auto value = vector<Scalar, Size, Mode>(static_cast<Scalar>(v));
		};

		template <arithmetic_type Scalar, size_type Size, storage_mode Mode, size_type... args, size_type v>
		struct _uniform_value_impl_<swizzle<Scalar, Size, Mode, args...>, v>
		{
			constexpr static auto value = vector<Scalar, Size, Mode>(static_cast<Scalar>(v));
		};

		template <
			arithmetic_type Scalar, size_type RowCount, size_type ColCount, size_type ColIdx, storage_mode Mode,
			size_type v>
		struct _uniform_value_impl_<row<Scalar, RowCount, ColCount, ColIdx, Mode>, v>
		{
			constexpr static auto value = vector<Scalar, RowCount, Mode>(static_cast<Scalar>(v));
		};

		template <arithmetic_type Scalar, storage_mode Mode, size_type v>
		struct _uniform_value_impl_<quaternion<Scalar, Mode>, v>
		{
			constexpr static auto value = quaternion<Scalar, Mode>(
				static_cast<Scalar>(v), static_cast<Scalar>(v), static_cast<Scalar>(v), static_cast<Scalar>(v)
			);
		};

		template <arithmetic_type Scalar, size_type RowCount, size_type ColCount, storage_mode Mode, size_type v>
		struct _uniform_value_impl_<matrix<Scalar, RowCount, ColCount, Mode>, v>
		{
			constexpr static auto value =
				matrix<Scalar, RowCount, ColCount>(static_cast<Scalar>(v), uniform_matrix_signal{});
		};
	} // namespace internal

	template <arithmetic_type T, size_type v>
	struct uniform_value : internal::_uniform_value_impl_<remove_cvr_t<T>, v>
	{
	};

	template <arithmetic_type T>
	constexpr auto zero = uniform_value<T, 0>::value;

	template <arithmetic_type T>
	constexpr auto one = uniform_value<T, 1>::value;

	template <arithmetic_type T, size_type v>
	constexpr auto uniform_value_v = uniform_value<T, v>::value;

	namespace internal
	{
		template <typename FPType>
		struct _epsilon_fp_impl
		{
			static constexpr auto value = ::std::numeric_limits<FPType>::epsilon();
		};

		template <typename IntType>
		struct _epsilon_int_impl : integral_constant<IntType, 1>
		{
		};

		template <typename FPType>
		struct _epsilon_impl :
			conditional_t<is_floating_point_v<FPType>, _epsilon_fp_impl<FPType>, _epsilon_int_impl<FPType>>
		{
		};
	} // namespace internal

	template <typename FPType>
	struct epsilon : internal::_epsilon_impl<remove_cvr_t<FPType>>
	{
	};

	template <typename FPType>
	constexpr auto epsilon_v = epsilon<FPType>::value;

	namespace internal
	{
		template <size_type size>
		constexpr auto _int_least_impl()
		{
			if constexpr (size <= 1)
			{
				return int_least8_t();
			}
			else if constexpr (size <= 2)
			{
				return int_least16_t();
			}
			else if constexpr (size <= 4)
			{
				return int_least32_t();
			}
			else if constexpr (size <= 8)
			{
				return int_least64_t();
			}
			else
			{
				static_assert(size <= 8, "unsupported type size");
			}
		}

		template <typename TypeA, typename TypeB>
		constexpr auto _highest_epsilon_impl()
		{
			using A = remove_cvr_t<TypeA>;
			using B = remove_cvr_t<TypeB>;
			if constexpr (epsilon_v<A> > epsilon_v<B>)
			{
				return epsilon_v<A>;
			}
			else
			{
				return epsilon_v<B>;
			}
		}

		template <typename TypeA, typename TypeB>
		constexpr auto _lowest_epsilon_impl()
		{
			using A = remove_cvr_t<TypeA>;
			using B = remove_cvr_t<TypeB>;
			if constexpr (epsilon_v<A> < epsilon_v<B>)
			{
				return epsilon_v<A>;
			}
			else
			{
				return epsilon_v<B>;
			}
		}
	} // namespace internal

	template <size_type size>
	using int_least = decltype(internal::_int_least_impl<size>());

	template <size_type size>
	using uint_least = std::make_unsigned_t<int_least<size>>;

	template <typename TypeA, typename TypeB>
	struct highest_epsilon
	{
		constexpr static auto value = internal::_highest_epsilon_impl<TypeA, TypeB>();
	};

	template <typename TypeA, typename TypeB>
	constexpr auto highest_epsilon_v = highest_epsilon<TypeA, TypeB>::value;

	template <typename TypeA, typename TypeB>
	struct lowest_epsilon
	{
		constexpr static auto value = internal::_lowest_epsilon_impl<TypeA, TypeB>();
	};

	template <typename TypeA, typename TypeB>
	constexpr auto lowest_epsilon_v = lowest_epsilon<TypeA, TypeB>::value;

	template <typename TypeA, typename TypeB>
	struct lowest_precision
	{
		using type = remove_cvr_t<decltype(highest_epsilon_v<TypeA, TypeB>)>;
	};

	template <typename TypeA, typename TypeB>
	using lowest_precision_t = typename lowest_precision<TypeA, TypeB>::type;

	template <typename TypeA, typename TypeB>
	struct highest_precision
	{
		using type = remove_cvr_t<decltype(lowest_epsilon_v<TypeA, TypeB>)>;
	};

	template <typename TypeA, typename TypeB>
	using highest_precision_t = typename highest_precision<TypeA, TypeB>::type;

	template <typename TypeA, typename TypeB>
	struct select_floating_type :
		conditional<is_floating_point_v<TypeA> || !is_floating_point_v<TypeB>, remove_cvr_t<TypeA>, remove_cvr_t<TypeB>>
	{
	};

	template <typename TypeA, typename TypeB>
	using select_floating_type_t = typename select_floating_type<TypeA, TypeB>::type;

	template <typename TypeA, typename TypeB>
	struct select_vector_type
	{
		using type = make_vector_t<
			conditional_t<is_vector_v<TypeA> || !is_vector_v<TypeB>, remove_cvr_t<TypeA>, remove_cvr_t<TypeB>>>;
	};

	template <typename TypeA, typename TypeB>
	using select_vector_type_t = typename select_vector_type<TypeA, TypeB>::type;

	template <typename TypeA, typename TypeB>
	struct select_matrix_type
	{
		using type = make_matrix_t<
			conditional_t<is_matrix_v<TypeA> || !is_matrix_v<TypeB>, remove_cvr_t<TypeA>, remove_cvr_t<TypeB>>>;
	};

	template <typename TypeA, typename TypeB>
	using select_matrix_type_t = typename select_matrix_type<TypeA, TypeB>::type;


	template <typename T>
	struct make_signed : ::std::make_signed<T>
	{
	};

	template <>
	struct make_signed<float>
	{
		using type = float;
	};

	template <>
	struct make_signed<double>
	{
		using type = double;
	};

	template <>
	struct make_signed<long double>
	{
		using type = long double;
	};

	template <typename T>
	using make_signed_t = typename make_signed<T>::type;


	template <typename T>
	struct make_unsigned : ::std::make_unsigned<T>
	{
	};

	template <>
	struct make_unsigned<float>
	{
		using type = float;
	};

	template <>
	struct make_unsigned<double>
	{
		using type = double;
	};

	template <>
	struct make_unsigned<long double>
	{
		using type = long double;
	};

	template <typename T>
	using make_unsigned_t = typename make_unsigned<T>::type;

	template <typename A, typename B>
	struct largest : conditional<(sizeof(A) > sizeof(B)), A, B>
	{
	};

	template <typename A, typename B>
	using largest_t = typename largest<A, B>::type;

	template <typename A, typename B>
	struct smallest : conditional<(sizeof(A) < sizeof(B)), A, B>
	{
	};

	template <typename A, typename B>
	using smallest_t = typename smallest<A, B>::type;

	namespace internal
	{
		template <typename A, typename B>
		struct _elevated_impl;

		template <arithmetic_type A, arithmetic_type B>
			requires floating_point_type<A> || floating_point_type<B>
		struct _elevated_impl<A, B> : highest_precision<A, B>
		{
		};

		template <vector_type A, vector_type B>
		struct _elevated_impl<A, B>
		{
			using type = vector<
				typename _elevated_impl<typename A::scalar, typename B::scalar>::type,
				(A::size < B::size ? A::size : B::size), (A::mode < B::mode ? A::mode : B::mode)>;
		};

		template <matrix_type A, matrix_type B>
		struct _elevated_impl<A, B>
		{
			using type = matrix<
				typename _elevated_impl<typename A::scalar, typename B::scalar>::type,
				(A::row_count < B::row_count ? A::row_count : B::row_count),
				(A::col_count < B::col_count ? A::col_count : B::col_count), (A::mode < B::mode ? A::mode : B::mode)>;
		};

		template <quat_type A, quat_type B>
		struct _elevated_impl<A, B>
		{
			using type = quaternion<
				typename _elevated_impl<typename A::scalar, typename B::scalar>::type,
				(A::mode < B::mode ? A::mode : B::mode)>;
		};

		template <integral_type A, integral_type B>
		struct _elevated_impl<A, B> :
			conditional_t<
				::std::is_signed_v<A> || ::std::is_signed_v<B>, make_signed<largest_t<A, B>>,
				make_unsigned<largest_t<A, B>>>
		{
		};
	} // namespace internal

	template <typename A, typename B>
	struct elevated : internal::_elevated_impl<remove_cvr_t<A>, remove_cvr_t<B>>
	{
	};

	template <typename A, typename B>
	using elevated_t = typename elevated<A, B>::type;
} // namespace rsl::math

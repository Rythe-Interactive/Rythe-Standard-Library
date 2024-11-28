#pragma once
#include "../../util/primitives.hpp"
#include "../basic/constraint.hpp"
#include "../matrix/matrix.hpp"
#include "../quaternion/quaternion_base.hpp"
#include "../util/type_util.hpp"
#include "../vector/vector.hpp"

namespace rsl::math
{
	template <typename TypeA, typename TypeB>
	[[nodiscard]] [[rythe_always_inline]] constexpr auto mul(TypeA&& a, TypeB&& b) noexcept;

	template <linear_algebraic_construct TypeA, linear_algebraic_construct TypeB>
	[[nodiscard]] [[rythe_always_inline]] constexpr auto operator*(TypeA&& a, TypeB&& b) noexcept;

	template <typename TypeA, typename TypeB>
		requires(linear_algebraic_construct<TypeA> && arithmetic_type<TypeB>) ||
				(linear_algebraic_construct<TypeB> && arithmetic_type<TypeA>)
	[[nodiscard]] [[rythe_always_inline]] constexpr auto operator*(TypeA&& a, TypeB&& b) noexcept;

	template <linear_algebraic_construct TypeA, linear_algebraic_construct TypeB>
	[[rythe_always_inline]] constexpr TypeA& mul_assign(TypeA& a, TypeB&& b) noexcept;

	template <typename TypeA, typename TypeB>
		requires(linear_algebraic_construct<TypeA> && arithmetic_type<TypeB>) ||
				(linear_algebraic_construct<TypeB> && arithmetic_type<TypeA>)
	[[rythe_always_inline]] constexpr TypeA& mul_assign(TypeA& a, TypeB&& b) noexcept;

	template <linear_algebraic_construct TypeA, linear_algebraic_construct TypeB>
	[[rythe_always_inline]] constexpr TypeA& operator*=(TypeA& a, TypeB&& b) noexcept;

	template <typename TypeA, typename TypeB>
		requires(linear_algebraic_construct<TypeA> && arithmetic_type<TypeB>) ||
				(linear_algebraic_construct<TypeB> && arithmetic_type<TypeA>)
	[[rythe_always_inline]] constexpr TypeA& operator*=(TypeA& a, TypeB&& b) noexcept;
} // namespace rsl::math

#include "multiplication.inl"

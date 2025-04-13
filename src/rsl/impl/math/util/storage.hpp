#pragma once

#include "../../defines.hpp"
#include "../../util/common.hpp"
#include "../../util/concepts.hpp"
#include "../../util/primitives.hpp"

namespace rsl::math
{
#ifndef RSL_DEFAULT_ALIGNED_MATH
	#define RSL_DEFAULT_ALIGNED_MATH false
#endif

#ifndef RSL_DEFAULT_MATH_MODE
	#define RSL_DEFAULT_MATH_MODE lowp
#endif

	enum struct storage_mode : uint8
	{
		packed_highp,
		packed_mediump,
		packed_lowp,
		aligned_highp,
		aligned_mediump,
		aligned_lowp,
#if RSL_DEFAULT_ALIGNED_MATH
		highp = aligned_highp,
		mediump = aligned_mediump,
		lowp = aligned_lowp,
#else
		highp = packed_highp,
		mediump = packed_mediump,
		lowp = packed_lowp,
#endif
		defaultp = RSL_DEFAULT_MATH_MODE,
	};

	template <storage_mode M>
	struct is_aligned : false_type
	{
	};

	template <>
	struct is_aligned<storage_mode::aligned_highp> : true_type
	{
	};

	template <>
	struct is_aligned<storage_mode::aligned_mediump> : true_type
	{
	};

	template <>
	struct is_aligned<storage_mode::aligned_lowp> : true_type
	{
	};

	template <storage_mode M>
	constexpr bool is_aligned_v = is_aligned<M>::value;

	template <arithmetic_type T, size_type N, bool Aligned>
	struct storage;

	template <arithmetic_type T, size_type N>
	struct storage<T, N, false>
	{
		struct type
		{
			T data[N];

			[[nodiscard]] [[rythe_always_inline]] constexpr T& operator[](size_type i) noexcept { return data[i]; }
			[[nodiscard]] [[rythe_always_inline]] constexpr const T& operator[](size_type i) const noexcept
			{
				return data[i];
			}
		};
	};

	template <arithmetic_type T, size_type N>
	struct storage<T, N, true>
	{
		struct alignas(N * sizeof(T)) type
		{
			T data[N];

			[[nodiscard]] [[rythe_always_inline]] constexpr T& operator[](size_type i) noexcept { return data[i]; }
			[[nodiscard]] [[rythe_always_inline]] constexpr const T& operator[](size_type i) const noexcept
			{
				return data[i];
			}
		};
	};

	template <arithmetic_type T>
	struct storage<T, 3, true>
	{
		struct alignas(4 * sizeof(T)) type
		{
			T data[4];

			[[nodiscard]] [[rythe_always_inline]] constexpr T& operator[](size_type i) noexcept { return data[i]; }
			[[nodiscard]] [[rythe_always_inline]] constexpr const T& operator[](size_type i) const noexcept
			{
				return data[i];
			}
		};
	};

	template <arithmetic_type T, size_type N, storage_mode M>
	using storage_t = typename storage<T, N, is_aligned_v<M>>::type;
} // namespace rsl::math

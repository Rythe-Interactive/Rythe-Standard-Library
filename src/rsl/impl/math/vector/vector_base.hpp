#pragma once
#include "../../util/assert.hpp"
#include "predefined.hpp"

namespace rsl::math
{
	template <arithmetic_type Scalar, size_type Size, mode Mode = mode::defaultp>
	struct vector
	{
		using scalar = Scalar;
		static constexpr size_type size = Size;
		static constexpr mode mode = Mode;
		using storage_type = storage_t<Scalar, Size, Mode>;

		storage_type data;

		static const vector one;
		static const vector zero;

		[[rythe_always_inline]] constexpr vector() noexcept;
		[[rythe_always_inline]] constexpr vector(const vector&) noexcept = default;
		[[rythe_always_inline]] explicit constexpr vector(scalar s) noexcept;

		template <typename vec_type>
			requires not_same_as<Scalar, typename vec_type::scalar> || (vec_type::size != Size)
		[[rythe_always_inline]] constexpr vector(const vec_type& other) noexcept;

		[[rythe_always_inline]] constexpr vector& operator=(const vector&) noexcept = default;

		[[nodiscard]] [[rythe_always_inline]] constexpr scalar& operator[](size_type i) noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const scalar& operator[](size_type i) const noexcept;
	};
} // namespace rsl::math

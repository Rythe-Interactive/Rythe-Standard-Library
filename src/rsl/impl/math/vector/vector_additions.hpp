#pragma once
#include "predefined.hpp"

namespace rsl::math::internal
{
	template <arithmetic_type Scalar, size_type size, storage_mode Mode, bool isSigned = signed_type<Scalar>>
	struct vector_additions
	{
	};

	template <arithmetic_type Scalar, storage_mode Mode>
	struct vector_additions<Scalar, 2, Mode, false>
	{
		static const vector<Scalar, 2, Mode> up;
		static const vector<Scalar, 2, Mode> right;
	};

	template <arithmetic_type Scalar, storage_mode Mode>
	struct vector_additions<Scalar, 2, Mode, true>
	{
		static const vector<Scalar, 2, Mode> up;
		static const vector<Scalar, 2, Mode> down;
		static const vector<Scalar, 2, Mode> right;
		static const vector<Scalar, 2, Mode> left;
	};

	template <arithmetic_type Scalar, storage_mode Mode>
	struct vector_additions<Scalar, 3, Mode, false>
	{
		static const vector<Scalar, 3, Mode> up;
		static const vector<Scalar, 3, Mode> right;
		static const vector<Scalar, 3, Mode> forward;
	};

	template <arithmetic_type Scalar, storage_mode Mode>
	struct vector_additions<Scalar, 3, Mode, true>
	{
		static const vector<Scalar, 3, Mode> up;
		static const vector<Scalar, 3, Mode> down;
		static const vector<Scalar, 3, Mode> right;
		static const vector<Scalar, 3, Mode> left;
		static const vector<Scalar, 3, Mode> forward;
		static const vector<Scalar, 3, Mode> backward;
	};

	template <arithmetic_type Scalar, storage_mode Mode>
	struct vector_additions<Scalar, 4, Mode, false>
	{
		static const vector<Scalar, 4, Mode> up;
		static const vector<Scalar, 4, Mode> right;
		static const vector<Scalar, 4, Mode> forward;
		static const vector<Scalar, 4, Mode> positiveW;
	};

	template <arithmetic_type Scalar, storage_mode Mode>
	struct vector_additions<Scalar, 4, Mode, true>
	{
		static const vector<Scalar, 4, Mode> up;
		static const vector<Scalar, 4, Mode> down;
		static const vector<Scalar, 4, Mode> right;
		static const vector<Scalar, 4, Mode> left;
		static const vector<Scalar, 4, Mode> forward;
		static const vector<Scalar, 4, Mode> backward;
		static const vector<Scalar, 4, Mode> positiveW;
		static const vector<Scalar, 4, Mode> negativeW;
	};
} // namespace rsl::math::internal

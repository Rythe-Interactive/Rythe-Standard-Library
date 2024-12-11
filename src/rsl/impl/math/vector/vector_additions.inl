#pragma once
#include "vector_additions.hpp"

namespace rsl::math::internal
{
	template <arithmetic_type Scalar, mode Mode>
	const vector<Scalar, 2, Mode> vector_additions<Scalar, 2, Mode, false>::up(Scalar(0), Scalar(1));
	template <arithmetic_type Scalar, mode Mode>
	const vector<Scalar, 2, Mode> vector_additions<Scalar, 2, Mode, false>::right(Scalar(1), Scalar(0));

	template <arithmetic_type Scalar, mode Mode>
	const vector<Scalar, 2, Mode> vector_additions<Scalar, 2, Mode, true>::up(Scalar(0), Scalar(1));
	template <arithmetic_type Scalar, mode Mode>
	const vector<Scalar, 2, Mode> vector_additions<Scalar, 2, Mode, true>::down(Scalar(0), Scalar(-1));
	template <arithmetic_type Scalar, mode Mode>
	const vector<Scalar, 2, Mode> vector_additions<Scalar, 2, Mode, true>::right(Scalar(1), Scalar(0));
	template <arithmetic_type Scalar, mode Mode>
	const vector<Scalar, 2, Mode> vector_additions<Scalar, 2, Mode, true>::left(Scalar(-1), Scalar(0));

	template <arithmetic_type Scalar, mode Mode>
	const vector<Scalar, 3, Mode> vector_additions<Scalar, 3, Mode, false>::up(Scalar(0), Scalar(1), Scalar(0));
	template <arithmetic_type Scalar, mode Mode>
	const vector<Scalar, 3, Mode> vector_additions<Scalar, 3, Mode, false>::right(Scalar(1), Scalar(0), Scalar(0));
	template <arithmetic_type Scalar, mode Mode>
	const vector<Scalar, 3, Mode> vector_additions<Scalar, 3, Mode, false>::forward(Scalar(0), Scalar(0), Scalar(1));

	template <arithmetic_type Scalar, mode Mode>
	const vector<Scalar, 3, Mode> vector_additions<Scalar, 3, Mode, true>::up(Scalar(0), Scalar(1), Scalar(0));
	template <arithmetic_type Scalar, mode Mode>
	const vector<Scalar, 3, Mode> vector_additions<Scalar, 3, Mode, true>::down(Scalar(0), Scalar(-1), Scalar(0));
	template <arithmetic_type Scalar, mode Mode>
	const vector<Scalar, 3, Mode> vector_additions<Scalar, 3, Mode, true>::right(Scalar(1), Scalar(0), Scalar(0));
	template <arithmetic_type Scalar, mode Mode>
	const vector<Scalar, 3, Mode> vector_additions<Scalar, 3, Mode, true>::left(Scalar(-1), Scalar(0), Scalar(0));
	template <arithmetic_type Scalar, mode Mode>
	const vector<Scalar, 3, Mode> vector_additions<Scalar, 3, Mode, true>::forward(Scalar(0), Scalar(0), Scalar(1));
	template <arithmetic_type Scalar, mode Mode>
	const vector<Scalar, 3, Mode> vector_additions<Scalar, 3, Mode, true>::backward(Scalar(0), Scalar(0), Scalar(-1));

	template <arithmetic_type Scalar, mode Mode>
	const vector<Scalar, 4, Mode>
		vector_additions<Scalar, 4, Mode, false>::up(Scalar(0), Scalar(1), Scalar(0), Scalar(0));
	template <arithmetic_type Scalar, mode Mode>
	const vector<Scalar, 4, Mode>
		vector_additions<Scalar, 4, Mode, false>::right(Scalar(1), Scalar(0), Scalar(0), Scalar(0));
	template <arithmetic_type Scalar, mode Mode>
	const vector<Scalar, 4, Mode>
		vector_additions<Scalar, 4, Mode, false>::forward(Scalar(0), Scalar(0), Scalar(1), Scalar(0));
	template <arithmetic_type Scalar, mode Mode>
	const vector<Scalar, 4, Mode>
		vector_additions<Scalar, 4, Mode, false>::positiveW(Scalar(0), Scalar(0), Scalar(0), Scalar(1));

	template <arithmetic_type Scalar, mode Mode>
	const vector<Scalar, 4, Mode>
		vector_additions<Scalar, 4, Mode, true>::up(Scalar(0), Scalar(1), Scalar(0), Scalar(0));
	template <arithmetic_type Scalar, mode Mode>
	const vector<Scalar, 4, Mode>
		vector_additions<Scalar, 4, Mode, true>::down(Scalar(0), Scalar(-1), Scalar(0), Scalar(0));
	template <arithmetic_type Scalar, mode Mode>
	const vector<Scalar, 4, Mode>
		vector_additions<Scalar, 4, Mode, true>::right(Scalar(1), Scalar(0), Scalar(0), Scalar(0));
	template <arithmetic_type Scalar, mode Mode>
	const vector<Scalar, 4, Mode>
		vector_additions<Scalar, 4, Mode, true>::left(Scalar(-1), Scalar(0), Scalar(0), Scalar(0));
	template <arithmetic_type Scalar, mode Mode>
	const vector<Scalar, 4, Mode>
		vector_additions<Scalar, 4, Mode, true>::forward(Scalar(0), Scalar(0), Scalar(1), Scalar(0));
	template <arithmetic_type Scalar, mode Mode>
	const vector<Scalar, 4, Mode>
		vector_additions<Scalar, 4, Mode, true>::backward(Scalar(0), Scalar(0), Scalar(-1), Scalar(0));
	template <arithmetic_type Scalar, mode Mode>
	const vector<Scalar, 4, Mode>
		vector_additions<Scalar, 4, Mode, true>::positiveW(Scalar(0), Scalar(0), Scalar(0), Scalar(1));
	template <arithmetic_type Scalar, mode Mode>
	const vector<Scalar, 4, Mode>
		vector_additions<Scalar, 4, Mode, true>::negativeW(Scalar(0), Scalar(0), Scalar(0), Scalar(-1));
} // namespace rsl::math::internal

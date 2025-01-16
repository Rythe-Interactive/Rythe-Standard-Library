
#ifndef SizeSpecialization
	#define SizeSpecialization Size
#endif // !SizeSpecialization

#ifndef ScalarSpecialization
	#define ScalarSpecialization Scalar
#endif // !ScalarSpecialization

#ifndef VectorTemplate
	#define VectorTemplate template <arithmetic_type Scalar, size_type Size, mode Mode>
#endif // !VectorTemplate

namespace rsl::math
{
	VectorTemplate const vector<ScalarSpecialization, SizeSpecialization, Mode>
		vector<ScalarSpecialization, SizeSpecialization, Mode>::one(ScalarSpecialization(1));

	VectorTemplate const vector<ScalarSpecialization, SizeSpecialization, Mode>
		vector<ScalarSpecialization, SizeSpecialization, Mode>::zero(ScalarSpecialization(0));


	VectorTemplate constexpr vector<ScalarSpecialization, SizeSpecialization, Mode>::vector() noexcept
		: data()
	{
		for (size_type i = 0; i < size; i++)
		{
			data[i] = scalar(0);
		}
	}

	VectorTemplate constexpr vector<ScalarSpecialization, SizeSpecialization, Mode>::vector(scalar s) noexcept
		: data()
	{
		for (size_type i = 0; i < size; i++)
		{
			data[i] = s;
		}
	}

	VectorTemplate template <typename vec_type>
		requires not_same_as<ScalarSpecialization, typename vec_type::scalar> || (vec_type::size != SizeSpecialization)
	constexpr vector<ScalarSpecialization, SizeSpecialization, Mode>::vector(const vec_type& other) noexcept
		: data()
	{
		if constexpr (size > vec_type::size)
		{
			for (size_type i = 0; i < vec_type::size; i++)
			{
				data[i] = static_cast<scalar>(other.data[i]);
			}

			for (size_type i = vec_type::size; i < size; i++)
			{
				data[i] = static_cast<scalar>(0);
			}
		}
		else
		{
			for (size_type i = 0; i < size; i++)
			{
				data[i] = static_cast<scalar>(other.data[i]);
			}
		}
	}

	VectorTemplate constexpr ScalarSpecialization&
	vector<ScalarSpecialization, SizeSpecialization, Mode>::operator[](size_type i) noexcept
	{
		rsl_assert_out_of_range_msg((i >= 0) && (i < size), "vector subscript out of range");
		return data[i];
	}

	VectorTemplate constexpr const ScalarSpecialization&
	vector<ScalarSpecialization, SizeSpecialization, Mode>::operator[](size_type i) const noexcept
	{
		rsl_assert_out_of_range_msg((i >= 0) && (i < size), "vector subscript out of range");
		return data[i];
	}
} // namespace rsl::math

#undef SizeSpecialization
#undef ScalarSpecialization
#undef VectorTemplate

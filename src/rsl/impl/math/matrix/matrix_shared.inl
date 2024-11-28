#ifndef RowCountSpecialization
	#define RowCountSpecialization RowCount
#endif // !RowCountSpecialization

#ifndef ColCountSpecialization
	#define ColCountSpecialization ColCount
#endif // !ColCountSpecialization

#ifndef ScalarSpecialization
	#define ScalarSpecialization Scalar
#endif // !ScalarSpecialization

#ifndef ModeSpecialization
	#define ModeSpecialization Mode
#endif // !ModeSpecialization

#ifndef MatrixTemplate
	#define MatrixTemplate                                                                                             \
		template <                                                                                                     \
			arithmetic_type ScalarSpecialization, size_type RowCountSpecialization, size_type ColCountSpecialization,  \
			mode ModeSpecialization>
#endif // !MatrixTemplate

namespace rsl::math
{
	MatrixTemplate const matrix<ScalarSpecialization, RowCountSpecialization, ColCountSpecialization, Mode>
		matrix<ScalarSpecialization, RowCountSpecialization, ColCountSpecialization, Mode>::identity(
			ScalarSpecialization(1)
		);

	MatrixTemplate const
		matrix<ScalarSpecialization, RowCountSpecialization, ColCountSpecialization, ModeSpecialization>
			matrix<ScalarSpecialization, RowCountSpecialization, ColCountSpecialization, ModeSpecialization>::zero(
				ScalarSpecialization(0)
			);

	MatrixTemplate constexpr matrix<
		ScalarSpecialization, RowCountSpecialization, ColCountSpecialization, ModeSpecialization>::matrix() noexcept
	{
		for (size_type i = 0; i < row_count; i++)
		{
			for (size_type j = 0; j < col_count; j++)
			{
				rows[i][j] = i == j ? static_cast<scalar>(1) : static_cast<scalar>(0);
			}
		}
	}

	MatrixTemplate constexpr matrix<
		ScalarSpecialization, RowCountSpecialization, ColCountSpecialization, ModeSpecialization>::matrix(scalar s
	) noexcept
		: matrix(s, identity_matrix_signal{})
	{
	}

	MatrixTemplate constexpr matrix<
		ScalarSpecialization, RowCountSpecialization, ColCountSpecialization,
		ModeSpecialization>::matrix(scalar s, uniform_matrix_signal) noexcept
	{
		for (size_type i = 0; i < row_count; i++)
		{
			for (size_type j = 0; j < col_count; j++)
			{
				rows[i][j] = s;
			}
		}
	}

	MatrixTemplate constexpr matrix<
		ScalarSpecialization, RowCountSpecialization, ColCountSpecialization,
		ModeSpecialization>::matrix(scalar s, identity_matrix_signal) noexcept
	{
		for (size_type i = 0; i < row_count; i++)
		{
			for (size_type j = 0; j < col_count; j++)
			{
				rows[i][j] = i == j ? s : static_cast<scalar>(0);
			}
		}
	}

	MatrixTemplate template <typename mat_type>
		requires not_same_as<ScalarSpecialization, typename mat_type::scalar> ||
				 (mat_type::row_count != RowCountSpecialization) || (mat_type::col_count != ColCountSpecialization)
	constexpr matrix<ScalarSpecialization, RowCountSpecialization, ColCountSpecialization, ModeSpecialization>::matrix(
		const mat_type& other
	) noexcept
	{
		if constexpr (row_count == mat_type::row_count && col_count == mat_type::col_count)
		{
			for (size_type i = 0; i < size; i++)
			{
				data[i] = static_cast<scalar>(other.data[i]);
			}
		}
		else
		{
			constexpr size_type min_row = row_count < mat_type::row_count ? row_count : mat_type::row_count;
			constexpr size_type min_col = col_count < mat_type::col_count ? col_count : mat_type::col_count;

			for (size_type i = 0; i < min_row; i++)
			{
				for (size_type j = 0; j < min_col; j++)
				{
					rows[i][j] = static_cast<scalar>(other.mx[i][j]);
				}
			}
		}
	}

	MatrixTemplate constexpr typename matrix<
		ScalarSpecialization, RowCountSpecialization, ColCountSpecialization, ModeSpecialization>::row_type&
	matrix<ScalarSpecialization, RowCountSpecialization, ColCountSpecialization, ModeSpecialization>::operator[](
		size_type i
	) noexcept
	{
		rsl_assert_out_of_range_msg((i >= 0) && (i < row_count), "matrix subscript out of range");
		return rows[i];
	}

	MatrixTemplate constexpr const typename matrix<
		ScalarSpecialization, RowCountSpecialization, ColCountSpecialization, ModeSpecialization>::row_type&
	matrix<ScalarSpecialization, RowCountSpecialization, ColCountSpecialization, ModeSpecialization>::operator[](
		size_type i
	) const noexcept
	{
		rsl_assert_out_of_range_msg((i >= 0) && (i < row_count), "matrix subscript out of range");
		return rows[i];
	}
} // namespace rsl::math

#undef RowCountSpecialization
#undef ColCountSpecialization
#undef ScalarSpecialization
#undef ModeSpecialization
#undef MatrixTemplate

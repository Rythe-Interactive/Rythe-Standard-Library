#pragma once
#include "matrix_base.hpp"
#include "column/column_base.hpp"
#include "../vector/vector3.hpp"

namespace rsl::math
{
    template<typename Scalar>
    struct quaternion;

    template<typename Scalar>
    struct matrix<Scalar, 3, 3> : matrix_base
    {
        static constexpr size_type row_count = 3;
        static constexpr size_type col_count = 3;
        static constexpr size_type size = row_count * col_count;
        using type = matrix<Scalar, 3, 3>;

        using row_type = vector<Scalar, col_count>;

        template<size_type idx>
        using col_type = column<Scalar, row_count, col_count, idx>;

        union
        {
            row_type rows[row_count];

            struct
            {
                row_type row0, row1, row2;
            };

            Scalar data[size];
            col_type<0> col0;
            col_type<1> col1;
            col_type<2> col2;
        };

        static const matrix identity;
        static const matrix zero;

        constexpr matrix() noexcept :
            row0(float3::right),
            row1(float3::up),
            row2(float3::forward)
        {}

        constexpr matrix(const matrix&) noexcept = default;

        explicit constexpr matrix(Scalar s) noexcept :
            row0(s, static_cast<Scalar>(0), static_cast<Scalar>(0)),
            row1(static_cast<Scalar>(0), s, static_cast<Scalar>(0)),
            row2(static_cast<Scalar>(0), static_cast<Scalar>(0), s)
        {}

        explicit constexpr matrix(
            Scalar s00, Scalar s01, Scalar s02,
            Scalar s10, Scalar s11, Scalar s12,
            Scalar s20, Scalar s21, Scalar s22) noexcept :
            row0(s00, s01, s02),
            row1(s10, s11, s12),
            row2(s20, s21, s22)
        {}

        explicit constexpr matrix(row_type r0, row_type r1, row_type r2) noexcept : row0(r0), row1(r1), row2(r2) {}

        template<typename Scal>
        explicit constexpr matrix(const quaternion<Scal>& orientation) noexcept;

        template<typename Scal, ::std::enable_if_t<!::std::is_same_v<Scalar, Scal>, bool> = true>
        constexpr explicit matrix(const matrix<Scal, row_count, col_count>& other) noexcept
        {
            for (size_type i = 0; i < size; i++)
                data[i] = static_cast<Scal>(other.data[i]);
        }



        template<typename mat_type, ::std::enable_if_t<3 != mat_type::row_count || 3 != mat_type::col_count, bool> = true>
        constexpr matrix(const mat_type& other) noexcept
        {
            constexpr size_type min_row = row_count < mat_type::row_count ? row_count : mat_type::row_count;
            constexpr size_type min_col = col_count < mat_type::col_count ? col_count : mat_type::col_count;

            for (size_type i = 0; i < min_row; i++)
                for (size_type j = 0; j < min_col; j++)
                    rows[i][j] = static_cast<Scalar>(other[i][j]);
        }


        constexpr matrix& operator=(const matrix&) noexcept = default;

        [[nodiscard]] constexpr row_type& operator[](size_type i) noexcept
        {
            rsl_assert_msg((i >= 0) && (i < row_count), "matrix subscript out of range"); return rows[i];
        }
        [[nodiscard]] constexpr const row_type& operator[](size_type i) const noexcept
        {
            rsl_assert_msg((i >= 0) && (i < row_count), "matrix subscript out of range"); return rows[i];
        }
    };

    using float3x3 = matrix<float32, 3, 3>;
    using mat3 = float3x3;
    using double3x3 = matrix<float64, 3, 3>;
    using dmat3 = double3x3;
}

#include "../quaternion/matrix_quat_conv.inl"

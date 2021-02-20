#include "gtest/gtest.h"
#include "tstLinearSolver.h"

static const double epsilon = 1e-6;

namespace csim
{
    void tst_equ_real_1(LinearSolver *solver)
    {
        const int n = 2;
        Complex A[] = {
            47.0,
            28.0,
            89.0,
            53.0,
        };
        Complex *x = new Complex[n];
        Complex B[] = {19.0, 36.0};

        int ret = solver->solve(A, n, x, B);
        EXPECT_EQ(0, ret);

        EXPECT_LT(fabs(x[0].real() - 1.0), epsilon);
        EXPECT_LT(fabs(x[1].real() + 1.0), epsilon);

        for (int i = 0; i < n; ++i)
        {
            EXPECT_DOUBLE_EQ(0.0, x[i].imag());
        }
    }

    void tst_equ_real_2(LinearSolver *solver)
    {
        const int n = 3;
        Complex A[] = {
            4.0,
            5.0,
            6.0,
            0.0,
            2.0,
            3.0,
            0.0,
            0.0,
            7.0,
        };
        Complex *x = new Complex[n];
        Complex B[] = {10.0, 3.0, 7.0};

        int ret = solver->solve(A, n, x, B);
        EXPECT_EQ(0, ret);

        EXPECT_LT(fabs(x[0].real() - 1.0), epsilon);
        EXPECT_LT(fabs(x[1].real() - 0.0), epsilon);
        EXPECT_LT(fabs(x[2].real() - 1.0), epsilon);

        for (int i = 0; i < n; ++i)
        {
            EXPECT_DOUBLE_EQ(0.0, x[i].imag());
        }
    }

    void tst_equ_complex_1(LinearSolver *solver)
    {
        const int n = 2;
        Complex A[] = {
            {2.0, 3.0},
            {-2.0},
            {-2.0},
            {2.0, -1.0}};
        Complex *x = new Complex[n];
        Complex B[] = {-2.0, 1.0};

        int ret = solver->solve(A, n, x, B);
        EXPECT_EQ(0, ret);

        EXPECT_LT(fabs(x[0].real() - 0.08), epsilon);
        EXPECT_LT(fabs(x[0].imag() - 0.56), epsilon);
        EXPECT_LT(fabs(x[1].real() - 0.24), epsilon);
        EXPECT_LT(fabs(x[1].imag() - 0.68), epsilon);
    }
}
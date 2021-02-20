#include "gtest/gtest.h"
#include <iostream>
#include "tstLinearSolver.h"
#include "csim/internal/LinearSolver.h"

namespace csim
{
    TEST(equ_real_1, tstLinearSolverGauss)
    {
        LinearSolver *gauss = LinearSolver::createInstance("gauss");
        ASSERT_NE(nullptr, gauss);
        tst_equ_real_1(gauss);
        delete gauss;
    }

    TEST(equ_real_2, tstLinearSolverGauss)
    {
        LinearSolver *gauss = LinearSolver::createInstance("gauss");
        ASSERT_NE(nullptr, gauss);
        tst_equ_real_2(gauss);
        delete gauss;
    }

    TEST(equ_complex_1, tstLinearSolverGauss)
    {
        LinearSolver *gauss = LinearSolver::createInstance("gauss");
        ASSERT_NE(nullptr, gauss);
        tst_equ_complex_1(gauss);
        delete gauss;
    }

} // namespace

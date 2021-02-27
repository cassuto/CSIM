#include "gtest/gtest.h"
#include "tstLinearSolver.h"
#include "csim/internal/LinearSolver.h"

namespace csim
{
    TEST(tstLinearSolverLU, equ_real_1)
    {
        LinearSolver *gauss = LinearSolver::createInstance("LU");
        ASSERT_NE(nullptr, gauss);
        tst_equ_real_1(gauss);
        delete gauss;
    }

    TEST(tstLinearSolverLU, equ_real_2)
    {
        LinearSolver *gauss = LinearSolver::createInstance("LU");
        ASSERT_NE(nullptr, gauss);
        tst_equ_real_2(gauss);
        delete gauss;
    }

    TEST(tstLinearSolverLU, equ_complex_1)
    {
        LinearSolver *gauss = LinearSolver::createInstance("LU");
        ASSERT_NE(nullptr, gauss);
        tst_equ_complex_1(gauss);
        delete gauss;
    }

} // namespace

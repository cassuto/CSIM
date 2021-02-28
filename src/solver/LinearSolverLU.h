#ifndef LINEARSOLVER_LU_H_
#define LINEARSOLVER_LU_H_

#include "csim/internal/LinearSolver.h"

namespace csim
{
    class LinearSolverLU : public LinearSolver
    {
    public:
        LinearSolverLU();
        virtual ~LinearSolverLU();

        virtual int solve(Complex *A, unsigned int n, Complex *x, Complex *B);

    private:
        Complex *m_bufRow, *m_bufA;
        unsigned int *m_rowLoc;
        double *m_privot;
        unsigned int m_bufRows;
        size_t m_lenRow;
    };
}

#endif // LINEARSOLVER_LU_H_
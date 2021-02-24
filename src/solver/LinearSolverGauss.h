#ifndef LINEARSOLVER_GAUSS_H_
#define LINEARSOLVER_GAUSS_H_

#include "csim/internal/LinearSolver.h"

namespace csim
{
    class LinearSolverGauss : public LinearSolver
    {
    public:
        LinearSolverGauss();
        virtual ~LinearSolverGauss();

        virtual int solve(const Complex *A, int n, Complex *x, const Complex *B);

    private:
        Complex *m_bufRow, *m_bufA, *m_bufB;
        int m_bufRows;
        size_t m_lenRow;
    };
}

#endif // LINEARSOLVER_GAUSS_H_
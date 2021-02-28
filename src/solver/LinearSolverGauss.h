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

        virtual int solve(Complex *A, unsigned int n, Complex *x, Complex *B);

    private:
        Complex *m_bufRow, *m_bufA;
        unsigned int m_bufRows;
        size_t m_lenRow;
    };
}

#endif // LINEARSOLVER_GAUSS_H_
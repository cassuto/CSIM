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

        virtual int solve(csimModel::MComplex *A, unsigned int n, csimModel::MComplex *x, csimModel::MComplex *B);

    private:
        csimModel::MComplex *m_bufRow, *m_bufA;
        unsigned int *m_rowLoc;
        double *m_privot;
        unsigned int m_bufRows;
        size_t m_lenRow;
    };
}

#endif // LINEARSOLVER_LU_H_
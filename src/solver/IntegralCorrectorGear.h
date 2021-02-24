#ifndef INTEGRALSOLVER_GEAR_H_
#define INTEGRALSOLVER_GEAR_H_

#include "csim/internal/IntegralCorrector.h"

namespace csim
{
    class LinearSolver;
    class IntegralCorrectorGear : public IntegralCorrector
    {
    public:
        IntegralCorrectorGear();
        ~IntegralCorrectorGear();

        virtual void setStep(double step);
        virtual void setOrder(unsigned int order);
        virtual void integrate(const IntegralHistory *x, IntegralHistory *y, double k, double *c0, double *c1);

    public:
        static const unsigned int MaxOrder = IntegralHistory::MaxNumHistoryNum;

    private:
        void genCoeffs();

    private:
        LinearSolver *m_linearSolver;
        static const unsigned int MatrixRows = MaxOrder + 1;
        double m_coeffs[MatrixRows + 1];
        Complex m_A[MatrixRows * MatrixRows], m_x[MatrixRows], m_b[MatrixRows];
    };
}

#endif // INTEGRALSOLVER_GEAR_H_
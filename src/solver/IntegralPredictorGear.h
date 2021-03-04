#ifndef PREDICTOR_GEAR_H_
#define PREDICTOR_GEAR_H_

#include <map>
#include "csim/internal/IntegralPredictor.h"

namespace csim
{
    class LinearSolver;
    class IntegralPredictorGear : public IntegralPredictor
    {
    public:
        IntegralPredictorGear();
        virtual ~IntegralPredictorGear();
        virtual void setOrder(unsigned int order, const IntegralHistory *hsteps);
        virtual void setStep(const IntegralHistory *hsteps);
        virtual double predict(const IntegralHistory *x, const IntegralHistory *hsteps);
        virtual double getTruncErrorCoeff();

    public:
        static const unsigned int MaxOrder = IntegralHistory::MaxNumHistoryNum;

    private:
        LinearSolver *m_linearSolver;
        static const unsigned int MatrixRows = MaxOrder + 1;
        double m_coeffs[MatrixRows + 1];
        Complex m_A[MatrixRows * MatrixRows], m_x[MatrixRows], m_b[MatrixRows];
    };
}

#endif // PREDICTOR_GEAR_H_
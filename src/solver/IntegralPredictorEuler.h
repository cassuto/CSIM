#ifndef PREDICTOR_FORWARD_EULER_H_
#define PREDICTOR_FORWARD_EULER_H_

#include <map>
#include "csim/internal/IntegralPredictor.h"

namespace csim
{
    class IntegralPredictorEuler : public IntegralPredictor
    {
    public:
        virtual void setOrder(unsigned int order);
        virtual double predict(const IntegralHistory *x, const IntegralHistory *hsteps) ;
        virtual double getTruncErrorCoeff();
    };
}

#endif // PREDICTOR_FORWARD_EULER_H_
#include "gtest/gtest.h"
#include "constants.h"
#include "csim/utils/errors.h"
#include "csim/internal/IntegralPredictor.h"
#include "csim/internal/IntegralCorrector.h"
#include "tstIntegralCorrectorData.h"
#include <algorithm>

namespace csim
{
    TEST(tstIntegralPredictor, integral_RC)
    {
        IntegralPredictor *predictor = IntegralPredictor::createInstance("euler");
        IntegralCorrector *corrector = IntegralCorrector::createInstance("gear");
        IntegralHistory X, Y, hsteps;
        const double step = 1e-3;
        const double Vcc = 5.0;
        const double C = 4.7e-6;
        const double R = 100e3;
        const double gR = 1.0 / R;
        double geq, Ieq;
        X.setInitial(0.0);

        corrector->setOrder(4);
        corrector->setStep(step);
        hsteps.setInitial(step);

        const unsigned int steps = 1.0 / step; /* 1s */
        unsigned int d = 0;
        ASSERT_EQ(steps, sizeof(tstIntegralCorrectorRC) / sizeof(*tstIntegralCorrectorRC));
        unsigned int total_iters = 0;
        for (unsigned int i = 0; i < steps; ++i)
        {
            double x_1;
            unsigned int iter = 0;

            do
            {
                hsteps.set(0, step);
                double pred = predictor->predict(&X, &hsteps);
                X.set(0, pred);

                corrector->integrate(&X, &Y, C, &geq, &Ieq);
                X.set(0, (Vcc * gR - Ieq) / (geq + gR));

                if (iter)
                {
                    if (std::abs(X.get(0) - x_1) < epsilon_nonlinear / 2.0)
                    {
                        break;
                    }
                }
                x_1 = X.get(0);
                iter++;
            } while (iter < 100);
            total_iters += iter;

            EXPECT_LT(iter, 100);
            EXPECT_NEAR(X.get(0), tstIntegralCorrectorRC[d++], epsilon_nonlinear);

            X.push();
            hsteps.push();
        }
        std::cout << "total iterations = " << total_iters << std::endl;
        delete corrector;
        delete predictor;
    }

} // namespace

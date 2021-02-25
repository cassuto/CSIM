#include "gtest/gtest.h"
#include "constants.h"
#include "csim/utils/errors.h"
#include "csim/internal/IntegralCorrector.h"
#include "tstIntegralCorrectorData.h"
#include <algorithm>

namespace csim
{
    TEST(history, tstIntegralCorrector)
    {
        IntegralHistory history;
        history.setInitial(5.0);
        for (unsigned int i = 0; i < IntegralHistory::MaxNumHistoryNum; ++i)
        {
            EXPECT_EQ(history.get(i), 5.0);
        }

        unsigned int N = std::max(IntegralHistory::MaxNumHistoryNum / 2 - 1, IntegralHistory::MaxNumHistoryNum);
        for (unsigned int iter = 0; iter < IntegralHistory::MaxNumHistoryNum * 3; ++iter)
        {
            for (unsigned int i = 0; i < N; ++i)
            {
                history.push(i);
            }
            for (unsigned int i = 0; i < N; ++i)
            {
                EXPECT_DOUBLE_EQ(history.get(i), N - 1 - i);
            }
        }
    }

    TEST(integral_RC, tstIntegralCorrector)
    {
        IntegralCorrector *corrector = IntegralCorrector::createInstance("gear");
        IntegralHistory X, Y;
        const double step = 1e-3;
        const double Vcc = 5.0;
        const double C = 4.7e-6;
        const double R = 100e3;
        const double gR = 1.0 / R;
        double geq, Ieq;
        X.setInitial(0.0);

        corrector->setOrder(4);
        corrector->setStep(step);

        const unsigned int steps = 1.0 / step; /* 1s */
        unsigned int d = 0;
        ASSERT_EQ(steps, sizeof(tstIntegralCorrectorRC) / sizeof(*tstIntegralCorrectorRC));
        for (unsigned int i = 0; i < steps; ++i)
        {
            double y_1;
            unsigned int iter = 0;

            do
            {
                corrector->integrate(&X, &Y, C, &geq, &Ieq);
                X.set(0, (Vcc * gR - Ieq) / (geq + gR));

                if (iter)
                {
                    if (std::abs(Y.get(0) - y_1) < epsilon_nonlinear)
                    {
                        break;
                    }
                }
                y_1 = Y.get(0);
                iter++;
            } while (iter < 100);

            EXPECT_LT(iter, 100);

            X.push(X.get(0));
            EXPECT_NEAR(X.get(0), tstIntegralCorrectorRC[d++], epsilon_nonlinear);
        }
        delete corrector;
    }

} // namespace

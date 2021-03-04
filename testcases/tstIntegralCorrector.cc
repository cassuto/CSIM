#include "gtest/gtest.h"
#include "constants.h"
#include "csim/utils/errors.h"
#include "csim/internal/IntegralCorrector.h"
#include "tstIntegralCorrectorData.h"
#include <algorithm>

namespace csim
{
    TEST(tstIntegralCorrector, history)
    {
        IntegralHistory history;
        history.setInitial(5.0);
        for (unsigned int i = 0; i < IntegralHistory::MaxNumHistoryNum; ++i)
        {
            EXPECT_EQ(history.get(i), 5.0);
        }

        history.set(2, 2.0);
        EXPECT_EQ(history.get(2), 2.0);
        history.set(3, 0.0);
        history.push();
        EXPECT_EQ(history.get(3), 2.0);

        unsigned int N = std::max(IntegralHistory::MaxNumHistoryNum / 2 - 1, IntegralHistory::MaxNumHistoryNum);
        for (unsigned int iter = 0; iter < IntegralHistory::MaxNumHistoryNum * 3; ++iter)
        {
            for (unsigned int i = 0; i < N; ++i)
            {
                history.set(0, i);
                if (i != N - 1)
                    history.push();
            }
            for (unsigned int i = 0; i < N; ++i)
            {
                EXPECT_DOUBLE_EQ(history.get(i), N - 1 - i);
            }
        }
    }

    TEST(tstIntegralCorrector, integral_RC)
    {
        IntegralCorrector *corrector = IntegralCorrector::createInstance("gear");
        IntegralHistory X, Y, hsteps;
        const double step = 1e-3;
        const double Vcc = 5.0;
        const double C = 4.7e-6;
        const double R = 100e3;
        const double gR = 1.0 / R;
        double geq, Ieq;
        X.setInitial(0.0);

        hsteps.setInitial(step);
        corrector->setOrder(4, &hsteps);

        const unsigned int steps = 1.0 / step; /* 1s */
        unsigned int d = 0;
        ASSERT_EQ(steps, sizeof(tstIntegralCorrectorRC) / sizeof(*tstIntegralCorrectorRC));
        unsigned int total_iters = 0;
        for (unsigned int i = 0; i < steps; ++i)
        {
            double x_1 = 0.0;
            unsigned int iter = 0;

            do
            {
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
            } while (iter < 100U);
            total_iters += iter;

            EXPECT_LT(iter, 100U);
            EXPECT_NEAR(X.get(0), tstIntegralCorrectorRC[d++], epsilon_nonlinear);

            X.push();
        }
        std::cout << "total iterations = " << total_iters << std::endl;
        delete corrector;
    }

} // namespace

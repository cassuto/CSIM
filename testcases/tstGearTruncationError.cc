#include "gtest/gtest.h"
#include "constants.h"
#include "csim/utils/errors.h"
#include "csim/internal/IntegralCorrector.h"
#include "tstIntegralCorrectorData.h"
#include <algorithm>

namespace csim
{

    TEST(tstGearTruncationError, gear)
    {
        IntegralCorrector *corrector = IntegralCorrector::createInstance("gear");
        const unsigned int MAXORDER = 6;
        static const double trucnCoeff[MAXORDER] = {
            -1.0 / 2, -2.0 / 9, -3.0 / 22, -12.0 / 125, -10.0 / 137, -20.0 / 343};

        for(unsigned int i=1;i<=MAXORDER;++i){
            corrector->setOrder(i);
            ASSERT_NEAR(corrector->getTruncErrorCoeff(), trucnCoeff[i-1], epsilon_linear);
        }
        delete corrector;
    }

} // namespace

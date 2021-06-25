#include "gtest/gtest.h"
#include "constants.h"
#include <cstdio>
#include "csim/utils/errors.h"
#include "csim/utils/constants.h"
#include "csim/internal/parser/algebraic-defs.h"
#include <algorithm>
#include <cstring>
#include <cmath>

#define VERBOSE

namespace csim
{
    TEST(tstAlgebraic, four_ops)
    {
        int rc;
        double out;
        AlgebraicScope *rootScope = new AlgebraicScope(nullptr);
        AlgebraicScope *scope = new AlgebraicScope(rootScope);
        Algebraic *alg;

        double V1 = 4.5, V2 = 2.3;
        double VAL1 = 2.3;
        double VAL2 = 4.5;
        rootScope->addParam("V1", V1);
        rootScope->addParam("V2", 0.0);
        scope->addParam("V2", V2); /* Override outer scope */
        scope->addParam("VAL1", VAL1);
        scope->addParam("VAL2", VAL2);

        struct Testcase
        {
            const char *expr;
            double expected;
        };

#define pwr(x, y) (std::pow(std::fabs(x), y))

        Testcase tsts[] = {
            {"2.3*3 + (4.6/2 + 1) * -2e10", 2.3 * 3 + (4.6 / 2 + 1) * -2e10},
            {" ((200* (-1e-10/VAL1*3 + (4.6/-V1 + VAL2)) * -2e10 /V2 ) /20e10) ", (200 * (-1e-10 / VAL1 * 3 + (4.6 / -V1 + VAL2)) * -2e10 / V2) / 20e10},
            {"-(1.003*V1*2.3-3.6)", -(1.003*V1*2.3-3.6)},
            {"max((0.1008/(V1*1e6)+0.102*V2), 1e-3)", std::max((0.1008 / (V1 * 1e6) + 0.102 * V2), 1e-3)},
            {"-3 * sqrt(4) * pow(2,5)", -192.0},
            {"max(1,2)==2 ? -2/3:0.5", -2.0 / 3.0},
            {"-(0.0733*V1*1000000-0.0117)-(0.0733*V1*1000000-0.0117)*pwr(V2*1000000,2)", -(0.0733*V1*1000000-0.0117)-(0.0733*V1*1000000-0.0117)*pwr(V2*1000000,2)},
        };

        for (size_t i = 0; i < sizeof(tsts) / sizeof(*tsts); i++)
        {
            alg = new Algebraic(tsts[i].expr);
            rc = alg->evaluate(scope, &out);
            ASSERT_EQ(CERR_SUCCEEDED, rc);
            ASSERT_FLOAT_EQ(out, tsts[i].expected);
            delete alg;
        }

        delete scope;
        delete rootScope;
    }
}

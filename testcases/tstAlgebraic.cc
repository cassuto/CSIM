#include "gtest/gtest.h"
#include "constants.h"
#include <cstdio>
#include "csim/utils/errors.h"
#include "csim/utils/constants.h"
#include "csim/internal/parser/algebraic-defs.h"
#include <algorithm>
#include <cstring>

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

        double V1 = 4.5, V2=2.3;
        double VAL1 = 2.3;
        double VAL2 = 4.5;
        rootScope->addParam("V1", V1);
        rootScope->addParam("V2", 0);
        scope->addParam("V2", V2); /* Override outer scope */
        scope->addParam("VAL1", VAL1);
        scope->addParam("VAL2", VAL2);

        alg = new Algebraic("'2.3*3 + (4.6/2 + 1) * -2e10'");
        rc = alg->evaluate(scope, &out);
        ASSERT_EQ(CERR_SUCCEEDED, rc);
        ASSERT_FLOAT_EQ(out, 2.3*3 + (4.6/2 + 1) * -2e10);
        delete alg;

        alg = new Algebraic("' (200* (-1e-10/VAL1*3 + (4.6/-V1 + VAL2)) * -2e10 /V2 ) /20e10 '");
        rc = alg->evaluate(scope, &out);
        ASSERT_EQ(CERR_SUCCEEDED, rc);
        ASSERT_FLOAT_EQ(out, (200* (-1e-10/VAL1*3 + (4.6/-V1 + VAL2)) * -2e10 / V2 ) /20e10 );
        delete alg;

        alg = new Algebraic("'-3 * sqrt(4) * pow(2,5)'");
        rc = alg->evaluate(scope, &out);
        ASSERT_EQ(CERR_SUCCEEDED, rc);
        ASSERT_FLOAT_EQ(out, -192.0);
        delete alg;

        alg = new Algebraic("'max(1,2)==2 ? -2/3:0.5'");
        rc = alg->evaluate(scope, &out);
        ASSERT_EQ(CERR_SUCCEEDED, rc);
        ASSERT_FLOAT_EQ(out, -2.0/3.0);
        delete alg;

        delete scope;
        delete rootScope;
    }
}

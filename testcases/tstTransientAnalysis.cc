#include "gtest/gtest.h"
#include "tstLinearSolver.h"
#include "constants.h"
#include "csim/utils/errors.h"
#include "csim/internal/Circuit.h"
#include "csim/internal/Netlist.h"
#include "csim/internal/Analyzers.h"
#include "csim/internal/Dataset.h"
#include "csim/internal/ModelLoader.h"
#include "csim/model/ModelBase.h"

namespace csim
{
    /*
     * Circuit diagram
     * 
     *          R1           C1
     *     0 .______. 1    0    1
     *  +---+|______|+-------||--------+
     *  |      100k         4.7uF      |
     *  |            V1                |
     *  |       0    ,-.    1          |
     *  +-----------(---)--------------+
     *               `-'
     *               5V
     */
    TEST(tr_RC, tstTransientAnalysis)
    {
        int ret = 0;
        ModelEntry *e_R = ModelLoader::load(resistorLibrary);
        ASSERT_NE(nullptr, e_R);
        ModelEntry *e_VDC = ModelLoader::load(VDCLibrary);
        ASSERT_NE(nullptr, e_VDC);
        ModelEntry *e_C = ModelLoader::load(CapacitorLibrary);
        ASSERT_NE(nullptr, e_C);

        Circuit *circuit = new Circuit();

        /* Add models */
        ret = circuit->netlist()->addComponent("R1", e_R);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("C1", e_C);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("V1", e_VDC);
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* Configure */
        const double R = 100e3;
        const double C = 4.7e-6;
        const double Vcc = 5.0;
        ret = circuit->netlist()->configComponent("R1", "R", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(R));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("C1", "C", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(C));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("V1", "V", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(Vcc));
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* Set netlist */
        ret = circuit->netlist()->prepare();
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        ret = circuit->netlist()->wire("R1", 0, "V1", 0);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("R1", 1, "C1", 0);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("C1", 1, "V1", 1);
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        ret = circuit->netlist()->generateNodes();
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* Transient analysis */
        AnalyzerBase *analyzer = Analyzers::createInstance("transient", circuit);
        ASSERT_NE(nullptr, analyzer);

        unsigned int n_gnd, n1;
        ret = circuit->netlist()->getTermlNode("R1", 1, &n1);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        analyzer->addInterestNode(n1);
        ret = circuit->netlist()->getTermlNode("V1", 1, &n_gnd);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        analyzer->addInterestNode(n_gnd);

        Dataset dset;
        ret = analyzer->analyze(&dset);
        EXPECT_EQ(CERR_SUCCEEDED, ret);

        /* Check solution of transient analysis */
        unsigned int N = dset.getIndependentVar("time").getNumValues();
        for (unsigned int i = 0; i < N; ++i)
        {
            double time = dset.getIndependentVar("time").at(i).real();
            Complex volt = dset.getDependentVar("voltage", analyzer->makeVarName("V", n1)).at(i) -
                           dset.getDependentVar("voltage", analyzer->makeVarName("V", n_gnd)).at(i);

            double expected = Vcc * (1-std::exp(-time/(R*C)));
            EXPECT_NEAR(volt.real(), expected, epsilon_nonlinear);
        }

        delete analyzer;
        delete circuit;
        delete e_R;
        delete e_VDC;
        delete e_C;
    }

} // namespace

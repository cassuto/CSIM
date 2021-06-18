#include "gtest/gtest.h"
#include "constants.h"
#include "csim/utils/errors.h"
#include "csim/internal/Analyzers.h"
#include "csim/internal/Circuit.h"
#include "csim/internal/Netlist.h"
#include "csim/internal/Dataset.h"
#include "csim/internal/ModelLoader.h"
#include <algorithm>
#include <cstring>

namespace csim
{
    TEST(tstOPNonlinearCircuit, circuit_PN_R)
    {
        int ret = 0;
        ModelEntry *e_R = ModelLoader::load(resistorLibrary);
        ASSERT_NE(nullptr, e_R);
        ModelEntry *e_PN = ModelLoader::load(PNLibrary);
        ASSERT_NE(nullptr, e_PN);
        ModelEntry *e_VDC = ModelLoader::load(VDCLibrary);
        ASSERT_NE(nullptr, e_VDC);

        Circuit *circuit = new Circuit();

        /* Add models */
        ret = circuit->netlist()->addComponent("D1", e_PN);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("D2", e_PN);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("R1", e_R);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("V1", e_VDC);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("V2", e_VDC);
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* Configure */
        ret = circuit->netlist()->configComponent("R1", "R", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(500.0));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("V1", "V", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(10.0));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("V2", "V", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(15.0));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        char ref[16];
        for (int i = 1; i <= 2; ++i)
        {
            snprintf(ref, sizeof(ref), "D%d", i);
            ret = circuit->netlist()->configComponent(ref, "Is", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(1e-14));
            ASSERT_EQ(CERR_SUCCEEDED, ret);
            ret = circuit->netlist()->configComponent(ref, "Isr", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(0.0));
            ASSERT_EQ(CERR_SUCCEEDED, ret);
            ret = circuit->netlist()->configComponent(ref, "Temp", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(27));
            ASSERT_EQ(CERR_SUCCEEDED, ret);
        }

        /* Set netlist */
        ret = circuit->netlist()->prepare();
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        ret = circuit->netlist()->wire("V1", 0, "V2", 1);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("V1", 0, "D1", 1);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("V1", 1, "D2", 1);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("V2", 0, "R1", 0);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("R1", 1, "D1", 0);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("R1", 1, "D2", 0);
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        ret = circuit->netlist()->generateNodes();
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* DC analysis */
        AnalyzerBase *analyzer = Analyzers::createInstance("OP", circuit);
        ASSERT_NE(nullptr, analyzer);
        Dataset dset;
        ret = analyzer->analyze(&dset);
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* Check solution of Circuit object */

        unsigned int n_gnd, n1;
        ret = circuit->netlist()->getTermlNode("V1", 0, &n_gnd);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->getTermlNode("R1", 1, &n1);
        EXPECT_EQ(CERR_SUCCEEDED, ret);

        csimModel::MComplex volt = circuit->getNodeVolt(n1) - circuit->getNodeVolt(n_gnd);
        std::cout << "Vd1 = " << volt << std::endl;
        /* This value may change slightly when the PN-junction model is adjusted */
        EXPECT_LT(std::abs(csimModel::MComplex(-9.20838, 0) - volt), epsilon_nonlinear);

        delete analyzer;
        delete circuit;
        delete e_R;
        delete e_VDC;
        delete e_PN;
    }

    TEST(tstOPNonlinearCircuit, circuit_simple_PN)
    {
        int ret = 0;
        ModelEntry *e_D = ModelLoader::load(PNLibrary);
        ASSERT_NE(nullptr, e_D);
        ModelEntry *e_R = ModelLoader::load(resistorLibrary);
        ASSERT_NE(nullptr, e_R);
        ModelEntry *e_VDC = ModelLoader::load(VDCLibrary);
        ASSERT_NE(nullptr, e_VDC);

        Circuit *circuit = new Circuit();

        /* Add models */
        ret = circuit->netlist()->addComponent("D1", e_D);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("R1", e_R);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("V1", e_VDC);
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* Configure */
        ret = circuit->netlist()->configComponent("R1", "R", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(10.0));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("V1", "V", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(-5.0));
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* Set netlist */
        ret = circuit->netlist()->prepare();
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        ret = circuit->netlist()->wire("V1", 0, "D1", 0);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("D1", 1, "R1", 0);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("R1", 1, "V1", 1);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->ground("V1", 1);
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        ret = circuit->netlist()->generateNodes();
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* AC analysis */
        AnalyzerBase *analyzer = Analyzers::createInstance("OP", circuit);
        ASSERT_NE(nullptr, analyzer);

        unsigned int n1;
        ret = circuit->netlist()->getTermlNode("R1", 0, &n1);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        analyzer->addInterestNode(n1);

        Dataset dset;
        ret = analyzer->analyze(&dset);
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* Check solution vector of DC analyzer */
        const Variable &Vn1 = dset.getDependentVar("voltage", analyzer->makeVarName("V", n1));
        EXPECT_GT(Vn1.at(0).real(), -1.0);

        delete analyzer;
        delete circuit;
        delete e_D;
        delete e_R;
        delete e_VDC;
    }

} // namespace

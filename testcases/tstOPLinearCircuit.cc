#include "gtest/gtest.h"
#include "constants.h"
#include "csim/utils/errors.h"
#include "csim/internal/Analyzers.h"
#include "csim/internal/Circuit.h"
#include "csim/internal/Dataset.h"
#include "csim/internal/Netlist.h"
#include "csim/internal/ModelLoader.h"
#include <algorithm>
#include <cstring>

namespace csim
{
    /*
     * Circuit diagram
     *         20ohm           10ohm
     *     0 .______. 1    0 .______. 1
     *  +---+|__R1__|+------+|__R2__|+---+
     *  |                V1              |
     *  |          0    ,-.    1         |
     *  +--------------(---)-------------+
     *                  `-'
     *                  12V
     */
    TEST(tstOPLinearCircuit, circuit_R_VS_divider)
    {
        int ret = 0;
        ModelEntry *e_R = ModelLoader::load(resistorLibrary);
        ASSERT_NE(nullptr, e_R);
        ModelEntry *e_VDC = ModelLoader::load(VDCLibrary);
        ASSERT_NE(nullptr, e_VDC);

        Circuit *circuit = new Circuit();

        /* Add models */
        ret = circuit->netlist()->addComponent("R1", e_R);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("R2", e_R);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("V1", e_VDC);
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* Configure */
        ret = circuit->netlist()->configComponent("R1", "R", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(20.0));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("R2", "R", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(10.0));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("V1", "V", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(12.0));
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* Set netlist */
        ret = circuit->netlist()->prepare();
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        ret = circuit->netlist()->wire("R1", 0, "V1", 0);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("R1", 1, "R2", 0);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("R2", 1, "V1", 1);
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        ret = circuit->netlist()->generateNodes();
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* DC analysis */
        AnalyzerBase *analyzer = Analyzers::createInstance("OP", circuit);
        ASSERT_NE(nullptr, analyzer);
        Dataset dset;
        ret = analyzer->analyze(&dset);
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* Get nodes */
        unsigned int n_gnd, n1;
        ret = circuit->netlist()->getTermlNode("R1", 1, &n1);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->getTermlNode("R2", 1, &n_gnd);
        EXPECT_EQ(CERR_SUCCEEDED, ret);

        /* Check status of Circuit object */
        csimModel::MComplex volt = circuit->getNodeVolt(n1) - circuit->getNodeVolt(n_gnd);
        EXPECT_LT(std::abs(csimModel::MComplex(4.0, 0) - volt), epsilon_linear);

        delete circuit;
        delete e_R;
        delete e_VDC;
    }

    /*
     *                                +-----------+-----+
     *   Circuit diagram:            1|          0|     |0
     *                               .-.         .-.   .-.
     *                         30ohm |R|   40ohm |R|   |R| 60ohm
     *                               |3|         |4|   |6|
     *                               '-'         '-'   '-'
     *                               0|          1|     |1
     *          10ohm         20ohm   |    50ohm  |     |
     *      0 .______. 1  0 .______. 1| 0.______.1|     |
     *   +----|__R1__|------|__R2__|--+--|__R5__|-+-----+
     *   |                V1                            |
     *   |          0    ,-.    1                       |
     *   +--------------(---)---------------------------+
     *                   `-'
     *                   12V
     */
    TEST(tstOPLinearCircuit, circuit_R_VS_network)
    {
        int ret = 0;
        ModelEntry *e_R = ModelLoader::load(resistorLibrary);
        ASSERT_NE(nullptr, e_R);
        ModelEntry *e_VDC = ModelLoader::load(VDCLibrary);
        ASSERT_NE(nullptr, e_VDC);

        Circuit *circuit = new Circuit();

        /* Add 6 resistors */
        char ref[32];
        for (int i = 1; i <= 6; ++i)
        {
            snprintf(ref, sizeof(ref), "R%d", i);
            ret = circuit->netlist()->addComponent(ref, e_R);
            ASSERT_EQ(CERR_SUCCEEDED, ret);
        }
        ret = circuit->netlist()->addComponent("V1", e_VDC);
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* Configure */
        for (int i = 1; i <= 6; ++i)
        {
            snprintf(ref, sizeof(ref), "R%d", i);
            ret = circuit->netlist()->configComponent(ref, "R", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(10.0 * i));
            ASSERT_EQ(CERR_SUCCEEDED, ret);
        }
        ret = circuit->netlist()->configComponent("V1", "V", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(12.0));
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* Set netlist */
        ret = circuit->netlist()->prepare();
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        ret = circuit->netlist()->wire("R1", 0, "V1", 0);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("R1", 1, "R2", 0);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("R2", 1, "R5", 0);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("R2", 1, "R3", 0);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("R3", 1, "R4", 0);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("R3", 1, "R6", 0);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("R5", 1, "R4", 1);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("R5", 1, "R6", 1);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("R5", 1, "V1", 1);
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        ret = circuit->netlist()->generateNodes();
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* Test nodes */
        EXPECT_EQ(5U, circuit->netlist()->getNumNodes());

        unsigned int nds[5];
        ret = circuit->netlist()->getTermlNode("R1", 0, &nds[0]);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->getTermlNode("V1", 0, &nds[1]);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        EXPECT_EQ(nds[0], nds[1]);

        ret = circuit->netlist()->getTermlNode("R1", 1, &nds[0]);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->getTermlNode("R2", 0, &nds[1]);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        EXPECT_EQ(nds[0], nds[1]);

        ret = circuit->netlist()->getTermlNode("R2", 1, &nds[0]);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->getTermlNode("R3", 0, &nds[1]);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->getTermlNode("R5", 0, &nds[2]);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        EXPECT_EQ(nds[0], nds[1]);
        EXPECT_EQ(nds[1], nds[2]);

        ret = circuit->netlist()->getTermlNode("R3", 1, &nds[0]);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->getTermlNode("R4", 0, &nds[1]);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->getTermlNode("R6", 0, &nds[2]);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        EXPECT_EQ(nds[0], nds[1]);
        EXPECT_EQ(nds[1], nds[2]);

        ret = circuit->netlist()->getTermlNode("R5", 1, &nds[0]);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->getTermlNode("R4", 1, &nds[1]);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->getTermlNode("R6", 1, &nds[2]);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->getTermlNode("V1", 1, &nds[3]);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        EXPECT_EQ(nds[0], nds[1]);
        EXPECT_EQ(nds[1], nds[2]);
        EXPECT_EQ(nds[2], nds[3]);

        /* DC analysis */
        AnalyzerBase *analyzer = Analyzers::createInstance("OP", circuit);
        ASSERT_NE(nullptr, analyzer);

        unsigned int n_gnd, n[3];
        ret = circuit->netlist()->getTermlNode("V1", 1, &n_gnd);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        analyzer->addInterestNode(n_gnd);
        ret = circuit->netlist()->getTermlNode("R1", 1, &n[0]);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        analyzer->addInterestNode(n[0]);
        ret = circuit->netlist()->getTermlNode("R2", 1, &n[1]);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        analyzer->addInterestNode(n[1]);
        ret = circuit->netlist()->getTermlNode("R3", 1, &n[2]);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        analyzer->addInterestNode(n[2]);

        Dataset dset;
        ret = analyzer->analyze(&dset);
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* Check solution of DC analysis */
        csimModel::MComplex volt;
        unsigned int step = 0;
        volt = dset.getDependentVar("voltage", analyzer->makeVarName("V", n[0])).at(step) -
               dset.getDependentVar("voltage", analyzer->makeVarName("V", n_gnd)).at(step);
        EXPECT_LT(std::abs(csimModel::MComplex(9.85567, 0) - volt), epsilon_linear);

        volt = dset.getDependentVar("voltage", analyzer->makeVarName("V", n[1])).at(step) -
               dset.getDependentVar("voltage", analyzer->makeVarName("V", n_gnd)).at(step);
        EXPECT_LT(std::abs(csimModel::MComplex(5.56701, 0) - volt), epsilon_linear);

        volt = dset.getDependentVar("voltage", analyzer->makeVarName("V", n[2])).at(step) -
               dset.getDependentVar("voltage", analyzer->makeVarName("V", n_gnd)).at(step);
        EXPECT_LT(std::abs(csimModel::MComplex(2.47423, 0) - volt), epsilon_linear);

        delete analyzer;
        delete circuit;
        delete e_R;
        delete e_VDC;
    }

} // namespace

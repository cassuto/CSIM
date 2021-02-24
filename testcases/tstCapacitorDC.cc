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
    /*
     * Circuit diagram
     * 
     *          R1               R2
     *     0 .______. 1    0 .______. 1
     *  +---+|______|+------+|______|+---+
     *  |      20ohm           10ohm     |
     *  |                V1              |
     *  |          0    ,-.    1         |
     *  +--------------(---)-------------+
     *  |               `-'              |
     *  |               12V              |
     *  |                                |
     *  |          0    C1     1         |
     *  +---------------||---------------+
     */
    TEST(circuit_C, tstCapacitorDC)
    {
        int ret = 0;
        ModelEntry *e_R = ModelLoader::load(resistorLibrary);
        ASSERT_NE(nullptr, e_R);
        ModelEntry *e_VDC = ModelLoader::load(VDCLibrary);
        ASSERT_NE(nullptr, e_VDC);
        ModelEntry *e_CAP = ModelLoader::load(CapacitorLibrary);
        ASSERT_NE(nullptr, e_CAP);
        
        Circuit *circuit = new Circuit();

        /* Add models */
        ret = circuit->netlist()->addComponent("R1", e_R);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("R2", e_R);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("V1", e_VDC);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("C1", e_CAP);
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
        ret = circuit->netlist()->wire("C1", 0, "V1", 0);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("C1", 1, "V1", 1);
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        ret = circuit->netlist()->generateNodes();
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* DC analysis */
        AnalyzerBase *analyzer = Analyzers::createInstance("DC", circuit);
        ASSERT_NE(nullptr, analyzer);
        Dataset dset;
        ret = analyzer->analyze(&dset);
        EXPECT_EQ(CERR_SUCCEEDED, ret);

        /* Get nodes */
        unsigned int n_gnd, n1;
        ret = circuit->netlist()->getTermlNode("R1", 1, &n1);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->getTermlNode("R2", 1, &n_gnd);
        EXPECT_EQ(CERR_SUCCEEDED, ret);

        /* Check solution of Circuit object */
        Complex volt = circuit->getNodeVolt(n1) - circuit->getNodeVolt(n_gnd);
        EXPECT_LT(std::abs(Complex(4.0, 0) - volt), epsilon_linear);

        delete analyzer;
        delete circuit;
        delete e_R;
        delete e_VDC;
        delete e_CAP;
    }

} // namespace
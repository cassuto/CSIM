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
     * The voltage from R2(0) to R2(1) could be written as:
     * V01 = V1 * R2/(R1+R2)
     */
    TEST(tstDCLinearCircuit, circuit_R_VS_divider)
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
        double R1 = 20.0;
        double R2 = 10.0;
        double V1 = 12.0;
        ret = circuit->netlist()->configComponent("R1", "R", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(R1));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("R2", "R", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(R2));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("V1", "V", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(V1));
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
        AnalyzerBase *analyzer = Analyzers::createInstance("DC", circuit);
        ASSERT_NE(nullptr, analyzer);
        analyzer->property().setProperty("vstart", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(0.0));
        analyzer->property().setProperty("vstop", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(5.0));
        analyzer->property().setProperty("vincr", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(0.1));
        analyzer->property().setProperty("srcname", csimModel::Variant(csimModel::Variant::VariantString).setString("V1"));

        /* Get nodes */
        unsigned int n_gnd, n1;
        ret = circuit->netlist()->getTermlNode("R2", 0, &n1);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->getTermlNode("R2", 1, &n_gnd);
        EXPECT_EQ(CERR_SUCCEEDED, ret);

        analyzer->addInterestNode(n1);
        analyzer->addInterestNode(n_gnd);

        Dataset dset;
        ret = analyzer->analyze(&dset);
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        const Variable &sweep = dset.getIndependentVar("V1");
        const Variable &Vgnd = dset.getDependentVar("voltage", analyzer->makeVarName("V", n_gnd));
        const Variable &Vn1 = dset.getDependentVar("voltage", analyzer->makeVarName("V", n1));

        for(size_t i=0;i<sweep.getNumValues();i++)
        {
            double voltSrc = sweep.at(i).real();
            /* Model of R-R voltage divided network. */
            double voltExpected = voltSrc * R2/(R1+R2);
            double volt = Vn1.at(i).real() - Vgnd.at(i).real();
            EXPECT_NEAR(volt, voltExpected, epsilon_linear);
        }

        delete circuit;
        delete e_R;
        delete e_VDC;
    }

} // namespace

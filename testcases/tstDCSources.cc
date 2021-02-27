#include "gtest/gtest.h"
#include "constants.h"
#include "csim/utils/errors.h"
#include "csim/utils/constants.h"
#include "csim/internal/Analyzers.h"
#include "csim/internal/Circuit.h"
#include "csim/internal/Dataset.h"
#include "csim/internal/Netlist.h"
#include "csim/internal/ModelLoader.h"
#include <algorithm>
#include <cstring>

namespace csim
{
    TEST(tstDCSources, circuit_CSDC)
    {
        int ret = 0;
        ModelEntry *e_R = ModelLoader::load(resistorLibrary);
        ASSERT_NE(nullptr, e_R);
        ModelEntry *e_IDC = ModelLoader::load(IDCLibrary);
        ASSERT_NE(nullptr, e_IDC);

        Circuit *circuit = new Circuit();

        /* Add models */
        ret = circuit->netlist()->addComponent("I1", e_IDC);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("R1", e_R);
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* Configure */
        ret = circuit->netlist()->configComponent("I1", "I", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(1.0));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("R1", "R", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(10.0));
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* Set netlist */
        ret = circuit->netlist()->prepare();
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        circuit->netlist()->ground("R1", 0);
        ret = circuit->netlist()->wire("I1", 1, "R1", 1);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("I1", 0, "R1", 0);
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        ret = circuit->netlist()->generateNodes();
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* DC analysis */
        AnalyzerBase *analyzer = Analyzers::createInstance("DC", circuit);
        ASSERT_NE(nullptr, analyzer);

        unsigned int n_gnd, n1;
        ret = circuit->netlist()->getTermlNode("R1", 0, &n_gnd);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        analyzer->addInterestNode(n_gnd);
        ret = circuit->netlist()->getTermlNode("R1", 1, &n1);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        analyzer->addInterestNode(n1);

        Dataset dset;
        ret = analyzer->analyze(&dset);
        EXPECT_EQ(CERR_SUCCEEDED, ret);

        /* Check solution vector of DC analyzer */
        const Variable &Vgnd = dset.getDependentVar("voltage", analyzer->makeVarName("V", n_gnd));
        EXPECT_NEAR(Vgnd.at(0).real(), 0, epsilon_linear);

        const Variable &Vn = dset.getDependentVar("voltage", analyzer->makeVarName("V", n1));
        EXPECT_NEAR(Vn.at(0).real(), 10.0, epsilon_linear);

        delete analyzer;
        delete circuit;
        delete e_R;
        delete e_IDC;
    }

    TEST(tstDCSources, circuit_simple)
    {
        int ret = 0;
        ModelEntry *e_R = ModelLoader::load(resistorLibrary);
        ASSERT_NE(nullptr, e_R);
        ModelEntry *e_IDC = ModelLoader::load(IDCLibrary);
        ASSERT_NE(nullptr, e_IDC);
        ModelEntry *e_CCCS = ModelLoader::load(CCCSLibrary);
        ASSERT_NE(nullptr, e_CCCS);

        Circuit *circuit = new Circuit();

        /* Add models */
        ret = circuit->netlist()->addComponent("I1", e_IDC);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("R1", e_R);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("H1", e_CCCS);
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* Configure */
        ret = circuit->netlist()->configComponent("I1", "I", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(1.0));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("R1", "R", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(10.0));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("H1", "alpha", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(2.0));
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* Set netlist */
        ret = circuit->netlist()->prepare();
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        circuit->netlist()->ground("R1", 0);
        //circuit->netlist()->ground("I1", 1);
        ret = circuit->netlist()->wire("I1", 0, "H1", 3);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("I1", 1, "H1", 2);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("H1", 1, "R1", 1);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("H1", 0, "R1", 0);
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        ret = circuit->netlist()->generateNodes();
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* DC analysis */
        AnalyzerBase *analyzer = Analyzers::createInstance("DC", circuit);
        ASSERT_NE(nullptr, analyzer);

        unsigned int n_gnd, n1;
        ret = circuit->netlist()->getTermlNode("R1", 0, &n_gnd);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        analyzer->addInterestNode(n_gnd);
        ret = circuit->netlist()->getTermlNode("R1", 1, &n1);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        analyzer->addInterestNode(n1);

        Dataset dset;
        ret = analyzer->analyze(&dset);
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* Check solution vector of DC analyzer */
        const Variable &Vgnd = dset.getDependentVar("voltage", analyzer->makeVarName("V", n_gnd));
        EXPECT_NEAR(Vgnd.at(0).real(), 0, epsilon_linear);

        const Variable &Vn = dset.getDependentVar("voltage", analyzer->makeVarName("V", n1));
        EXPECT_NEAR(Vn.at(0).real(), 20.0, epsilon_nonlinear);

        delete analyzer;
        delete circuit;
        delete e_R;
        delete e_IDC;
        delete e_CCCS;
    }

    TEST(tstDCSources, circuit_VS_CS_VCVS_CCVS_CCVS_CCCS_R)
    {
        int ret = 0;
        ModelEntry *e_R = ModelLoader::load(resistorLibrary);
        ASSERT_NE(nullptr, e_R);
        ModelEntry *e_VDC = ModelLoader::load(VDCLibrary);
        ASSERT_NE(nullptr, e_VDC);
        ModelEntry *e_IDC = ModelLoader::load(IDCLibrary);
        ASSERT_NE(nullptr, e_IDC);
        ModelEntry *e_VCVS = ModelLoader::load(VCVSLibrary);
        ASSERT_NE(nullptr, e_VCVS);
        ModelEntry *e_CCVS = ModelLoader::load(CCVSLibrary);
        ASSERT_NE(nullptr, e_CCVS);
        ModelEntry *e_VCCS = ModelLoader::load(VCCSLibrary);
        ASSERT_NE(nullptr, e_VCCS);
        ModelEntry *e_CCCS = ModelLoader::load(CCCSLibrary);
        ASSERT_NE(nullptr, e_CCCS);

        Circuit *circuit = new Circuit();

        /* Add models */
        ret = circuit->netlist()->addComponent("V1", e_VDC);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("I1", e_IDC);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("R1", e_R);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("R2", e_R);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("R3", e_R);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("R4", e_R);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("E1", e_VCVS);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("F1", e_CCVS);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("F2", e_CCCS);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("G1", e_VCCS);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("G2", e_VCCS);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("H1", e_CCCS);
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* Configure */
        ret = circuit->netlist()->configComponent("I1", "I", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(1.0));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("V1", "V", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(1.0));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("R1", "R", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(10.0));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("R2", "R", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(10.0));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("R3", "R", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(10.0));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("R4", "R", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(10.0));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("E1", "Mu", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(2.0));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("F1", "r", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(1.5));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("F2", "alpha", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(1.0));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("G1", "G", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(1.5));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("G2", "G", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(1.5));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("H1", "alpha", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(3.7));
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* Set netlist */
        ret = circuit->netlist()->prepare();
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        ret = circuit->netlist()->wire("I1", 0, "R1", 0);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("I1", 0, "E1", 3);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("I1", 0, "F1", 1);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("I1", 0, "G1", 0);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("I1", 0, "G2", 0);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("I1", 0, "R4", 1);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("I1", 0, "R3", 1);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("I1", 0, "G1", 3);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("I1", 0, "G2", 3);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("V1", 1, "R1", 1);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("V1", 1, "I1", 1);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("V1", 0, "E1", 2);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("E1", 0, "R2", 1);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("E1", 0, "H1", 0);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("E1", 0, "F2", 0);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("R2", 0, "F2", 1);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("R2", 0, "H1", 1);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("R2", 0, "F1", 2);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("E1", 1, "F1", 3);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("F1", 0, "H1", 3);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("H1", 2, "F2", 3);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("F2", 2, "R3", 0);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("F2", 2, "G1", 2);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("F2", 2, "G2", 2);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("G1", 1, "R4", 0);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("G2", 1, "R4", 0);
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        ret = circuit->netlist()->generateNodes();
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* DC analysis */
        AnalyzerBase *analyzer = Analyzers::createInstance("DC", circuit);
        ASSERT_NE(nullptr, analyzer);

        const unsigned int N = 7;
        unsigned int n_gnd, n[N];
        ret = circuit->netlist()->getTermlNode("I1", 0, &n_gnd);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        analyzer->addInterestNode(n_gnd);
        ret = circuit->netlist()->getTermlNode("R1", 1, &n[0]);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        analyzer->addInterestNode(n[0]);
        ret = circuit->netlist()->getTermlNode("V1", 0, &n[1]);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        analyzer->addInterestNode(n[1]);
        ret = circuit->netlist()->getTermlNode("F1", 0, &n[2]);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        analyzer->addInterestNode(n[2]);
        ret = circuit->netlist()->getTermlNode("F2", 2, &n[3]);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        analyzer->addInterestNode(n[3]);
        ret = circuit->netlist()->getTermlNode("G1", 1, &n[4]);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        analyzer->addInterestNode(n[4]);
        ret = circuit->netlist()->getTermlNode("E1", 0, &n[5]);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        analyzer->addInterestNode(n[5]);
        ret = circuit->netlist()->getTermlNode("E1", 1, &n[6]);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        analyzer->addInterestNode(n[6]);

        Dataset dset;
        ret = analyzer->analyze(&dset);
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* Check solution vector of DC analyzer */
        const Variable &Vgnd = dset.getDependentVar("voltage", analyzer->makeVarName("V", n_gnd));
        Complex volt[N];
        for (unsigned int i = 0; i < N; ++i)
        {
            const Variable &Vn = dset.getDependentVar("voltage", analyzer->makeVarName("V", n[i]));
            volt[i] = Vn.at(0) - Vgnd.at(0);
        }

        static const double solutions[5] = {
            10,
            11,
            1.93548,
            1.93548,
            58.0645,
        };
        for (unsigned int i = 0; i < 5; ++i)
        {
            ASSERT_NEAR(volt[i].real(), solutions[i], epsilon_nonlinear);
        }
        ASSERT_NEAR(volt[5].real() - volt[6].real(), 22.0, epsilon_nonlinear);

        delete analyzer;
        delete circuit;
        delete e_R;
        delete e_VDC;
        delete e_IDC;
        delete e_VCVS;
        delete e_CCVS;
        delete e_VCCS;
        delete e_CCCS;
    }

} // namespace

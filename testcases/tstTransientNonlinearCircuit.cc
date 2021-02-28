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

#include <fstream>
namespace csim
{
    TEST(tstTransientNonlinearCircuit, circuit_simple_D)
    {
        int ret = 0;
        ModelEntry *e_D = ModelLoader::load(PNLibrary);
        ASSERT_NE(nullptr, e_D);
        ModelEntry *e_R = ModelLoader::load(resistorLibrary);
        ASSERT_NE(nullptr, e_R);
        ModelEntry *e_VAC = ModelLoader::load(VACLibrary);
        ASSERT_NE(nullptr, e_VAC);

        Circuit *circuit = new Circuit();

        /* Add models */
        ret = circuit->netlist()->addComponent("D1", e_D);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("R1", e_R);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("V1", e_VAC);
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* Configure */
        ret = circuit->netlist()->configComponent("R1", "R", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(10.0));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("V1", "Vp", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(5.0));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("V1", "freq", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(50.0));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("V1", "phase", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(90.0));
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

        /* Transient analysis */
        AnalyzerBase *analyzer = Analyzers::createInstance("transient", circuit);
        ASSERT_NE(nullptr, analyzer);
        analyzer->property().setProperty("tstop", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(0.05));
        analyzer->property().setProperty("tstep", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(1e-6));

        unsigned int n1, nt;
        ret = circuit->netlist()->getTermlNode("R1", 0, &n1);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        analyzer->addInterestNode(n1);

        Dataset dset;
        ret = analyzer->analyze(&dset);
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* Check solution vector of DC analyzer */
        const Variable &T = dset.getIndependentVar("time");
        const Variable &Vn1 = dset.getDependentVar("voltage", analyzer->makeVarName("V", n1));
        //std::ofstream fof("outd.csv");
        for (unsigned int i = 0; i < T.getNumValues(); ++i)
        {
            //fof << T.at(i).real() << "," << Vn1.at(i).real() << "\n";
        }

        delete analyzer;
        delete circuit;
        delete e_D;
        delete e_R;
        delete e_VAC;
    }

    TEST(tstTransientNonlinearCircuit, circuit_simple_D_C)
    {
        int ret = 0;
        ModelEntry *e_D = ModelLoader::load(PNLibrary);
        ASSERT_NE(nullptr, e_D);
        ModelEntry *e_R = ModelLoader::load(resistorLibrary);
        ASSERT_NE(nullptr, e_R);
        ModelEntry *e_CAP = ModelLoader::load(CapacitorLibrary);
        ASSERT_NE(nullptr, e_CAP);
        ModelEntry *e_VAC = ModelLoader::load(VACLibrary);
        ASSERT_NE(nullptr, e_VAC);

        Circuit *circuit = new Circuit();

        /* Add models */
        ret = circuit->netlist()->addComponent("D1", e_D);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("R1", e_R);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("C1", e_CAP);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("V1", e_VAC);
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* Configure */
        ret = circuit->netlist()->configComponent("R1", "R", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(10.0));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("C1", "C", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(1e-6));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("V1", "Vp", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(5.0));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("V1", "freq", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(50.0));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("V1", "phase", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(0.0));
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* Set netlist */
        ret = circuit->netlist()->prepare();
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        ret = circuit->netlist()->wire("V1", 0, "D1", 0);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("D1", 1, "R1", 0);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("R1", 1, "C1", 0);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("C1", 1, "V1", 1);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->ground("V1", 1);
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        ret = circuit->netlist()->generateNodes();
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* Transient analysis */
        AnalyzerBase *analyzer = Analyzers::createInstance("transient", circuit);
        ASSERT_NE(nullptr, analyzer);
        analyzer->property().setProperty("tstop", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(0.05));
        analyzer->property().setProperty("tstep", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(1e-6));

        unsigned int n1, nt;
        ret = circuit->netlist()->getTermlNode("R1", 1, &n1);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        analyzer->addInterestNode(n1);

        Dataset dset;
        ret = analyzer->analyze(&dset);
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* Check solution vector of DC analyzer */
        const Variable &T = dset.getIndependentVar("time");
        const Variable &Vn1 = dset.getDependentVar("voltage", analyzer->makeVarName("V", n1));
        std::ofstream fof("outc.csv");
        for (unsigned int i = 0; i < T.getNumValues(); ++i)
        {
            fof << T.at(i).real() << "," << Vn1.at(i).real() << "\n";
        }

        delete analyzer;
        delete circuit;
        delete e_D;
        delete e_R;
        delete e_CAP;
        delete e_VAC;
    }

    TEST(tstTransientNonlinearCircuit, circuit_D_RLC)
    {
        int ret = 0;
        ModelEntry *e_D = ModelLoader::load(PNLibrary);
        ASSERT_NE(nullptr, e_D);
        ModelEntry *e_R = ModelLoader::load(resistorLibrary);
        ASSERT_NE(nullptr, e_R);
        ModelEntry *e_L = ModelLoader::load(InductorLibrary);
        ASSERT_NE(nullptr, e_L);
        ModelEntry *e_CAP = ModelLoader::load(CapacitorLibrary);
        ASSERT_NE(nullptr, e_CAP);
        ModelEntry *e_VAC = ModelLoader::load(VACLibrary);
        ASSERT_NE(nullptr, e_VAC);

        Circuit *circuit = new Circuit();

        /* Add models */
        ret = circuit->netlist()->addComponent("D1", e_D);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("L1", e_L);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("C1", e_CAP);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("R1", e_R);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("V1", e_VAC);
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* Configure */
        ret = circuit->netlist()->configComponent("L1", "L", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(20e-3));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("C1", "C", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(1e-6));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("R1", "R", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(10.0));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("V1", "Vp", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(5.0));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("V1", "freq", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(50.0));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("V1", "phase", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(90.0));
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* Set netlist */
        ret = circuit->netlist()->prepare();
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        ret = circuit->netlist()->wire("V1", 0, "D1", 0);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("D1", 1, "R1", 0);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("R1", 1, "L1", 0);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("L1", 1, "C1", 1);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("C1", 0, "R1", 1);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("V1", 1, "C1", 1);
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        ret = circuit->netlist()->ground("V1", 1);
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        ret = circuit->netlist()->generateNodes();
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* Transient analysis */
        AnalyzerBase *analyzer = Analyzers::createInstance("transient", circuit);
        ASSERT_NE(nullptr, analyzer);
        analyzer->property().setProperty("tstop", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(0.05));
        analyzer->property().setProperty("tstep", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(100e-6));

        unsigned int n1;
        ret = circuit->netlist()->getTermlNode("R1", 1, &n1);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        analyzer->addInterestNode(n1);

        Dataset dset;
        ret = analyzer->analyze(&dset);
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* Check solution vector of DC analyzer */
        const Variable &T = dset.getIndependentVar("time");
        const Variable &Vn1 = dset.getDependentVar("voltage", analyzer->makeVarName("V", n1));
        std::ofstream fof("outa.csv");
        for (unsigned int i = 0; i < T.getNumValues(); ++i)
        {
            fof << T.at(i).real() << "," << Vn1.at(i).real() << "\n";
        }

        delete analyzer;
        delete circuit;
        delete e_D;
        delete e_R;
        delete e_L;
        delete e_CAP;
        delete e_VAC;
    }

} // namespace

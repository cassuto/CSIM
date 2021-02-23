#include "gtest/gtest.h"
#include "constants.h"
#include "csim/utils/errors.h"
#include "csim/utils/constants.h"
#include "csim/internal/Analyzers.h"
#include "csim/internal/Circuit.h"
#include "csim/internal/Netlist.h"
#include "csim/internal/ModelLoader.h"
#include <algorithm>
#include <cstring>

namespace csim
{
    /*
     * Circuit diagram
     *          L1         C1            R1
     *     0         1   0    1    0  .______.  1
     *  +----^v^v^v^v------||--------+|______|+----+
     *  |      10mH       4.7uF         1ohm       |
     *  |            V1                            |
     *  |      0    ,-.    1                       |
     *  +----------(---)---------------------------+
     *              `-'
     *            Vpeak=1V
     * 
     * The resonant frequency of this circuit can be written as:
     * \f[
     *     f = \frac{1}{2\pi\sqrt{LC}}\approx 734.127 (Hz)
     * \f]
     * Under f, the circuit presents pure resistance
     * and the phase of V and I is equal.
     */
    TEST(circuit_LC, tstACLinearCircuit)
    {
        int ret = 0;
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
        ret = circuit->netlist()->addComponent("L1", e_L);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("C1", e_CAP);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("R1", e_R);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("V1", e_VAC);
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* Configure */
        const double L = 10e-3;
        const double C = 4.7e-6;
        ret = circuit->netlist()->configComponent("L1", "L", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(L));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("C1", "C", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(C));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("R1", "R", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(1.0));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("V1", "Vp", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(1.0));
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* Set netlist */
        ret = circuit->netlist()->prepare();
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        ret = circuit->netlist()->wire("L1", 0, "V1", 0);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("L1", 1, "C1", 0);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("C1", 1, "R1", 0);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("R1", 1, "V1", 1);
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        ret = circuit->netlist()->generateNodes();
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* AC analysis */
        AnalyzerBase *analyzer = Analyzers::createInstance("AC", circuit);
        ASSERT_NE(nullptr, analyzer);
        const double fstart = 1.0 / (2 * M_PI * std::sqrt(L*C));
        analyzer->property().setProperty("fstart", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(fstart));
        analyzer->property().setProperty("fstop", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(800));
        analyzer->property().setProperty("fstep", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(1.0));

        ret = analyzer->analyze();
        EXPECT_EQ(CERR_SUCCEEDED, ret);

        /* Get nodes */
        unsigned int n_gnd, n1;
        ret = circuit->netlist()->getTermlNode("R1", 0, &n1);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->getTermlNode("V1", 1, &n_gnd);
        EXPECT_EQ(CERR_SUCCEEDED, ret);

        /* Check solution vector of DC analyzer */
        
        EXPECT_GE(analyzer->getNumSteps(), 1);

        const Complex *Vn = analyzer->getNodeVoltVector(0);
        Complex volt = Vn[n1] - Vn[n_gnd];
        EXPECT_NEAR(std::abs(volt), 1.0, epsilon_linear);
        EXPECT_NEAR(180 * std::arg(volt) / M_PI, 0.0, epsilon_linear);
        
        delete circuit;
        delete e_R;
        delete e_L;
        delete e_CAP;
        delete e_VAC;
    }

} // namespace

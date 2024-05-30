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
#include <complex>
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
    static void tstACLinearCircuit_helper(const char *fspace, double fstart, double fstop) {
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
        const double L = 10e-3,
                     C = 4.7e-6,
                     R = 1.0,
                     Vp = 1.0;
        ret = circuit->netlist()->configComponent("L1", "L", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(L));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("C1", "C", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(C));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("R1", "R", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(R));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("V1", "Vp", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(Vp));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("V1", "freq", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(50.0));
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
        analyzer->property().setProperty("fstart", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(fstart));
        analyzer->property().setProperty("fstop", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(fstop));
        analyzer->property().setProperty("fpoints", csimModel::Variant(csimModel::Variant::VariantUint32).setUint32(50));
        analyzer->property().setProperty("fspace", csimModel::Variant(csimModel::Variant::VariantString).setString(fspace));
        unsigned int n_gnd, n1;
        ret = circuit->netlist()->getTermlNode("R1", 0, &n1);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->getTermlNode("V1", 1, &n_gnd);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        analyzer->addInterestNode(n_gnd);
        analyzer->addInterestNode(n1);

        Dataset dset;
        ret = analyzer->analyze(&dset);
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* Check solution vector of AC analyzer */
        const Variable &F = dset.getIndependentVar("frequency");
        const Variable &Vgnd = dset.getDependentVar("voltage", analyzer->makeVarName("V", n_gnd));
        const Variable &Vn1 = dset.getDependentVar("voltage", analyzer->makeVarName("V", n1));
        for(size_t i=0; i<F.getNumValues(); ++i) {
            csimModel::MComplex _volt = Vn1.at(i) - Vgnd.at(i);
            auto volt = std::complex(_volt.real(), _volt.imag());
            double omega = F.at(i).real() * 2 * M_PI;
            std::complex<double> z(R, omega*L - 1.0/(omega * C));
            auto Ic = Vp / z;
            double mag = std::abs(R*Ic);
            EXPECT_NEAR(mag, std::abs(volt), epsilon_linear);
            EXPECT_NEAR(std::arg(volt), std::arg(Ic), epsilon_linear);
        }

        delete analyzer;
        delete circuit;
        delete e_R;
        delete e_L;
        delete e_CAP;
        delete e_VAC;
    }
    TEST(tstACLinearCircuit, circuit_LC_sweep)
    {
        tstACLinearCircuit_helper("lin", 500, 800);
        tstACLinearCircuit_helper("log", 500, 800);
        tstACLinearCircuit_helper("dec", 50, 1000);
        tstACLinearCircuit_helper("oct", 50, 1000);
    }

    TEST(tstACLinearCircuit, reverse_sweep)
    {
        tstACLinearCircuit_helper("lin", 700, 100);
        tstACLinearCircuit_helper("log", 700, 100);
        tstACLinearCircuit_helper("dec", 1000, 10);
        tstACLinearCircuit_helper("oct", 1000, 10);
    }

} // namespace

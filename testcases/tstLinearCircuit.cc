#include "gtest/gtest.h"
#include "csim/utils/errors.h"
#include "csim/internal/Circuit.h"
#include "csim/internal/ModelLoader.h"

namespace csim
{
    TEST(circuit_R_R, tstLinearCircuit)
    {
        ModelEntry *e_R = ModelLoader::load("cygcsimModelResistor.dll");
        ASSERT_NE(nullptr, e_R);
        ModelEntry *e_VDC = ModelLoader::load("cygcsimModelVDC.dll");
        ASSERT_NE(nullptr, e_VDC);


        Circuit *circuit = new Circuit();
        csimModel::ModelBase *R1 = e_R->createInstance(circuit);
        ASSERT_NE(nullptr, R1);
        csimModel::ModelBase *R2 = e_R->createInstance(circuit);
        ASSERT_NE(nullptr, R2);
        csimModel::ModelBase *VDC1 = e_R->createInstance(circuit);
        ASSERT_NE(nullptr, VDC1);

        R1->property().setProperty("R", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(10.0));
        R2->property().setProperty("R", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(20.0));
        VDC1->property().setProperty("V", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(12.0));

        circuit->addComponent(R1);
        circuit->addComponent(R2);
        circuit->addComponent(VDC1);

        int ret = circuit->analyseDC();
        EXPECT_TRUE(CSIM_OK(ret));

        delete circuit;
        e_R->deleteInstance(R1);
        e_R->deleteInstance(R2);
        delete e_R;

        delete e_VDC;
    }

} // namespace

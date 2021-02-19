#include "gtest/gtest.h"
#include <iostream>
#include "csim/internal/Circuit.h"
#include "csim/internal/ModelLoader.h"

namespace csim
{
    TEST(loadResistor, tstLoader)
    {
        ModelEntry *entry = ModelLoader::load("cygcsimModelResistor.dll");
        ASSERT_NE(nullptr, entry);
        std::cout<<"ID = "<<entry->descriptor()->id<<std::endl;
        std::cout<<"DESC = "<<entry->descriptor()->description<<std::endl;

        Circuit *circuit = new Circuit();
        csimModel::ModelBase *model = entry->createInstance(circuit);
        ASSERT_NE(nullptr, model);

        model->property().setProperty("R", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(10.0));
        model->configure();

        ASSERT_EQ(2, model->getNumTerml());
    }

    TEST(loadNonModel, tstLoader)
    {
        ModelEntry *entry = ModelLoader::load("cygcsim.dll");
        ASSERT_EQ(nullptr, entry);
    }

} // namespace

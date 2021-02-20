#include "gtest/gtest.h"
#include <iostream>
#include <cstring>
#include "csim/internal/Circuit.h"
#include "csim/internal/ModelLoader.h"

namespace csim
{
    TEST(loadResistor, tstLoader)
    {
        ModelEntry *entry = ModelLoader::load("cygcsimModelResistor.dll");
        ASSERT_NE(nullptr, entry);
        EXPECT_GT(strlen(entry->descriptor()->id), 0);
        EXPECT_GT(strlen(entry->descriptor()->description), 0);
        
        Circuit *circuit = new Circuit();
        csimModel::ModelBase *model = entry->createInstance(circuit);
        ASSERT_NE(nullptr, model);

        model->property().setProperty("R", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(10.0));
        int ret = model->configure();

        ASSERT_EQ(0, ret);
        ASSERT_EQ(2, model->getNumTerml());

        delete circuit;
        entry->deleteInstance(model);
        delete entry;
    }

    TEST(loadNonModel, tstLoader)
    {
        ModelEntry *entry = ModelLoader::load("cygcsim.dll");
        ASSERT_EQ(nullptr, entry);
        delete entry;
    }

} // namespace

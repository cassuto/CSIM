#include "gtest/gtest.h"
#include "constants.h"
#include <cstring>
#include "csim/internal/Circuit.h"
#include "csim/internal/ModelLoader.h"

namespace csim
{
    TEST(tstLoader, loadResistor)
    {
        ModelEntry *entry = ModelLoader::load(resistorLibrary);
        ASSERT_NE(nullptr, entry);
        EXPECT_GT(strlen(entry->descriptor()->id), 0U);
        EXPECT_GT(strlen(entry->descriptor()->description), 0U);
        
        Circuit *circuit = new Circuit();
        csimModel::ModelBase *model = entry->createInstance(circuit);
        ASSERT_NE(nullptr, model);

        model->property().setProperty("R", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(10.0));
        int ret = model->configure();

        ASSERT_EQ(0, ret);
        ASSERT_EQ(2U, model->getNumTerml());

        delete circuit;
        entry->deleteInstance(model);
        delete entry;
    }

    TEST(tstLoader, loadNonModel)
    {
        ModelEntry *entry = ModelLoader::load(csimLibrary);
        ASSERT_EQ(nullptr, entry);
        delete entry;
    }

} // namespace

#include "gtest/gtest.h"
namespace
{

    TEST(Loader, tstLoader)
    {
        EXPECT_EQ(1, 1);
        EXPECT_EQ(1, 1);
        EXPECT_GT(2, 0);
    }

    TEST(Loader2, tstLoader)
    {
        EXPECT_EQ(1, 2);
    }

} // namespace

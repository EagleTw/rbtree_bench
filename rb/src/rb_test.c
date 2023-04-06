#include <gtest/gtest.h>
#include "rb.h"

// Just a simple test that returns true for testing CMake setting
TEST(insertion_test, simple) {
    EXPECT_EQ(1,1);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

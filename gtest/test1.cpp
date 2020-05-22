#include "gtest/gtest.h"
#include <iostream>

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Message;
using ::testing::Test;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::UnitTest;

bool MutuallyPrime(int m, int n)
{
    return m + n > 1;
}

TEST(PredicateAssertionTest, Demo)
{
    int m = 5, n = 6;
    EXPECT_PRED2(MutuallyPrime, m, n);
}

TEST(Int2Str, testInt2Str)
{
    std::string intstr = "100";
    std::string handrud = "100";
    ASSERT_STREQ(intstr.c_str(), handrud.c_str());
}

int main(int argc, char **argv) {
  InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}


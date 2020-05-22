#include "gtest/gtest.h"
#include <iostream>

class FooCalc
{
public:
    void Init(){}
    void Finalize(){}
    int calc(int m, int n){return m+n;}
};

class FooCalcTest:public testing::Test
{
protected:
    virtual void SetUp()
    {
        m_foo.Init();
    }
    virtual void TearDown()
    {
        m_foo.Finalize();
    }

    FooCalc m_foo;
};

TEST_F(FooCalcTest, testFooCalc_calc)
{
    EXPECT_EQ(12+16, m_foo.calc(12, 16));
}

int main(int argc, char*argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

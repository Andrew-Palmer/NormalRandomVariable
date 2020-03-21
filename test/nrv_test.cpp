#include <gtest/gtest.h>

#include "NormalRandomVariable.h"


TEST(Instantiation, ValidVariance)
{
    EXPECT_NO_THROW(NRV::NormalRandomVariable(1, 1));
}

TEST(Instantiation, InvalidVariance)
{
    EXPECT_ANY_THROW(NRV::NormalRandomVariable(1, 0));
    EXPECT_ANY_THROW(NRV::NormalRandomVariable(1, -1));
}

TEST(Getters, GetMean)
{
    auto rv = NRV::NormalRandomVariable(1,2);
    EXPECT_DOUBLE_EQ(rv.mean(), 1);
}

TEST(Getters, GetVariance)
{
    auto rv = NRV::NormalRandomVariable(1,2);
    EXPECT_DOUBLE_EQ(rv.variance(), 2);
}

TEST(Addition, TwoRandomVariables)
{
    auto rv1 = NRV::NormalRandomVariable(1,2);
    auto rv2 = NRV::NormalRandomVariable(3,4);
    auto rv3 = rv1 + rv2;
    EXPECT_DOUBLE_EQ(rv3.mean(), 4);
    EXPECT_DOUBLE_EQ(rv3.variance(), 6);
}

TEST(Addition, RandomVariableAndConst)
{
    auto rv1 = NRV::NormalRandomVariable(1,2);
    auto rv2 = rv1 + 10;
    EXPECT_DOUBLE_EQ(rv2.mean(), 11);
    EXPECT_DOUBLE_EQ(rv2.variance(), 2);

    rv2 = 12.5 + rv1;
    EXPECT_DOUBLE_EQ(rv2.mean(), 13.5);
    EXPECT_DOUBLE_EQ(rv2.variance(), 2);
}

TEST(Subtraction, TwoRandomVariables)
{
    auto rv1 = NRV::NormalRandomVariable(1,2);
    auto rv2 = NRV::NormalRandomVariable(3,4);
    auto rv3 = rv1 - rv2;
    EXPECT_DOUBLE_EQ(rv3.mean(), -2);
    EXPECT_DOUBLE_EQ(rv3.variance(), 6);
}

TEST(Subtraction, RandomVariableAndConst)
{
    auto rv1 = NRV::NormalRandomVariable(1,2);
    auto rv2 = rv1 - 10;
    EXPECT_DOUBLE_EQ(rv2.mean(), -9);
    EXPECT_DOUBLE_EQ(rv2.variance(), 2);

    rv2 = 12.5 - rv1;
    EXPECT_DOUBLE_EQ(rv2.mean(), 11.5);
    EXPECT_DOUBLE_EQ(rv2.variance(), 2);
}

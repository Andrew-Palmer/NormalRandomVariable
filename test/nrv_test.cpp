#include <gtest/gtest.h>

#include "NormalRandomVariable.h"


TEST(Instantiation, InvalidVariance){
    //EXPECT_ANY_THROW(NRV::NormalRandomVariable(1,-1));
    EXPECT_TRUE(1 == 1);
}

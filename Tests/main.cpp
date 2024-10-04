#include <gtest/gtest.h>
#include "Environment.hpp"

int main(int argc, char **argv) 
{
    testing::InitGoogleTest(&argc, argv);
    auto env = testing::AddGlobalTestEnvironment(new Environment);
    return RUN_ALL_TESTS();
}

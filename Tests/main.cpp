#include <gtest/gtest.h>
#include "Args.hpp"
#include "Environment.hpp"

int main(int argc, char **argv) 
{
    testing::InitGoogleTest(&argc, argv);
    Args args(argc, argv);
    auto env = testing::AddGlobalTestEnvironment(new Environment(args));
    return RUN_ALL_TESTS();
}

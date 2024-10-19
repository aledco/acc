#pragma once

#include <tuple>
#include <gtest/gtest.h>
#include "Args.hpp"
#include "TestUtils.hpp"

extern std::vector<Input> all_inputs;

class Environment : public ::testing::Environment {
private:
    Args& args;

public:

    Environment(Args& args): args(args) {}

    ~Environment() override {}

    void SetUp() override;

    void TearDown() override {}

    inline Args& GetArgs() { return args; }
};

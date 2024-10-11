#pragma once

#include <tuple>
#include <gtest/gtest.h>
#include "TestUtils.hpp"

extern std::vector<Input> all_inputs;

class Environment : public ::testing::Environment {
public:
    ~Environment() override {}

    void SetUp() override;

    void TearDown() override {}
};

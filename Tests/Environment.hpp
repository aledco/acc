#pragma once

#include <gtest/gtest.h>
#include "TestUtils.hpp"

extern std::vector<std::string> all_inputs;

class Environment : public ::testing::Environment {
public:
    ~Environment() override {}

    void SetUp() override;

    void TearDown() override {}
};

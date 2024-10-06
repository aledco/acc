#pragma once

#include <gtest/gtest.h>
#include <string>
#include <fstream>
#include <vector>
#include <tuple>

std::string read_input(int test_number);
std::vector<std::tuple<int, std::string>> read_all_inputs();
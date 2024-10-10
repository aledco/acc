#pragma once

#include <gtest/gtest.h>
#include <string>
#include <fstream>
#include <vector>
#include <tuple>

#ifdef __INTELLISENSE__
    #pragma diag_suppress 20
#endif

std::string read_input(int test_number);
std::vector<std::tuple<int, std::string>> read_all_inputs();
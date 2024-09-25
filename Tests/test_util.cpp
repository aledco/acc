#include "test_util.hpp"

std::string read_input(int test_number)
{
    auto filepath = "Tests/in/test" + std::to_string(test_number) + ".c";
    std::ifstream file(filepath);
    EXPECT_TRUE(file.good());

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
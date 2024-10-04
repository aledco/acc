#include "TestUtils.hpp"


std::string read_input(int test_number)
{
    auto filepath = "Tests/in/test" + std::to_string(test_number) + ".c";
    std::ifstream file(filepath);
    EXPECT_TRUE(file.good());

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

static bool file_exists(std::string filepath)
{
    std::ifstream file(filepath);
    return file.good();
}

std::vector<std::string> read_all_inputs()
{
    std::vector<std::string> inputs;
    auto basepath = "Tests/in/test";
    for (auto i  = 1; file_exists(basepath + std::to_string(i) + ".c"); i++)
    {
        inputs.push_back(read_input(i));
    }

    return inputs;
}
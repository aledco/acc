#include <filesystem>
#include "TestUtils.hpp"

static std::string indir = "Tests/in";
static std::string outdir = "Tests/out";

std::string read_input(int test_id)
{
    auto filepath = indir + "/test" + std::to_string(test_id) + ".c";
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

std::vector<Input> read_all_inputs()
{
    std::vector<Input> inputs;
    auto basepath = indir + "/test";
    for (auto i  = 1; file_exists(basepath + std::to_string(i) + ".c"); i++)
    {
        inputs.push_back(Input(i, basepath + std::to_string(i) + ".c", read_input(i)));
    }

    return inputs;
}

void clear_outdir()
{
    if (std::filesystem::is_directory(outdir))
    {
        std::filesystem::remove_all(outdir);
    }

    std::filesystem::create_directory(outdir);
}

OutputFiles get_output_files(int test_id)
{
    auto test_outdir = outdir + "/test" + std::to_string(test_id);
    std::filesystem::create_directory(test_outdir);
    return OutputFiles(test_outdir);
}
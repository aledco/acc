#include <filesystem>
#include "Environment.hpp"

std::vector<Input> all_inputs;

void Environment::SetUp() 
{
    all_inputs = read_all_inputs();
    clear_outdir();
}

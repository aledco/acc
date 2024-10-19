#include <filesystem>
#include "Environment.hpp"

std::vector<Input> all_inputs;

void Environment::SetUp() 
{
    if (args.test.empty())
    {
        all_inputs = read_all_inputs();
    }
    else
    {
        int t = std::stoi(args.test);
        all_inputs.push_back(read_input(t));
    }
    
    clear_outdir();
}

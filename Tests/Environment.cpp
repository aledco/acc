#include "Environment.hpp"

std::vector<std::string> all_inputs;

void Environment::SetUp() 
{
    all_inputs = read_all_inputs();
}

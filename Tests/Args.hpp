#pragma once


#include <tclap/CmdLine.h>

struct Args
{
    std::string output;
    std::string test;


    Args(int argc, char *argv[])
    {
        TCLAP::CmdLine cmd("Test Alexander's C Compiler", ' ', "1.0");
        TCLAP::ValueArg<std::string> output_arg("o", "output", "Specify the output file", false, "", "string", cmd);
        TCLAP::ValueArg<std::string> test_arg("t", "test", "Specify a test to run", false, "", "string", cmd);

        cmd.parse(argc, argv);

        output = output_arg.getValue();
        test = test_arg.getValue();
    }
};

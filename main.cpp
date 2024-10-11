#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <tclap/CmdLine.h>
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Codegen.hpp"

struct Args
{
    std::vector<std::string> files;
    std::string output;
    bool dump;

    Args(int argc, char *argv[])
    {
        TCLAP::CmdLine cmd("Alexander's C Compiler", ' ', "1.0");
        TCLAP::ValueArg<std::string> output_arg("o", "output", "Specify the output file", false, "", "string", cmd);
        TCLAP::SwitchArg dump_arg("d", "dump", "Dump intermediate representations", cmd, false);
        TCLAP::UnlabeledMultiArg<std::string> file_args("files", "The files to compile", true, "string", cmd);

        cmd.parse(argc, argv);

        files = file_args.getValue();
        output = output_arg.getValue();
        dump = dump_arg.getValue();
    }
};

static _GLIBCXX_NORETURN void error(const std::string& msg)
{
    std::cerr << "error: " << msg << "\n";
    std::exit(1);
}

static std::string read_file(std::string filepath) 
{
    std::ifstream file(filepath);
    if (!file.good())
    {
        error("file not found");
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main(int argc, char *argv[])
{
    Args args(argc, argv);
    
    // for now, process just the first file passed
    std::string input = read_file(args.files[0]);
    Lexer lexer(input);
    Parser parser(lexer);
    auto program = parser.parse();
    
    if (args.dump)
    {
        std::cerr << "AST DUMP:\n";
        program->dump();
        std::cerr << "\n";
    }

    program->ir_codegen();

    if (args.dump)
    {
        std::cerr << "IR DUMP:\n";
        program->ir_list.dump();
        std::cerr << "\n";
    }

    if (!args.output.empty())
    {
        std::ofstream output(args.output);
        codegen(program, &output);
    }
    else
    {
        codegen(program);
    }

    return 0;
}

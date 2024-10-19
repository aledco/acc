#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <tclap/CmdLine.h>
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Codegen.hpp"
#include "Error.hpp"

struct Args
{
    std::vector<std::string> files;
    std::string output;
    bool dump;
    bool emit_llvm;
    bool link_test;


    Args(int argc, char *argv[])
    {
        TCLAP::CmdLine cmd("Alexander's C Compiler", ' ', "1.0");
        TCLAP::ValueArg<std::string> output_arg("o", "output", "Specify the output file", false, "", "string", cmd);
        TCLAP::SwitchArg dump_arg("d", "dump", "Dump intermediate representations", cmd, false);
        TCLAP::SwitchArg emit_llvm_arg("S", "emit-llvm", "Emit LLVM for compiled files", cmd, false);
        TCLAP::SwitchArg link_test_arg("T", "link-test", "Link test LLVM files", cmd, false);
        TCLAP::UnlabeledMultiArg<std::string> file_args("files", "The files to compile", true, "string", cmd);

        cmd.parse(argc, argv);

        files = file_args.getValue();
        output = output_arg.getValue();
        dump = dump_arg.getValue();
        emit_llvm = emit_llvm_arg.getValue();
        link_test = link_test_arg.getValue();
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

static void link_llvm(Args& args, std::vector<std::string> llvm_files)
{
    std::string link_cmd = "llvm-link-18 ";
    for (auto& file : llvm_files)
    {
        link_cmd += file + " ";
    }

    if (args.link_test)
    {
        link_cmd += "$HOME/repos/acc/acc-link/test-code.ll ";
    }

    auto bin_output = args.output.empty() ? "a.out" : args.output;
    link_cmd += " -o " + bin_output;
    std::system(link_cmd.c_str());
}

int main(int argc, char *argv[])
{
    Args args(argc, argv);
    
    // for now, process just the first file passed
    std::string input = read_file(args.files[0]);
    Lexer lexer(input);
    Parser parser(lexer);
    auto program = parser.parse();
    
    try
    {
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
            program->ir_dump();
            std::cerr << "\n";
        }

        if (args.emit_llvm || args.dump)
        {
            if (args.dump)
            {
                std::cerr << "LLVM DUMP:\n";
            }

            codegen(program, &std::cout);
        }
        else
        {   
            auto llvm_output = "a.ll";
            {
                std::ofstream llvm_output_stream(llvm_output);
                codegen(program, &llvm_output_stream);
            }

            std::vector<std::string> llvm_files;
            llvm_files.push_back(llvm_output);
            link_llvm(args, llvm_files);

            for (auto& file : llvm_files) 
            {
                std::remove(file.c_str());
            }
        }
    }
    catch(Error& e)
    {
        std::cerr << e.what();
    }
    
    return 0;
}

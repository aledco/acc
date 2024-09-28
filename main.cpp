#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Lexer.hpp"
#include "Parser.hpp"

static _GLIBCXX_NORETURN void error(const std::string& msg)
{
    std::cerr << "error: " << msg << "\n";
    std::exit(1);
}

static std::string read_file(const char *filepath) 
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
    // TODO add command line arg parsing
    if (argc < 2)
    {
        error("no input file");
    }

    char *filepath = argv[1];
    std::string input = read_file(filepath);
    Lexer lexer(input);
    Parser parser(lexer);
    auto program = parser.parse();
    program->codegen();
    return 0;
}

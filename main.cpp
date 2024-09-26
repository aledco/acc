#include <string>
#include "Lexer.hpp"
#include "Parser.hpp"

int main()
{
    // TODO add command line arg parsing
    std::string input = "int main() { return 0; }"; // TODO read from file
    Lexer lexer(input);
    Parser parser(lexer);
    auto program = parser.parse();
    program->dump();
    return 0;
}

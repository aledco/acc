#include "lexer.hpp"
#include "parser.tab.hpp"

int main()
{
    Lexer lexer;
    yy::parser parser(lexer);    
    return parser();
}

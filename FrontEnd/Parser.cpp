#include "Parser.hpp"

SyntaxTree Parser::parse()
{

    return {};
}

SyntaxTree Parser::parse_varable_declaration()
{

}

FunctionDef Parser::parse_function();
Statement Parser::parse_statement();
CompoundStatement Parser::parse_compound_statement();
Return Parser::parse_return_statement();
Expression Parser::parse_expression();

IntegerConstant Parser::parse_integer_constant()
{

}

Symbol& parse_symbol();

Token& Parser::match(TokenType token_type)
{
    if (current().type == token_type)
    {
        
    }
}
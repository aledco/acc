#pragma once

#include <string>
#include <vector>
#include <memory>
#include "Lexer.hpp"
#include "SyntaxTree.hpp"
#include "SymbolTable.hpp"
#include "Type.hpp"

class Parser
{
private:
    Lexer lexer;
    std::size_t index = 0;

    inline Token& current() { return lexer[index]; }
    inline Token& next() { index++; return current(); }

    Token& match(TokenType token_type);

    SyntaxTree parse_varable_declaration();
    FunctionDef parse_function();
    Statement parse_statement();
    CompoundStatement parse_compound_statement();
    Return parse_return_statement();
    Expression parse_expression();
    IntegerConstant parse_integer_constant();
    //Variable parse_variable();
    
    Symbol& parse_symbol();
public:
    Parser(Lexer& lexer): lexer{lexer} {}

    SyntaxTree parse();
};
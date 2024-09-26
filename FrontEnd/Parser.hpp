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

    Token& match(std::string token_type);
    Token& Parser::match_type();
    bool is_currently(std::initializer_list<std::string> options);

    struct ParserContext
    {
        std::shared_ptr<SymbolTable> global_symbol_table;
        std::shared_ptr<SymbolTable> local_symbol_table;
    };

    SyntaxTree parse_varable_declaration(ParserContext& context);
    FunctionDef parse_function(ParserContext& context);
    Statement parse_statement(ParserContext& context);
    CompoundStatement parse_compound_statement(ParserContext& context);
    Return parse_return_statement(ParserContext& context);
    Expression parse_expression(ParserContext& context);
    IntegerConstant parse_integer_constant(ParserContext& context);
    //Variable parse_variable();

    Type Parser::parse_type(ParserContext& context);

public:
    Parser(Lexer& lexer): lexer{lexer} {}

    SyntaxTree parse();
};
#pragma once

#include <string>
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
    bool is_currently(std::initializer_list<std::string> options);

    struct ParserContext
    {
        std::shared_ptr<SymbolTable> global_symbol_table;
        std::shared_ptr<SymbolTable> local_symbol_table;
    };

    //std::unique_ptr<SyntaxTree> parse_varable_declaration(ParserContext& context);
    std::unique_ptr<FunctionDef> parse_function(ParserContext& context);
    std::unique_ptr<Statement> parse_statement(ParserContext& context);
    std::unique_ptr<CompoundStatement> parse_compound_statement(ParserContext& context);
    std::unique_ptr<Return> parse_return_statement(ParserContext& context);
    std::unique_ptr<Expression> parse_expression(ParserContext& context);
    std::unique_ptr<IntegerConstant> parse_integer_constant(ParserContext& context);
    //Variable parse_variable();

    std::shared_ptr<Type> parse_type(ParserContext& context);

public:
    Parser(Lexer& lexer): lexer{lexer} {}

    std::unique_ptr<Program> parse();
};
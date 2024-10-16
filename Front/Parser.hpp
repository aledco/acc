#pragma once

#include <string>
#include <memory>
#include <vector>
#include <stack>
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
    Token& match(std::initializer_list<std::string> token_types);
    Token& match(std::vector<std::string> token_types);
    bool is_currently(std::initializer_list<std::string> options);
    bool is_currently(std::vector<std::string> options);

    struct ParserContext
    {
    private:
        std::stack<std::shared_ptr<SymbolTable>> symbol_table_stack;

    public:
        std::shared_ptr<SymbolTable> global_symbol_table;

        ParserContext()
        {
            global_symbol_table = std::make_shared<SymbolTable>();
            symbol_table_stack.push(global_symbol_table);
        }

        inline std::shared_ptr<SymbolTable> current_symbol_table() { return symbol_table_stack.top(); }
        inline void push_symbol_table() { symbol_table_stack.push(std::make_shared<SymbolTable>(current_symbol_table())); }
        inline void pop_symbol_table() { symbol_table_stack.pop(); }
    };

    std::shared_ptr<FunctionDef> parse_function(ParserContext& context);
    std::shared_ptr<Symbol> parse_parameter(ParserContext& context);
    std::shared_ptr<Statement> parse_statement(ParserContext& context);
    std::shared_ptr<CompoundStatement> parse_compound_statement(ParserContext& context);
    std::shared_ptr<VariableDeclaration> parse_variable_declaration(ParserContext& context);
    std::shared_ptr<Return> parse_return_statement(ParserContext& context);
    std::shared_ptr<Expression> parse_expression(ParserContext& context);
    std::shared_ptr<Expression> parse_expression(ParserContext& context, int p);
    std::shared_ptr<Expression> parse_unary(ParserContext& context);
    std::shared_ptr<Expression> parse_term(ParserContext& context);

    std::shared_ptr<Type> parse_type(ParserContext& context);
    std::shared_ptr<Symbol> parse_identifier(ParserContext& context);

    _GLIBCXX_NORETURN void error(std::initializer_list<std::string> token_types);
    _GLIBCXX_NORETURN void error(std::vector<std::string> token_types);

public:
    Parser(Lexer& lexer): lexer{lexer} {}

    std::shared_ptr<Program> parse();
};
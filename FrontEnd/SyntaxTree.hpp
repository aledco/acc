#pragma once

#include <optional>
#include <vector>
#include <memory>
#include "SymbolTable.hpp"
#include "Type.hpp"

struct SyntaxTree 
{
};

struct Expression : SyntaxTree
{
    std::unique_ptr<Type> type;
};

struct IntegerConstant : Expression
{
    long value;
    IntegerConstant(long value): value(value) {}
};

struct Statement : SyntaxTree
{
};

struct CompoundStatement : Statement
{
    std::vector<Statement> statements;
    CompoundStatement(std::vector<Statement> statements): statements(statements) {}
};

struct Return : Statement
{
    std::unique_ptr<Expression> expr;
    Return(): expr(nullptr) {}
    Return(std::unique_ptr<Expression> expr): expr(std::move(expr)) {}
};

struct FunctionDef : SyntaxTree
{
    std::shared_ptr<Symbol> function;
    std::vector<std::shared_ptr<Symbol>> params;
    CompoundStatement body;
    FunctionDef(std::shared_ptr<Symbol> function, std::vector<std::shared_ptr<Symbol>> params, CompoundStatement body):
        function(function), 
        params(params), 
        body(body) 
    {}
};

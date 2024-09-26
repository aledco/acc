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
};

struct Return : Statement
{
    std::unique_ptr<Expression> expr;
    Return(): expr(nullptr) {}
    Return(std::unique_ptr<Expression> expr): expr(std::move(expr)) {}
};

struct FunctionDef : SyntaxTree
{
    Symbol& function;
    std::vector<Symbol&> params;
    CompoundStatement body;
};

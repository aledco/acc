#pragma once

#include <optional>
#include <vector>
#include <memory>
#include "SymbolTable.hpp"
#include "Type.hpp"

struct SyntaxTree 
{
    virtual void dump(int depth = 0) = 0;
};

struct Expression : SyntaxTree
{
    std::unique_ptr<Type> type;
    //virtual void dump(int depth = 0) = 0;
};

struct IntegerConstant : Expression
{
    long value;
    IntegerConstant(long value): value(value) {}
    void dump(int depth = 0) override;
};

struct Statement : SyntaxTree
{
    //virtual void dump(int depth = 0) = 0;
};

struct CompoundStatement : Statement
{
    std::vector<std::unique_ptr<Statement>> statements;
    CompoundStatement(std::vector<std::unique_ptr<Statement>> statements): statements(statements) {}
    void dump(int depth = 0) override;
};

struct Return : Statement
{
    std::unique_ptr<Expression> expr;
    Return(): expr(nullptr) {}
    Return(std::unique_ptr<Expression> expr): expr(std::move(expr)) {}
    void dump(int depth = 0) override;
};

struct FunctionDef : SyntaxTree
{
    std::shared_ptr<Symbol> function;
    std::vector<std::shared_ptr<Symbol>> params;
    std::unique_ptr<CompoundStatement> body;
    FunctionDef(std::shared_ptr<Symbol> function, std::vector<std::shared_ptr<Symbol>> params, std::unique_ptr<CompoundStatement> body):
        function(function), 
        params(params), 
        body(std::move(body)) 
    {}
    void dump(int depth = 0) override;
};

struct Program : SyntaxTree
{
    std::vector<std::unique_ptr<FunctionDef>> functions;
    Program(std::vector<std::unique_ptr<FunctionDef>> functions) : functions(functions) {}
    void dump(int depth = 0) override;
};

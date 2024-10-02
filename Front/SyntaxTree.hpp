#pragma once

#include <optional>
#include <vector>
#include <memory>
#include "SymbolTable.hpp"
#include "Type.hpp"
#include "Operator.hpp"
#include "Quad.hpp"

struct SyntaxTree 
{
    QuadList ir_list;
    virtual void dump(int depth = 0) = 0;
};

struct Expression : SyntaxTree
{
    std::shared_ptr<Type> type;
};

struct Variable : Expression
{
    std::shared_ptr<Symbol> symbol;
    Variable(std::shared_ptr<Symbol> symbol) : symbol(symbol) {}
    void dump(int depth = 0) override;
};

struct Assignment : Expression
{
    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;
    Assignment(std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs): lhs(lhs), rhs(rhs) {}
    void dump(int depth = 0) override;
};

struct FunctionCall : Expression
{
    std::shared_ptr<Symbol> function;
    std::vector<std::shared_ptr<Expression>> args;
    FunctionCall(std::shared_ptr<Symbol> function, std::vector<std::shared_ptr<Expression>> args): function(function), args(args) {}
    void dump(int depth = 0) override;
};

struct BinaryOperation : Expression
{
    BinOp op;
    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;
    BinaryOperation(BinOp op, std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs): op(op), lhs(lhs), rhs(rhs) {}
    void dump(int depth = 0) override;
};

struct UnaryOperation : Expression
{
    UnOp op;
    std::shared_ptr<Expression> expr;
    UnaryOperation(UnOp op, std::shared_ptr<Expression> expr): op(op), expr(expr) {}
    void dump(int depth = 0) override;
};

struct IntegerConstant : Expression
{
    long value;
    IntegerConstant(long value): value(value) {}
    void dump(int depth = 0) override;
};

struct CharConstant : Expression
{
    char value;
    CharConstant(long value): value(value) {}
    void dump(int depth = 0) override;
};

struct Statement : SyntaxTree
{
};

struct CompoundStatement : Statement
{
    std::vector<std::shared_ptr<Statement>> statements;
    CompoundStatement(std::vector<std::shared_ptr<Statement>> statements): statements(statements) {}
    void dump(int depth = 0) override;
};

struct VariableDeclaration : Statement
{
    std::shared_ptr<Type> type;
    std::shared_ptr<Symbol> symbol;
    VariableDeclaration(std::shared_ptr<Type> type, std::shared_ptr<Symbol> symbol) : type(type), symbol(symbol) {}
    void dump(int depth = 0) override;
};

struct Return : Statement
{
    std::shared_ptr<Expression> expr;
    Return(): expr(nullptr) {}
    Return(std::shared_ptr<Expression> expr): expr(expr) {}
    void dump(int depth = 0) override;
};

struct FunctionDef : SyntaxTree
{
    std::shared_ptr<Symbol> function;
    std::vector<std::shared_ptr<Symbol>> params;
    std::shared_ptr<CompoundStatement> body;
    FunctionDef(std::shared_ptr<Symbol> function, std::vector<std::shared_ptr<Symbol>> params, std::shared_ptr<CompoundStatement> body):
        function(function), 
        params(params), 
        body(body)
    {}
    void dump(int depth = 0) override;
};

struct Program : SyntaxTree
{
    std::vector<std::shared_ptr<FunctionDef>> functions;
    Program(const std::vector<std::shared_ptr<FunctionDef>>& functions) : functions(functions) {}
    void dump(int depth = 0) override;
};

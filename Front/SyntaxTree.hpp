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
    virtual QuadList ir_codegen() = 0;
    virtual void dump(int depth = 1) = 0;
};

struct Statement : SyntaxTree
{
};

struct Expression : Statement
{
    std::shared_ptr<Type> type;
};

struct CompoundStatement : Statement
{
    std::vector<std::shared_ptr<Statement>> statements;
    std::shared_ptr<SymbolTable> symbol_table;

    CompoundStatement(std::vector<std::shared_ptr<Statement>> statements, std::shared_ptr<SymbolTable> symbol_table): 
        statements(statements), 
        symbol_table(symbol_table)
    {}

    QuadList ir_codegen() override;
    void dump(int depth = 1) override;
};

struct VariableDeclaration : Statement
{
    std::shared_ptr<Type> type;
    std::shared_ptr<Symbol> symbol;

    VariableDeclaration(std::shared_ptr<Type> type, std::shared_ptr<Symbol> symbol) : type(type), symbol(symbol) {}

    QuadList ir_codegen() override;
    void dump(int depth = 1) override;
};

struct Return : Statement
{
    std::shared_ptr<Expression> expr;

    Return(): expr(nullptr) {}
    Return(std::shared_ptr<Expression> expr): expr(expr) {}

    QuadList ir_codegen() override;
    void dump(int depth = 1) override;
};

struct Variable : Expression
{
    std::shared_ptr<Symbol> symbol;

    Variable(std::shared_ptr<Symbol> symbol) : symbol(symbol) {}

    QuadList ir_codegen() override;
    void dump(int depth = 1) override;
};

struct FunctionCall : Expression
{
    std::shared_ptr<Symbol> function;
    std::vector<std::shared_ptr<Expression>> args;

    FunctionCall(std::shared_ptr<Symbol> function, std::vector<std::shared_ptr<Expression>> args): function(function), args(args) {}

    QuadList ir_codegen() override;
    void dump(int depth = 1) override;
};

struct BinaryOperation : Expression
{
    BinOp op;
    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;

    BinaryOperation(BinOp op, std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs): op(op), lhs(lhs), rhs(rhs) {}

    QuadList ir_codegen() override;
    void dump(int depth = 1) override;
};

struct UnaryOperation : Expression
{
    UnOp op;
    std::shared_ptr<Expression> expr;

    UnaryOperation(UnOp op, std::shared_ptr<Expression> expr): op(op), expr(expr) {}

    QuadList ir_codegen() override;
    void dump(int depth = 1) override;
};

struct IntegerConstant : Expression
{
    long value;

    IntegerConstant(long value): value(value) {}

    QuadList ir_codegen() override;
    void dump(int depth = 1) override;
};

struct CharConstant : Expression
{
    char value;

    CharConstant(long value): value(value) {}

    QuadList ir_codegen() override;
    void dump(int depth = 1) override;
};

struct FunctionDef : SyntaxTree
{
    std::shared_ptr<Symbol> function;
    std::vector<std::shared_ptr<Symbol>> params;
    std::shared_ptr<CompoundStatement> body;
    std::shared_ptr<SymbolTable> symbol_table;
    FunctionDef(std::shared_ptr<Symbol> function, std::vector<std::shared_ptr<Symbol>> params, std::shared_ptr<CompoundStatement> body, std::shared_ptr<SymbolTable> symbol_table):
        function(function), 
        params(params), 
        body(body),
        symbol_table(symbol_table)
    {}

    QuadList ir_codegen() override;
    void dump(int depth = 1) override;
};

struct Program : SyntaxTree
{
    std::vector<std::shared_ptr<FunctionDef>> functions;
    std::shared_ptr<SymbolTable> global_symbol_table;

    Program(const std::vector<std::shared_ptr<FunctionDef>>& functions,  std::shared_ptr<SymbolTable> global_symbol_table):
        functions(functions),
        global_symbol_table(global_symbol_table)
    {}

    QuadList ir_codegen() override;
    void dump(int depth = 1) override;
};

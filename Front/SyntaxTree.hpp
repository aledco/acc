#pragma once

#include <optional>
#include <vector>
#include <memory>
#include "SymbolTable.hpp"
#include "Type.hpp"
#include "Operator.hpp"
#include "Quad.hpp"

struct FunctionDef;

struct SyntaxTree 
{
protected:
    struct TypecheckContext 
    {
        std::shared_ptr<FunctionDef> func_def;
    };

public:
    std::shared_ptr<SymbolTable> symbol_table;
    QuadList ir_list;

    SyntaxTree(std::shared_ptr<SymbolTable> symbol_table) : symbol_table(symbol_table) {}

    virtual void typecheck(TypecheckContext& context) = 0;
    virtual void ir_codegen() = 0;
    virtual void dump(int depth = 1) = 0;
};

struct Statement : SyntaxTree
{
    Statement(std::shared_ptr<SymbolTable> symbol_table) : SyntaxTree(symbol_table) {}
};

struct Expression : Statement
{
    std::shared_ptr<Type> type;
    std::shared_ptr<Symbol> place;
    std::shared_ptr<Symbol> location;

    Expression(std::shared_ptr<SymbolTable> symbol_table) : Statement(symbol_table) {}

    virtual void ir_codegen_lval();
};

struct CompoundStatement : Statement
{
    std::vector<std::shared_ptr<Statement>> statements;

    CompoundStatement(std::vector<std::shared_ptr<Statement>> statements, std::shared_ptr<SymbolTable> symbol_table):
        Statement(symbol_table),
        statements(statements)
    {}

    void typecheck(TypecheckContext& context) override;
    void ir_codegen() override;
    void dump(int depth = 1) override;
};

struct VariableDeclaration : Statement
{
    std::shared_ptr<Type> type;
    std::shared_ptr<Symbol> symbol;

    VariableDeclaration(std::shared_ptr<Type> type, std::shared_ptr<Symbol> symbol, std::shared_ptr<SymbolTable> symbol_table) : 
        Statement(symbol_table), 
        type(type), 
        symbol(symbol) 
    {}

    void typecheck(TypecheckContext& context) override;
    void ir_codegen() override;
    void dump(int depth = 1) override;
};

struct Return : Statement
{
    std::shared_ptr<Expression> expr;

    Return(std::shared_ptr<SymbolTable> symbol_table): Statement(symbol_table), expr(nullptr) {}
    Return(std::shared_ptr<Expression> expr, std::shared_ptr<SymbolTable> symbol_table) : Statement(symbol_table), expr(expr) {}

    void typecheck(TypecheckContext& context) override;
    void ir_codegen() override;
    void dump(int depth = 1) override;
};

struct Variable : Expression
{
    std::shared_ptr<Symbol> symbol;

    Variable(std::shared_ptr<Symbol> symbol, std::shared_ptr<SymbolTable> symbol_table) : Expression(symbol_table), symbol(symbol) {}

    void typecheck(TypecheckContext& context) override;
    void ir_codegen() override;
    void ir_codegen_lval() override;
    void dump(int depth = 1) override;
};

struct FunctionCall : Expression
{
    std::shared_ptr<Symbol> function;
    std::vector<std::shared_ptr<Expression>> args;

    FunctionCall(std::shared_ptr<Symbol> function, std::vector<std::shared_ptr<Expression>> args, std::shared_ptr<SymbolTable> symbol_table) :
        Expression(symbol_table),
        function(function),
        args(args)
    {}

    void typecheck(TypecheckContext& context) override;
    void ir_codegen() override;
    void dump(int depth = 1) override;
};

struct BinaryOperation : Expression
{
    BinOp op;
    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;

    BinaryOperation(BinOp op, std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs, std::shared_ptr<SymbolTable> symbol_table) :
        Expression(symbol_table),
        op(op),
        lhs(lhs),
        rhs(rhs)
    {}

    void typecheck(TypecheckContext& context) override;
    void ir_codegen() override;
    void dump(int depth = 1) override;
};

struct UnaryOperation : Expression
{
    UnOp op;
    std::shared_ptr<Expression> expr;

    UnaryOperation(UnOp op, std::shared_ptr<Expression> expr, std::shared_ptr<SymbolTable> symbol_table) : Expression(symbol_table), op(op), expr(expr) {}

    void typecheck(TypecheckContext& context) override;
    void ir_codegen() override;
    void ir_codegen_lval() override;
    void dump(int depth = 1) override;
};

struct IntegerConstant : Expression
{
    long value;

    IntegerConstant(long value, std::shared_ptr<SymbolTable> symbol_table) : Expression(symbol_table), value(value) {}

    void typecheck(TypecheckContext& context) override;
    void ir_codegen() override;
    void dump(int depth = 1) override;
};

struct CharConstant : Expression
{
    char value;

    CharConstant(long value, std::shared_ptr<SymbolTable> symbol_table) : Expression(symbol_table), value(value) {}

    void typecheck(TypecheckContext& context) override;
    void ir_codegen() override;
    void dump(int depth = 1) override;
};

struct FunctionDef : SyntaxTree
{
    std::shared_ptr<Symbol> function;
    std::vector<std::shared_ptr<Symbol>> params;
    std::shared_ptr<CompoundStatement> body;

    FunctionDef(std::shared_ptr<Symbol> function, std::vector<std::shared_ptr<Symbol>> params, std::shared_ptr<CompoundStatement> body, std::shared_ptr<SymbolTable> symbol_table):
        SyntaxTree(symbol_table),
        function(function), 
        params(params), 
        body(body)
    {}

    void typecheck(TypecheckContext& context) override;
    void ir_codegen() override;
    void dump(int depth = 1) override;
};

struct Program : SyntaxTree
{
    std::vector<std::shared_ptr<FunctionDef>> functions;

    Program(const std::vector<std::shared_ptr<FunctionDef>>& functions,  std::shared_ptr<SymbolTable> symbol_table):
        SyntaxTree(symbol_table),
        functions(functions)
    {}

    void typecheck(TypecheckContext& context) override;
    void typecheck();
    void ir_codegen() override;
    void dump(int depth = 1) override;
};

#pragma once

#include <optional>
#include <vector>
#include <memory>
#include "Span.hpp"
#include "SymbolTable.hpp"
#include "Type.hpp"
#include "Operator.hpp"
#include "Quad.hpp"
#include "CFG.hpp"

struct FunctionDef;

/**
 * Base class for all AST nodes.
 */
struct SyntaxTree 
{
protected:
    struct TypecheckContext 
    {
        std::shared_ptr<FunctionDef> func_def;
    };

public:
    Span span;
    std::shared_ptr<SymbolTable> symbol_table;
    QuadList ir_list;

    SyntaxTree(Span span, std::shared_ptr<SymbolTable> symbol_table) : span(span), symbol_table(symbol_table) {}

    virtual void typecheck(TypecheckContext& context) = 0;
    virtual void ir_codegen() = 0;
    virtual void dump(int depth = 1) = 0;
};

/**
 * Base class for all statement AST nodes.
 */
struct Statement : SyntaxTree
{
    Statement(Span span, std::shared_ptr<SymbolTable> symbol_table) : SyntaxTree(span, symbol_table) {}
};

/**
 * Base class for all expression AST nodes.
 */
struct Expression : Statement
{
    std::shared_ptr<Type> type;
    std::shared_ptr<Operand> place;
    std::shared_ptr<Operand> location;

    Expression(Span span, std::shared_ptr<SymbolTable> symbol_table) : Statement(span, symbol_table) {}

    virtual void ir_codegen_lval();
    virtual void ir_codegen_bool(std::shared_ptr<Operand> true_label, std::shared_ptr<Operand> false_label);
};

/**
 * The compound statement AST node.
 */
struct CompoundStatement : Statement
{
    std::vector<std::shared_ptr<Statement>> statements;

    CompoundStatement(Span span, std::vector<std::shared_ptr<Statement>> statements, std::shared_ptr<SymbolTable> symbol_table):
        Statement(span, symbol_table),
        statements(statements)
    {}

    void typecheck(TypecheckContext& context) override;
    void ir_codegen() override;
    void dump(int depth = 1) override;
};

/**
 * The variable declaration AST node.
 */
struct VariableDeclaration : Statement
{
    std::shared_ptr<Type> type;
    std::shared_ptr<Symbol> symbol;

    VariableDeclaration(Span span, std::shared_ptr<Type> type, std::shared_ptr<Symbol> symbol, std::shared_ptr<SymbolTable> symbol_table) : 
        Statement(span, symbol_table), 
        type(type), 
        symbol(symbol) 
    {}

    void typecheck(TypecheckContext& context) override;
    void ir_codegen() override;
    void dump(int depth = 1) override;
};

/**
 * The return statement AST node.
 */
struct Return : Statement
{
    std::shared_ptr<Expression> expr;

    Return(Span span, std::shared_ptr<SymbolTable> symbol_table): Statement(span, symbol_table), expr(nullptr) {}
    Return(Span span, std::shared_ptr<Expression> expr, std::shared_ptr<SymbolTable> symbol_table) : Statement(span, symbol_table), expr(expr) {}

    void typecheck(TypecheckContext& context) override;
    void ir_codegen() override;
    void dump(int depth = 1) override;
};

/**
 * The if statement AST node.
 */
struct IfStatement : Statement
{
    std::shared_ptr<Expression> guard;
    std::shared_ptr<Statement> then_stmt;
    std::shared_ptr<Statement> else_stmt;

    IfStatement(Span span, std::shared_ptr<Expression> guard, std::shared_ptr<Statement> then_stmt, std::shared_ptr<Statement> else_stmt, std::shared_ptr<SymbolTable> symbol_table) :
        Statement(span, symbol_table),
        guard(guard), 
        then_stmt(then_stmt), 
        else_stmt(else_stmt)
    {}

    IfStatement(Span span, std::shared_ptr<Expression> guard, std::shared_ptr<Statement> then_stmt, std::shared_ptr<SymbolTable> symbol_table) :
        Statement(span, symbol_table),
        guard(guard), 
        then_stmt(then_stmt), 
        else_stmt(nullptr)
    {}

    void typecheck(TypecheckContext& context) override;
    void ir_codegen() override;
    void dump(int depth = 1) override;
};

/**
 * The while loop AST node.
 */
struct WhileLoop : Statement
{
    std::shared_ptr<Expression> guard;
    std::shared_ptr<Statement> body;

    WhileLoop(Span span, std::shared_ptr<Expression> guard, std::shared_ptr<Statement> body, std::shared_ptr<SymbolTable> symbol_table) :
        Statement(span, symbol_table),
        guard(guard), 
        body(body)
    {}

    void typecheck(TypecheckContext& context) override;
    void ir_codegen() override;
    void dump(int depth = 1) override;
};

/**
 * The for loop AST node.
 */
struct ForLoop : Statement
{
    std::shared_ptr<Expression> init;
    std::shared_ptr<Expression> guard;
    std::shared_ptr<Expression> update;
    std::shared_ptr<Statement> body;

    ForLoop(Span span,
            std::shared_ptr<Expression> init_stmt, 
            std::shared_ptr<Expression> guard, 
            std::shared_ptr<Expression> update_stmt, 
            std::shared_ptr<Statement> body, 
            std::shared_ptr<SymbolTable> symbol_table) :
        Statement(span, symbol_table),
        init(init_stmt),
        guard(guard), 
        update(update_stmt),
        body(body)
    {}

    void typecheck(TypecheckContext& context) override;
    void ir_codegen() override;
    void dump(int depth = 1) override;
};

/**
 * The variable AST node.
 */
struct Variable : Expression
{
    std::shared_ptr<Symbol> symbol;

    Variable(Span span, std::shared_ptr<Symbol> symbol, std::shared_ptr<SymbolTable> symbol_table) : Expression(span, symbol_table), symbol(symbol) {}

    void typecheck(TypecheckContext& context) override;
    void ir_codegen() override;
    void ir_codegen_lval() override;
    void dump(int depth = 1) override;
};

/**
 * The function call AST node.
 */
struct FunctionCall : Expression
{
    std::shared_ptr<Symbol> function;
    std::vector<std::shared_ptr<Expression>> args;

    FunctionCall(Span span, std::shared_ptr<Symbol> function, std::vector<std::shared_ptr<Expression>> args, std::shared_ptr<SymbolTable> symbol_table) :
        Expression(span, symbol_table),
        function(function),
        args(args)
    {}

    void typecheck(TypecheckContext& context) override;
    void ir_codegen() override;
    void dump(int depth = 1) override;
};

/**
 * The binary operation AST node.
 */
struct BinaryOperation : Expression
{
    BinOp op;
    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;

    BinaryOperation(Span span, BinOp op, std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs, std::shared_ptr<SymbolTable> symbol_table) :
        Expression(span, symbol_table),
        op(op),
        lhs(lhs),
        rhs(rhs)
    {}

    void typecheck(TypecheckContext& context) override;
    void ir_codegen() override;
    void ir_codegen_bool(std::shared_ptr<Operand> true_label, std::shared_ptr<Operand> false_label) override;
    void dump(int depth = 1) override;
};

/**
 * The unary operation AST node.
 */
struct UnaryOperation : Expression
{
    UnOp op;
    std::shared_ptr<Expression> expr;
    bool is_postfix = false;

    UnaryOperation(Span span, UnOp op, std::shared_ptr<Expression> expr, std::shared_ptr<SymbolTable> symbol_table) : 
        Expression(span, symbol_table),
        op(op), 
        expr(expr)
    {}

    UnaryOperation(Span span, UnOp op, std::shared_ptr<Expression> expr, std::shared_ptr<SymbolTable> symbol_table, bool is_postfix) : 
        Expression(span, symbol_table),
        op(op), 
        expr(expr),
        is_postfix(is_postfix)
    {}

    void typecheck(TypecheckContext& context) override;
    void ir_codegen() override;
    void ir_codegen_lval() override;
    void ir_codegen_bool(std::shared_ptr<Operand> true_label, std::shared_ptr<Operand> false_label) override;
    void dump(int depth = 1) override;
};

/**
 * The integer constant AST node.
 */
struct IntegerConstant : Expression
{
    long value;

    IntegerConstant(Span span, long value, std::shared_ptr<SymbolTable> symbol_table) : Expression(span, symbol_table), value(value) {}

    void typecheck(TypecheckContext& context) override;
    void ir_codegen() override;
    void dump(int depth = 1) override;
};

/**
 * The char constant AST node.
 */
struct CharConstant : Expression
{
    char value;

    CharConstant(Span span, long value, std::shared_ptr<SymbolTable> symbol_table) : Expression(span, symbol_table), value(value) {}

    void typecheck(TypecheckContext& context) override;
    void ir_codegen() override;
    void dump(int depth = 1) override;
};

/**
 * The function AST node.
 */
struct FunctionDef : SyntaxTree
{
    std::shared_ptr<Symbol> function;
    std::vector<std::shared_ptr<Symbol>> params;
    std::shared_ptr<CompoundStatement> body;

    std::vector<std::shared_ptr<BasicBlock>> cfg;

    FunctionDef(Span span, std::shared_ptr<Symbol> function, std::vector<std::shared_ptr<Symbol>> params, std::shared_ptr<CompoundStatement> body, std::shared_ptr<SymbolTable> symbol_table):
        SyntaxTree(span, symbol_table),
        function(function), 
        params(params),
        body(body)
    {}

    FunctionDef(Span span, std::shared_ptr<Symbol> function, std::vector<std::shared_ptr<Symbol>> params, std::shared_ptr<SymbolTable> symbol_table):
        SyntaxTree(span, symbol_table),
        function(function), 
        params(params),
        body(nullptr)
    {}

    void typecheck(TypecheckContext& context) override;
    void ir_codegen() override;
    void dump(int depth = 1) override;

    inline bool is_proto() { return body == nullptr; }
};

/**
 * The program AST node.
 */
struct Program : SyntaxTree
{
    std::vector<std::shared_ptr<FunctionDef>> functions;

    Program(Span span, const std::vector<std::shared_ptr<FunctionDef>>& functions,  std::shared_ptr<SymbolTable> symbol_table):
        SyntaxTree(span, symbol_table),
        functions(functions)
    {}

    void typecheck(TypecheckContext& context) override;
    void typecheck();
    void ir_codegen() override;
    void dump(int depth = 1) override;
    void ir_dump();
};

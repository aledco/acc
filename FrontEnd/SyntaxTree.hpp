#pragma once

#include <optional>
#include <vector>
#include <memory>
#include "SymbolTable.hpp"
#include "Type.hpp"
#include "CodegenContext.hpp"
#include "CodegenResult.hpp"

struct SyntaxTree 
{
    virtual void dump(int depth = 0) = 0;
    virtual std::unique_ptr<CodegenResult> codegen(CodegenContext& context, bool lvalue = false) = 0;
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
    std::unique_ptr<CodegenResult> codegen(CodegenContext& context, bool lvalue = false) override;
};

struct Assignment : Expression
{
    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;
    Assignment(std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs): lhs(lhs), rhs(rhs) {}
    void dump(int depth = 0) override;
    std::unique_ptr<CodegenResult> codegen(CodegenContext& context, bool lvalue = false) override;
}

struct IntegerConstant : Expression
{
    long value;
    IntegerConstant(long value): value(value) {}
    void dump(int depth = 0) override;
    std::unique_ptr<CodegenResult> codegen(CodegenContext& context, bool lvalue = false) override;
};

struct Statement : SyntaxTree
{
};

struct CompoundStatement : Statement
{
    std::vector<std::shared_ptr<Statement>> statements;
    CompoundStatement(std::vector<std::shared_ptr<Statement>> statements): statements(statements) {}
    void dump(int depth = 0) override;
    std::unique_ptr<CodegenResult> codegen(CodegenContext& context, bool lvalue = false) override;
};

struct VariableDeclaration : Statement
{
    std::shared_ptr<Type> type;
    std::shared_ptr<Symbol> symbol;
    VariableDeclaration(std::shared_ptr<Type> type, std::shared_ptr<Symbol> symbol) : type(type), symbol(symbol) {}
    void dump(int depth = 0) override;
    std::unique_ptr<CodegenResult> codegen(CodegenContext& context, bool lvalue = false) override;
};

struct Return : Statement
{
    std::shared_ptr<Expression> expr;
    Return(): expr(nullptr) {}
    Return(std::shared_ptr<Expression> expr): expr(expr) {}
    void dump(int depth = 0) override;
    std::unique_ptr<CodegenResult> codegen(CodegenContext& context, bool lvalue = false) override;
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
    std::unique_ptr<CodegenResult> codegen(CodegenContext& context, bool lvalue = false) override;
};

struct Program : SyntaxTree
{
    std::vector<std::shared_ptr<FunctionDef>> functions;
    Program(const std::vector<std::shared_ptr<FunctionDef>>& functions) : functions(functions) {}
    void dump(int depth = 0) override;
    std::unique_ptr<CodegenResult> codegen(CodegenContext& context, bool lvalue = false) override;
    void codegen();
};

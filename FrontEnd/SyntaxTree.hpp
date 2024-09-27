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
    virtual std::unique_ptr<CodegenResult> codegen(CodegenContext& context) = 0;
};

struct Expression : SyntaxTree
{
    std::shared_ptr<Type> type;
};

struct IntegerConstant : Expression
{
    long value;
    IntegerConstant(long value): value(value) {}
    void dump(int depth = 0) override;
    std::unique_ptr<CodegenResult> codegen(CodegenContext& context) override;
};

struct Statement : SyntaxTree
{
};

struct CompoundStatement : Statement
{
    std::vector<std::shared_ptr<Statement>> statements;
    CompoundStatement(std::vector<std::shared_ptr<Statement>> statements): statements(statements) {}
    void dump(int depth = 0) override;
    std::unique_ptr<CodegenResult> codegen(CodegenContext& context) override;
};

struct Return : Statement
{
    std::shared_ptr<Expression> expr;
    Return(): expr(nullptr) {}
    Return(std::shared_ptr<Expression> expr): expr(expr) {}
    void dump(int depth = 0) override;
    std::unique_ptr<CodegenResult> codegen(CodegenContext& context) override;
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
    std::unique_ptr<CodegenResult> codegen(CodegenContext& context) override;
};

struct Program : SyntaxTree
{
    std::vector<std::shared_ptr<FunctionDef>> functions;
    Program(const std::vector<std::shared_ptr<FunctionDef>>& functions) : functions(functions) {}
    void dump(int depth = 0) override;
    std::unique_ptr<CodegenResult> codegen(CodegenContext& context) override;
    void codegen();
};

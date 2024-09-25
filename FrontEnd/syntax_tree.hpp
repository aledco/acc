#pragma once

#include <optional>

struct TreeNode 
{
};

struct Expression : TreeNode
{
};

struct IntegerConstant : Expression
{
    long value;
    IntegerConstant(long value): value(value) {}
};

struct Statement : TreeNode
{
};

struct Return : Statement
{
    std::optional<Expression> expr;
    Return(): expr({}) {}
    Return(Expression& expr): expr(expr) {}
};

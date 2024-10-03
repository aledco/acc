#include <iostream>
#include <string>
#include "Operator.hpp"
#include "SyntaxTree.hpp"

static void indent(int depth)
{
    std::cout << std::string(depth, '\t');
}

void IntegerConstant::dump(int depth)
{
    std::cout << "IntegerConstant(" << value << ")";
}

void CharConstant::dump(int depth)
{
    std::cout << "CharConstant('" << value << "')";
}

void Variable::dump(int depth)
{
    std::cout << "Variable(" << symbol->name << ")";
}

void FunctionCall::dump(int depth)
{
    std::cout << "FunctionCall(\n";
    indent(depth);
    std::cout << "name = " << function->name << ",\n";
    indent(depth);
    std::cout << "args = (";
    // TODO put each arg on new line
    std::cout <<")\n";
}

void BinaryOperation::dump(int depth)
{
    std::cout << "BinaryOperation(\n";
    indent(depth);
    std::cout << "op = '" << toString(op) << "',\n";
    indent(depth);
    lhs->dump(depth + 1);
    std::cout << ",\n";
    indent(depth);
    rhs->dump(depth + 1);
    std::cout << ")";
}

void UnaryOperation::dump(int depth)
{
    std::cout << "UnaryOperation(\n";
    indent(depth);
    std::cout << "op = " << toString(op) << ",\n";
    indent(depth);
    std::cout << "expr = ";
    expr->dump(depth);
    std::cout << "\n";
    indent(depth);
    std::cout << ")";
}

void CompoundStatement::dump(int depth)
{
    std::cout << "CompoundStatement(\n";
    for (auto i = 0; i < statements.size(); i++)
    {
        indent(depth);
        statements[i]->dump(depth + 1);
        if (i < statements.size() - 1)
        {
            std::cout << ",\n";
        }
    }

    std::cout << ")";
}

void VariableDeclaration::dump(int depth)
{
    std::cout << "VariableDeclaration(" << symbol->name << ")";
}

void Return::dump(int depth)
{
    std::cout << "Return(";
    if (expr != nullptr)
    {
        std::cout << "expr = ";
        expr->dump(depth + 1);
    }

    std::cout << ")";
}

void FunctionDef::dump(int depth)
{
    std::cout << "FunctionDef(\n";
    indent(depth);
    std::cout << "name = " << function->name << ",\n";
    
    indent(depth);
    std::cout << "params = (";
    for (auto& param : params)
    {
        std::cout << param->name << ", ";
    }

    std::cout << "),\n";
    indent(depth);
    std::cout << "body = ";

    body->dump(depth + 1);
    std::cout << ")";
}

void Program::dump(int depth)
{   
    std::cout << "Program(\n";
    for (auto i = 0; i < functions.size(); i++)
    {
        indent(depth);
        functions[i]->dump(depth + 1);
        if (i < functions.size() - 1)
        {
            std::cout << ",\n";
        }
    }

    std::cout << ")\n";
}
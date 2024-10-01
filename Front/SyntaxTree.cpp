#include <iostream>
#include <string>
#include "SyntaxTree.hpp"

void IntegerConstant::dump(int depth)
{
    std::cout << std::string(depth, '\t') << "IntegerConstant(" << value << ")\n";
}

void CharConstant::dump(int depth)
{
    std::cout << std::string(depth, '\t') << "CharConstant('" << value << "')\n";
}

void Variable::dump(int depth)
{
    std::cout << std::string(depth, '\t') << "Variable(" << symbol->name << ")\n";
}

void Assignment::dump(int depth)
{
    std::cout << std::string(depth, '\t') << "Assignment(";
    lhs->dump(depth + 1);
    rhs->dump(depth + 1);
    std::cout <<")\n";
}

void FunctionCall::dump(int depth)
{
    std::cout << std::string(depth, '\t') << "FunctionCall(";
    std::cout <<")\n";
}

void BinaryOperation::dump(int depth)
{
    std::cout << std::string(depth, '\t') << "BinaryOperation(";
    std::cout <<")\n";
}

void UnaryOperation::dump(int depth)
{
    std::cout << std::string(depth, '\t') << "UnaryOperation(";
    std::cout <<")\n";
}

void CompoundStatement::dump(int depth)
{
    std::cout << std::string(depth, '\t') << "CompoundStatement(";
    for (auto& statement : statements)
    {
        statement->dump(depth + 1);
        std::cout << ",\n";
    }

    std::cout << ")\n";
}

void VariableDeclaration::dump(int depth)
{
    std::cout << std::string(depth, '\t') << "VariableDeclaration(" << symbol->name << ")\n";
}

void Return::dump(int depth)
{
    std::cout << std::string(depth, '\t') << "Return(";
    if (expr != nullptr)
    {
        std::cout << " ";
        expr->dump(depth);
    }

    std::cout << ")\n";
}

void FunctionDef::dump(int depth)
{
    std::cout << std::string(depth, '\t') << "FunctionDef(";
    // TODO write symbol dump func

    std::cout << "name = " << function->name << ", parms = (";
    for (auto& param : params)
    {
        std::cout << param->name << ", ";
    }

    body->dump(depth);
    std::cout << ")\n";
}

void Program::dump(int depth)
{   
    std::cout << std::string(depth, '\t') << "Program(";
    for (auto& function : functions)
    {
        function->dump(depth + 1);
        std::cout << ",\n";
    }

    std::cout << ")\n";
}
#include <iostream>
#include <string>
#include "SyntaxTree.hpp"

void IntegerConstant::dump(int depth)
{
    std::cout << std::string(depth, '\t') << "IntegerConstant(" << value << ")\n";
}

void Variable::dump(int depth)
{
    std::cout << std::string(depth, '\t') << "Variable(" << symbol->name << ")\n";
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
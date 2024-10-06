#include <iostream>
#include <string>
#include "Operator.hpp"
#include "SyntaxTree.hpp"

/********************************************************************************/
/*                             Type Check                                       */
/********************************************************************************/

static _GLIBCXX_NORETURN void error(std::string msg) // TODO take a span to show line numbers
{
    std::cerr << "Type Error: " << msg << "\n";
    throw std::exception();
}

// TODO add type checking
void CompoundStatement::typecheck(TypecheckContext& context)
{
    if (statements.empty() || dynamic_cast<Return*>(statements.back().get()) == nullptr)
    {
        // if the last statement of the compound add a return.
        statements.push_back(std::make_shared<Return>(symbol_table));
    }

    for (auto& statement : statements)
    {
        statement->typecheck(context);
    }
}

void VariableDeclaration::typecheck(TypecheckContext& context)
{
}

void Return::typecheck(TypecheckContext& context)
{
    if (expr)
    {
        expr->typecheck(context);
        if (*expr->type != *context.func_def->function->type->ret_type)
        {
            // TODO implicitly cast type if possible
            error("return type does not match function");
        }
    }
    else if (context.func_def->function->type->ret_type->type != TypeType::Void)
    {
        // expr->type->dump();
        // std::cerr << "\n"; 
        // context.func_def->function->type->ret_type->dump();
        // std::cerr << "\n"; 

        error("return type does not match function");
    }
}

void Variable::typecheck(TypecheckContext& context)
{
    type = symbol->type;
}

void FunctionCall::typecheck(TypecheckContext& context)
{
    // TODO check types of args
    if (args.size() != function->type->param_types.size())
    {
        error("number of arguments do not match the function");
    }

    for (auto i = 0; i < args.size(); i++)
    {
        args[i]->typecheck(context);
        if (*args[i]->type != *function->type->param_types[i])
        {
            // TODO implicitly cast type if possible
            error("argument type does not match function");
        }
    }

    type = function->type->ret_type;
}

void BinaryOperation::typecheck(TypecheckContext& context)
{
    lhs->typecheck(context);
    rhs->typecheck(context);
    if (*lhs->type != *rhs->type)
    {
        // TODO implicitly cast type if possible
            error("type mismatch between operands of binary operation");
    }
    else
    {
        type = lhs->type;
    }
}

void UnaryOperation::typecheck(TypecheckContext& context)
{
    expr->typecheck(context);
    switch (op)
    {
        case UnOp::Negation:
            type = expr->type;
            break;
        case UnOp::Deref:
        case UnOp::AddrOf:
        case UnOp::Pre_PlusPlus:
        case UnOp::Post_PlusPlus:
        case UnOp::Pre_MinusMinus:
        case UnOp::Post_MinusMinus:
            std::cerr << "Unimplemented\n";
            throw std::exception(); // TODO handle later
    }
}

void IntegerConstant::typecheck(TypecheckContext& context)
{
    type = std::make_shared<Type>(TypeType::Int);
}

void CharConstant::typecheck(TypecheckContext& context)
{
    // TODO later
}

void FunctionDef::typecheck(TypecheckContext& context)
{
    body->typecheck(context);
}

void Program::typecheck(TypecheckContext& context)
{
    for (auto& function : functions)
    {
        context.func_def = function;
        function->typecheck(context);
    }
}

void Program::typecheck()
{
    TypecheckContext context;
    typecheck(context);
}

/********************************************************************************/
/*                                   Dump                                       */
/********************************************************************************/

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
    std::cout << "args = ";
    if (args.empty())
    {
        std::cout << "(";
    }
    else 
    {
        std::cout << "(\n";
        for (auto i = 0; i < args.size(); i++)
        {
            indent(depth + 1);
            args[i]->dump(depth + 1);
            if (i < args.size() - 1)
            {
                std::cout << ",\n";
            }
        }
    }
    

    std::cout << ")";
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
    for (auto i = 0; i < params.size(); i++)
    {
        std::cout << params[i]->name;
        if (i < params.size() - 1)
        {
            std::cout << ", ";
        }
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
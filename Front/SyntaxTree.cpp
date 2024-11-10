#include <iostream>
#include <string>
#include "Operator.hpp"
#include "Error.hpp"
#include "SyntaxTree.hpp"

/********************************************************************************/
/*                             Type Check                                       */
/********************************************************************************/

/**
 * Typechecks the compound statement.
 */
void CompoundStatement::typecheck(TypecheckContext& context)
{
    for (auto& statement : statements)
    {
        statement->typecheck(context);
    }
}

/**
 * Typechecks the variable declaration.
 */
void VariableDeclaration::typecheck(TypecheckContext& context)
{
    for (auto expr : expressions)
    {
        expr->typecheck(context); // TODO need to add some additional type checking
    }
}

/**
 * Typechecks the return statement.
 */
void Return::typecheck(TypecheckContext& context)
{
    if (expr != nullptr)
    {
        expr->typecheck(context);
        if (*expr->type != *context.func_def->function->type->ret_type)
        {
            // TODO implicitly cast type if possible
            throw TypeError(span, "return type does not match function");
        }
    }
    else if (context.func_def->function->type->ret_type->type != TypeType::Void)
    {
        throw TypeError(span, "return type does not match function");
    }
}

/**
 * Typechecks the if statement.
 */
void IfStatement::typecheck(TypecheckContext& context)
{
    guard->typecheck(context);
    then_stmt->typecheck(context);
    if (else_stmt != nullptr) 
    {
        else_stmt->typecheck(context);
    }
}

/**
 * Typechecks the while loop.
 */
void WhileLoop::typecheck(TypecheckContext& context)
{
    guard->typecheck(context);
    body->typecheck(context);
}

/**
 * Typechecks the while loop.
 */
void ForLoop::typecheck(TypecheckContext& context)
{
    if (init != nullptr)
    {
        init->typecheck(context);
    }
    
    if (guard != nullptr)
    {
        guard->typecheck(context);
    }
    
    if (update != nullptr)
    {
        update->typecheck(context);
    }

    body->typecheck(context);
}

/**
 * Typechecks the variable.
 */
void Variable::typecheck(TypecheckContext& context)
{
    type = symbol->type;
}

/**
 * Typechecks the function call.
 */
void FunctionCall::typecheck(TypecheckContext& context)
{
    // TODO check types of args
    if (args.size() != function->type->param_types.size())
    {
        throw TypeError(span, "number of arguments do not match the function");
    }

    for (auto i = 0; i < args.size(); i++)
    {
        args[i]->typecheck(context);
        if (*args[i]->type != *function->type->param_types[i])
        {
            // TODO implicitly cast type if possible
            throw TypeError(span, "argument type does not match function");
        }
    }

    type = function->type->ret_type;
}

/**
 * Typechecks the binary operation.
 */
void BinaryOperation::typecheck(TypecheckContext& context)
{
    lhs->typecheck(context);
    rhs->typecheck(context);
    if (*lhs->type != *rhs->type)
    {
        // TODO implicitly cast type if possible
            throw TypeError(span, "type mismatch between operands of binary operation");
    }
    else
    {
        type = lhs->type;
    }
}

/**
 * Typechecks the unary operation.
 */
void UnaryOperation::typecheck(TypecheckContext& context)
{
    expr->typecheck(context);
    switch (op)
    {
        case UnOp::Negation:
        case UnOp::PlusPlus:
        case UnOp::MinusMinus:
            type = expr->type;
            break;
        case UnOp::Not:
            type = std::make_shared<Type>(TypeType::Int);
            break;
        case UnOp::Deref:
        case UnOp::AddrOf:
            assert(false && "unimplemented");    
    }
}

/**
 * Typechecks the array index.
 */
void ArrayIndex::typecheck(TypecheckContext& context)
{
    array->typecheck(context);
    if (array->type->type != TypeType::Array)
    {
        throw TypeError(span, "type cannot be indexed like an array");
    }

    index->typecheck(context);
    if (index->type->type != TypeType::Int || index->type->type != TypeType::Char)
    {
        throw TypeError(span, "invalid type for array index");
    }

    type = array->type->elem_type;
}

/**
 * Typechecks the integer constant.
 */
void IntegerConstant::typecheck(TypecheckContext& context)
{
    type = std::make_shared<Type>(TypeType::Int);
}

/**
 * Typechecks the char constant.
 */
void CharConstant::typecheck(TypecheckContext& context)
{
    // TODO later
}

/**
 * Typechecks the function.
 */
void FunctionDef::typecheck(TypecheckContext& context)
{
    if (!is_proto())
    {
        body->typecheck(context);
    }
    else
    {
        if (!function->type->is_extern && !function->type->is_defined)
        {
            throw TypeError(span, "function prototype was never defined");
        }
    }
}

/**
 * Typechecks the program.
 */
void Program::typecheck(TypecheckContext& context)
{
    for (auto& function : functions)
    {
        context.func_def = function;
        function->typecheck(context);
    }
}

/**
 * Typechecks the program.
 */
void Program::typecheck()
{
    TypecheckContext context;
    typecheck(context);
}

/********************************************************************************/
/*                                   Dump                                       */
/********************************************************************************/

/**
 * Outputs an indentation.
 */
static void indent(int depth)
{
    std::cout << std::string(depth, '\t');
}

/**
 * Dumps the AST node.
 */
void IntegerConstant::dump(int depth)
{
    std::cout << "IntegerConstant(" << value << ")";
}

/**
 * Dumps the AST node.
 */
void CharConstant::dump(int depth)
{
    std::cout << "CharConstant('" << value << "')";
}

/**
 * Dumps the AST node.
 */
void Variable::dump(int depth)
{
    std::cout << "Variable(" << symbol->get_name() << ")";
}

/**
 * Dumps the AST node.
 */
void FunctionCall::dump(int depth)
{
    std::cout << "FunctionCall(\n";
    indent(depth);
    std::cout << "name = " << function->get_name() << ",\n";
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

/**
 * Dumps the AST node.
 */
void BinaryOperation::dump(int depth)
{
    std::cout << "BinaryOperation(\n";
    indent(depth);
    std::cout << "op = '" << to_string(op) << "',\n";
    indent(depth);
    lhs->dump(depth + 1);
    std::cout << ",\n";
    indent(depth);
    rhs->dump(depth + 1);
    std::cout << ")";
}

/**
 * Dumps the AST node.
 */
void UnaryOperation::dump(int depth)
{
    std::cout << "UnaryOperation(\n";
    indent(depth);
    std::cout << "op = " << to_string(op) << ",\n";
    indent(depth);
    std::cout << "expr = ";
    expr->dump(depth);
    std::cout << "\n";
    indent(depth);
    std::cout << "is_postfix = " << is_postfix;
    std::cout << ")";
}

/**
 * Dumps the AST node.
 */
void ArrayIndex::dump(int depth)
{
    std::cout << "ArrayIndex(\n";
    indent(depth);
    std::cout << "array = ";
    array->dump(depth);
    indent(depth);
    std::cout << "index = ";
    index->dump(depth);
    std::cout << ")";
}

/**
 * Dumps the AST node.
 */
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

/**
 * Dumps the AST node.
 */
void VariableDeclaration::dump(int depth)
{
    std::cout << "VariableDeclaration(\n";
    indent(depth);
    std::cout << "type = ";
    type->dump();

    std::cout << "\n";
    for (auto i = 0; i < expressions.size(); i++)
    {
        indent(depth);
        expressions[i]->dump(depth + 1);
        if (i < expressions.size() - 1)
        {
            std::cout << ",\n";
        }
    }

    std::cout << ")";
}

/**
 * Dumps the AST node.
 */
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

/**
 * Dumps the AST node.
 */
void IfStatement::dump(int depth)
{
    std::cout << "IfStatement(\n";
    indent(depth);
    std::cout << "guard = ";
    guard->dump(depth + 1);

    std::cout << "\n";
    indent(depth);
    std::cout << "then = ";
    then_stmt->dump(depth + 1);

    std::cout << "\n";
    indent(depth);
    std::cout << "else = ";
    if (else_stmt != nullptr)
    {
        else_stmt->dump(depth + 1);
    }
    else
    {
        std::cout << "null";
    }

    std::cout << ")";
}

/**
 * Dumps the AST node.
 */
void WhileLoop::dump(int depth)
{
    std::cout << "WhileLoop(\n";
    indent(depth);
    std::cout << "guard = ";
    guard->dump(depth + 1);

    std::cout << "\n";
    indent(depth);
    std::cout << "body = ";
    body->dump(depth + 1);

    std::cout << ")";
}

/**
 * Dumps the AST node.
 */
void ForLoop::dump(int depth)
{
    std::cout << "ForLoop(\n";

    indent(depth);
    std::cout << "init = ";
    if (init != nullptr)
    {
        init->dump(depth + 1);
    }
    else
    {
        std::cout << "nullptr";
    }

    std::cout << "\n";
    indent(depth);
    std::cout << "guard = ";
    if (guard != nullptr)
    {
        guard->dump(depth + 1);
    }
    else
    {
        std::cout << "nullptr";
    }

    std::cout << "\n";
    indent(depth);
    std::cout << "update = ";
    if (update != nullptr)
    {
        update->dump(depth + 1);
    }
    else
    {
        std::cout << "nullptr";
    }

    std::cout << "\n";
    indent(depth);
    std::cout << "body = ";
    body->dump(depth + 1);

    std::cout << ")";
}

/**
 * Dumps the AST node.
 */
void FunctionDef::dump(int depth)
{
    if (!is_proto())
    {
        std::cout << "FunctionDef(\n";
    }
    else
    {
        std::cout << "FunctionProto(\n";
    }

    indent(depth);
    std::cout << "name = " << function->get_name() << ",\n";
    
    indent(depth);
    std::cout << "params = (";
    for (auto i = 0; i < params.size(); i++)
    {
        std::cout << params[i]->get_name();
        if (i < params.size() - 1)
        {
            std::cout << ", ";
        }
    }

    std::cout << "),\n";
    indent(depth);
    std::cout << "is_extern = " << function->type->is_extern << "),\n";
    indent(depth);
    std::cout << "is_defined = " << function->type->is_defined << ")";

    if (!is_proto())
    {
        std::cout << ",\n";
        indent(depth);
        std::cout << "body = ";

        body->dump(depth + 1);
        std::cout << ")";
    }
    else
    {
        std::cout << ")";
    }
}

/**
 * Dumps the AST node.
 */
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

/**
 * Dumps the IR for the program.
 */
void Program::ir_dump() 
{
    for (auto f : functions)
    {
        std::cout << f->function->get_name() << "\n";
        f->ir_list.dump();
        std::cout << "\n";
    }
}

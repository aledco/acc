#include <iostream>
#include "SyntaxTree.hpp"
#include "Quad.hpp"

void CompoundStatement::ir_codegen()
{
    for (auto& statement : statements)
    {
        statement->ir_codegen();
        ir_list = QuadList::concat(ir_list, statement->ir_list);
    }
}

void VariableDeclaration::ir_codegen()
{
}

void Return::ir_codegen()
{
    if (expr)
    {
        expr->ir_codegen();
        auto inst = Quad::MakeReturnOp(expr->place);
        ir_list = QuadList::append(expr->ir_list, inst);
    }
    else
    {
        auto inst = Quad::MakeReturnOp();
        ir_list = QuadList(inst, inst);
    }
}

void Variable::ir_codegen()
{
    place = Operand::MakeVariableOperand(symbol);
}

void FunctionCall::ir_codegen()
{
    for (auto &arg : args)
    {
        arg->ir_codegen();
        ir_list = QuadList::concat(ir_list, arg->ir_list);
        auto inst = Quad::MakeParamOp(arg->place);
        ir_list = QuadList::append(ir_list, inst);
    }

    place = Operand::MakeVariableOperand(symbol_table->new_temp(type));
    auto call_inst = Quad::MakeCallOp(Operand::MakeVariableOperand(function), Operand::MakeIntConstOperand(args.size()), place); 
    ir_list = QuadList::append(ir_list, call_inst);
}

void BinaryOperation::ir_codegen()
{
    if (op == BinOp::Assign)
    {
        // this is a special case, need to get the lvalue of the lhs
        lhs->ir_codegen_lval();
        rhs->ir_codegen();
        ir_list = QuadList::concat(lhs->ir_list, rhs->ir_list);
        auto copy = Quad::MakeUnOp(QuadOp::Copy, rhs->place, lhs->location);
        ir_list = QuadList::append(ir_list, copy);
        place = rhs->place;
        return;
    }

    lhs->ir_codegen();
    rhs->ir_codegen();
    ir_list = QuadList::concat(lhs->ir_list, rhs->ir_list);
    place = Operand::MakeVariableOperand(symbol_table->new_temp(type));
    QuadOp quad_op;
    switch (op)
    {
        case BinOp::Plus:
            quad_op = QuadOp::Add;
            break;
        case BinOp::Minus:
            quad_op = QuadOp::Sub;
            break;
        case BinOp::Times:
            quad_op = QuadOp::Mul;
            break;
        case BinOp::Divide:
            quad_op = QuadOp::Div;
            break;
        case BinOp::Modulo:
            quad_op = QuadOp::Mod;
            break;
        default:
            std::cerr << "Not implemented\n";
            throw std::exception();
    }

    auto inst = Quad::MakeBinOp(quad_op, lhs->place, rhs->place, place);
    ir_list = QuadList::append(ir_list, inst);
}

void UnaryOperation::ir_codegen()
{
    expr->ir_codegen();
    place = Operand::MakeVariableOperand(symbol_table->new_temp(type));
    switch (op)  
    {
        case UnOp::Negation:
        {
            auto inst = Quad::MakeUnOp(QuadOp::Neg, expr->place, place);
            ir_list = QuadList::append(expr->ir_list, inst);
            break;
        }
        case UnOp::Deref:
        case UnOp::AddrOf:
        case UnOp::Pre_PlusPlus:
        case UnOp::Post_PlusPlus:
        case UnOp::Pre_MinusMinus:
        case UnOp::Post_MinusMinus:
            std::cerr << "Not implemented\n";
            throw std::exception();
    }
}

void IntegerConstant::ir_codegen()
{
    place = Operand::MakeVariableOperand(symbol_table->new_temp(type));
    auto inst = Quad::MakeUnOp(QuadOp::Copy, Operand::MakeIntConstOperand(value), place);
    ir_list = QuadList(inst, inst);
}

void CharConstant::ir_codegen()
{
    // TODO later
}

void FunctionDef::ir_codegen()
{
    if (is_proto())
    {
        return;
    }

    auto enter = Quad::MakeEnterOp(Operand::MakeVariableOperand(function));
    ir_list = QuadList(enter, enter);

    body->ir_codegen();
    ir_list = QuadList::concat(ir_list, body->ir_list);

    cfg = ConstructCFG(ir_list);
}

void Program::ir_codegen()
{
    for (auto& function : functions)
    {
        function->ir_codegen();
    }
}

void Expression::ir_codegen_lval()
{
    std::cerr << "Error: cannot take lvalue of expression\n"; // TODO line numbers
    throw std::exception();
}

void Variable::ir_codegen_lval()
{
    location = Operand::MakeVariableOperand(symbol);
}

void UnaryOperation::ir_codegen_lval()
{
    // TODO
}
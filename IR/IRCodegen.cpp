#include <iostream>
#include "SyntaxTree.hpp"
#include "Quad.hpp"

void CompoundStatement::ir_codegen()
{

}

void VariableDeclaration::ir_codegen()
{

}

void Return::ir_codegen()
{

}

void Variable::ir_codegen()
{

}

void FunctionCall::ir_codegen()
{

}

void BinaryOperation::ir_codegen()
{
    if (op == BinOp::Assign)
    {
        // this is a special case, need to get the lvalue of the lhs
        
        // TOOD implement
        return;
    }

    lhs->ir_codegen();
    rhs->ir_codegen();
    ir_list = QuadList::concat(lhs->ir_list, rhs->ir_list);
    place = symbol_table->new_temp(type);
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

    auto lhs_operand = Operand::MakeVariableOperand(lhs->place);
    auto rhs_operand = Operand::MakeVariableOperand(rhs->place);
    auto inst = Quad::MakeBinOp(quad_op, lhs_operand, rhs_operand, Operand::MakeVariableOperand(place));
    ir_list = QuadList::append(ir_list, inst);
}

void UnaryOperation::ir_codegen()
{
    expr->ir_codegen();
    place = symbol_table->new_temp(type);
    switch (op)  
    {
        case UnOp::Negation:
        {
            auto inst = Quad::MakeUnOp(QuadOp::Neg, Operand::MakeVariableOperand(expr->place), Operand::MakeVariableOperand(place));
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
    place = symbol_table->new_temp(type);
    auto inst = Quad::MakeUnOp(QuadOp::Copy, Operand::MakeIntConstOperand(value), Operand::MakeVariableOperand(place));
    ir_list = QuadList(inst, inst);
}

void CharConstant::ir_codegen()
{
    // TODO later
}

void FunctionDef::ir_codegen()
{
    auto enter = Quad::MakeEnterOp(Operand::MakeVariableOperand(function));
    ir_list = QuadList(enter, enter);

    body->ir_codegen();
    ir_list = QuadList::concat(ir_list, body->ir_list);
}

void Program::ir_codegen() // TODO dont need to return quad list
{
    for (auto& function : functions)
    {
        function->ir_codegen();
        ir_list = QuadList::concat(ir_list, function->ir_list);
    }
}

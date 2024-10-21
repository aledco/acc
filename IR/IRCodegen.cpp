#include <iostream>
#include "SyntaxTree.hpp"
#include "Error.hpp"
#include "Quad.hpp"

/**
 * Generates IR for the AST node.
 */
void CompoundStatement::ir_codegen()
{
    for (auto& statement : statements)
    {
        statement->ir_codegen();
        ir_list = QuadList::concat(ir_list, statement->ir_list);
    }
}

/**
 * Generates IR for the AST node.
 */
void VariableDeclaration::ir_codegen()
{
}

/**
 * Generates IR for the AST node.
 */
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

/**
 * Generates IR for the AST node.
 */
void IfStatement::ir_codegen()
{
    auto then_label = Operand::MakeLabelOperand();
    auto else_label = Operand::MakeLabelOperand();
    auto end_label = Operand::MakeLabelOperand();
    guard->ir_codegen_bool(then_label, else_label);
    ir_list = QuadList::append(guard->ir_list, Quad::MakeLabelOp(then_label));
    
    then_stmt->ir_codegen();
    ir_list = QuadList::concat(ir_list, then_stmt->ir_list);

    ir_list = QuadList::append(ir_list, Quad::MakeGotoOp(end_label));
    ir_list = QuadList::append(ir_list, Quad::MakeLabelOp(else_label));

    if (else_stmt != nullptr)
    {
        else_stmt->ir_codegen();
        ir_list = QuadList::concat(ir_list, else_stmt->ir_list);
    }
    
    ir_list = QuadList::append(ir_list, Quad::MakeLabelOp(end_label));
}

/**
 * Generates IR for the AST node.
 */
void WhileLoop::ir_codegen()
{
    auto top_label = Operand::MakeLabelOperand();
    auto eval_label = Operand::MakeLabelOperand();
    auto end_label = Operand::MakeLabelOperand();

    guard->ir_codegen_bool(top_label, end_label);
    body->ir_codegen();

    ir_list = QuadList::append(ir_list, Quad::MakeGotoOp(eval_label));
    ir_list = QuadList::append(ir_list, Quad::MakeLabelOp(top_label));
    ir_list = QuadList::concat(ir_list, body->ir_list);
    ir_list = QuadList::append(ir_list, Quad::MakeLabelOp(eval_label));
    ir_list = QuadList::concat(ir_list, guard->ir_list);
    ir_list = QuadList::append(ir_list, Quad::MakeLabelOp(end_label));
}

/**
 * Generates IR for the AST node.
 */
void Variable::ir_codegen()
{
    place = Operand::MakeVariableOperand(symbol);
}

/**
 * Generates IR for the AST node.
 */
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

/**
 * Generates IR for the AST node.
 */
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
            assert(false && "unimplemented");
    }

    auto inst = Quad::MakeBinOp(quad_op, lhs->place, rhs->place, place);
    ir_list = QuadList::append(ir_list, inst);
}

/**
 * Generates IR for the AST node.
 */
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
        case UnOp::Not:
            assert(false && "should be processed in codegen_bool");
        case UnOp::Deref:
        case UnOp::AddrOf:
        case UnOp::PlusPlus:
        case UnOp::MinusMinus:
            assert(false && "unimplemented");
    }
}

/**
 * Generates IR for the AST node.
 */
void IntegerConstant::ir_codegen()
{
    place = Operand::MakeVariableOperand(symbol_table->new_temp(type));
    auto inst = Quad::MakeUnOp(QuadOp::Copy, Operand::MakeIntConstOperand(value), place);
    ir_list = QuadList(inst, inst);
}

/**
 * Generates IR for the AST node.
 */
void CharConstant::ir_codegen()
{
    // TODO later
}

/**
 * Generates IR for the AST node.
 */
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

/**
 * Generates IR for the AST node.
 */
void Program::ir_codegen()
{
    for (auto& function : functions)
    {
        function->ir_codegen();
    }
}

/**
 * Generates IR for the AST node.
 */
void Expression::ir_codegen_lval()
{
    throw Error(span, "Error", "cannot take lvalue of expression");
}

/**
 * Generates IR for the AST node.
 */
void Variable::ir_codegen_lval()
{
    location = Operand::MakeVariableOperand(symbol);
}

/**
 * Generates IR for the AST node.
 */
void UnaryOperation::ir_codegen_lval()
{
    // TODO
}

/**
 * Creates the jump instructions for the if statement.
 */
static QuadList codegen_if(QuadOp op, std::shared_ptr<Operand> rhs, std::shared_ptr<Operand> lhs, 
    std::shared_ptr<Operand> true_label, std::shared_ptr<Operand> false_label)
{
    auto if_inst = Quad::MakeIfOp(op, rhs, lhs, true_label);
    auto goto_inst = Quad::MakeGotoOp(false_label);
    if_inst->next = goto_inst;
    return QuadList(if_inst, goto_inst);
}

/**
 * Generates IR for the AST node.
 */
void Expression::ir_codegen_bool(std::shared_ptr<Operand> true_label, std::shared_ptr<Operand> false_label)
{
    ir_codegen();
    auto if_code = codegen_if(QuadOp::IfNeq, place, Operand::MakeIntConstOperand(0), true_label, false_label);
    ir_list = QuadList::concat(ir_list, if_code);
}

/**
 * Generates IR for the AST node.
 */
void BinaryOperation::ir_codegen_bool(std::shared_ptr<Operand> true_label, std::shared_ptr<Operand> false_label)
{
    auto codegen_comparison = [=](QuadOp op) 
    { 
        lhs->ir_codegen();
        rhs->ir_codegen();
        auto if_code = codegen_if(op, lhs->place, rhs->place, true_label, false_label);
        ir_list = QuadList::concat(lhs->ir_list, rhs->ir_list);
        ir_list = QuadList::concat(ir_list, if_code);
    };

    auto codegen_and = [=]() 
    {
        auto label = Operand::MakeLabelOperand();
        lhs->ir_codegen_bool(label, false_label);
        rhs->ir_codegen_bool(true_label, false_label);
        ir_list = QuadList::append(lhs->ir_list, Quad::MakeLabelOp(label));
        ir_list = QuadList::concat(ir_list, rhs->ir_list);
    };

    auto codegen_or = [=]() 
    {
        auto label = Operand::MakeLabelOperand();
        lhs->ir_codegen_bool(true_label, label);
        rhs->ir_codegen_bool(true_label, false_label);
        ir_list = QuadList::append(lhs->ir_list, Quad::MakeLabelOp(label));
        ir_list = QuadList::concat(ir_list, rhs->ir_list);
    };

    switch (op)
    {
        case BinOp::Equal:
            codegen_comparison(QuadOp::IfEq);
            break;
        case BinOp::NotEqual:
            codegen_comparison(QuadOp::IfNeq);
            break;
        case BinOp::LessThan:
            codegen_comparison(QuadOp::IfLt);
            break;
        case BinOp::LessThanEqual:
            codegen_comparison(QuadOp::IfLeq);
            break;
        case BinOp::GreaterThan:
            codegen_comparison(QuadOp::IfGt);
            break;
        case BinOp::GreaterThanEqual:
            codegen_comparison(QuadOp::IfGeq);
            break;
        case BinOp::And:
            codegen_and();
            break;
        case BinOp::Or:
            codegen_or();
            break;
        default:
            Expression::ir_codegen_bool(true_label, false_label);
            break;
    }
}

/**
 * Generates IR for the AST node.
 */
void UnaryOperation::ir_codegen_bool(std::shared_ptr<Operand> true_label, std::shared_ptr<Operand> false_label)
{
    auto codegen_not = [=]() 
    {
        expr->ir_codegen_bool(false_label, true_label);
        ir_list = expr->ir_list;
    };

    switch (op)
    {
        case UnOp::Not:
            codegen_not();
            break;
        default:
            Expression::ir_codegen_bool(true_label, false_label);
            break;     
    }
}
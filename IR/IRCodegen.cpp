#include "SyntaxTree.hpp"
#include "Quad.hpp"

QuadList CompoundStatement::ir_codegen()
{
    return QuadList();
}

QuadList VariableDeclaration::ir_codegen()
{
    return QuadList();
}

QuadList Return::ir_codegen()
{
    return QuadList();
}

QuadList Variable::ir_codegen()
{
    return QuadList();
}

QuadList FunctionCall::ir_codegen()
{
    return QuadList();
}

QuadList BinaryOperation::ir_codegen()
{
    return QuadList();
}

QuadList UnaryOperation::ir_codegen()
{
    return QuadList();
}

QuadList IntegerConstant::ir_codegen()
{
    // TODO make temp
    std::shared_ptr<Operand> tmp;

    auto inst = Quad::MakeUnOp(QuadOp::Copy, tmp, Operand::MakeIntConstOperand(value));
    return QuadList(inst, inst);
}

QuadList CharConstant::ir_codegen()
{
    return QuadList(); // TODO
}

QuadList FunctionDef::ir_codegen()
{
    auto enter = Quad::MakeEnterOp(Operand::MakeVariableOperand(function));
    ir_list = QuadList(enter, enter);

    body->ir_codegen();
    ir_list = QuadList::concat(ir_list, body->ir_list);
    return ir_list;
}

QuadList Program::ir_codegen() // TODO dont need to return quad list
{
    for (auto& function : functions)
    {
        function->ir_codegen();
        ir_list = QuadList::concat(ir_list, function->ir_list);
    }

    return ir_list;
}

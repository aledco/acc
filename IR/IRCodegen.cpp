#include "SyntaxTree.hpp"
#include "Quad.hpp"

QuadList IntegerConstant::ir_codegen()
{
    // TODO make temp
    std::shared_ptr<Operand> tmp;

    auto inst = Quad::MakeUnOp(QuadOp::Copy, tmp, Operand::MakeIntConstOperand(value));
    return QuadList(inst, inst);
}

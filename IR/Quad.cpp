#include <cassert>
#include "Quad.hpp"

std::shared_ptr<Operand> Operand::MakeIntConstOperand(long val)
{
    return std::make_shared<Operand>(OperandType::IntConst, val);
}

std::shared_ptr<Operand> Operand::MakeStrConstOperand(std::string val)
{
    return std::make_shared<Operand>(OperandType::StrConst, val);
}

std::shared_ptr<Operand> Operand::MakeVariableOperand(std::shared_ptr<Symbol> val)
{
    return std::make_shared<Operand>(OperandType::Variable, val);
}

std::shared_ptr<Operand> Operand::MakeLabelOperand(std::string val)
{
    return std::make_shared<Operand>(OperandType::Label, val);
}

std::shared_ptr<Quad> Quad::MakeBinOp(QuadOp op, std::shared_ptr<Operand> arg1, std::shared_ptr<Operand> arg2, std::shared_ptr<Operand> res)
{
    assert(op == QuadOp::Add || op == QuadOp::Sub || op == QuadOp::Mul || op == QuadOp::Div || op == QuadOp::Mod);
    return std::make_shared<Quad>(op, arg1, arg2, res);
}

std::shared_ptr<Quad> Quad::MakeUnOp(QuadOp op, std::shared_ptr<Operand> arg1, std::shared_ptr<Operand> res)
{
    assert(op == QuadOp::Neg || op == QuadOp::Deref || op == QuadOp::Addr || op == QuadOp::Copy);
    return std::make_shared<Quad>(op, arg1, nullptr, res);
}

std::shared_ptr<Quad> Quad::MakeRIndexOp(std::shared_ptr<Operand> array, std::shared_ptr<Operand> index, std::shared_ptr<Operand> res)
{
    return std::make_shared<Quad>(QuadOp::RIndex, array, index, res);
}

std::shared_ptr<Quad> Quad::MakeLIndexOp(std::shared_ptr<Operand> arg1, std::shared_ptr<Operand> index, std::shared_ptr<Operand> array)
{
    return std::make_shared<Quad>(QuadOp::LIndex, arg1, index, array);
}

std::shared_ptr<Quad> Quad::MakeLabelOp(std::shared_ptr<Operand> label)
{
    return std::make_shared<Quad>(QuadOp::Label, label, nullptr, nullptr);
}

std::shared_ptr<Quad> Quad::MakeGotoOp(std::shared_ptr<Operand> label)
{
    return std::make_shared<Quad>(QuadOp::Goto, label, nullptr, nullptr);
}

std::shared_ptr<Quad> Quad::MakeIfOp(QuadOp op, std::shared_ptr<Operand> arg1, std::shared_ptr<Operand> arg2, std::shared_ptr<Operand> res)
{
    assert(op == QuadOp::IfEq || op == QuadOp::IfNeq || op == QuadOp::IfLt || op == QuadOp::IfLeq || op == QuadOp::IfGt || op == QuadOp::IfGeq);
    return std::make_shared<Quad>(op, arg1, arg2, res);
}

std::shared_ptr<Quad> Quad::MakeEnterOp(std::shared_ptr<Operand> func)
{
    return std::make_shared<Quad>(QuadOp::Enter, func, nullptr, nullptr);
}

std::shared_ptr<Quad> Quad::MakeReturnOp(std::shared_ptr<Operand> arg1)
{
    return std::make_shared<Quad>(QuadOp::Return, arg1, nullptr, nullptr);
}

std::shared_ptr<Quad> Quad::MakeReturnOp()
{
    return std::make_shared<Quad>(QuadOp::Return, nullptr, nullptr, nullptr);
}

std::shared_ptr<Quad> Quad::MakeParamOp(std::shared_ptr<Operand> arg1)
{
    return std::make_shared<Quad>(QuadOp::Param, arg1, nullptr, nullptr);
}

std::shared_ptr<Quad> Quad::MakeCallOp(std::shared_ptr<Operand> func, std::shared_ptr<Operand> nargs)
{
    return std::make_shared<Quad>(QuadOp::Call, func, nargs, nullptr);
}

std::shared_ptr<Quad> Quad::MakeRetrieveOp(std::shared_ptr<Operand> res)
{
    return std::make_shared<Quad>(QuadOp::Retrieve, nullptr, nullptr, res);
}

/********************************************************************************/
/*                                    QuadList                                  */
/********************************************************************************/

void QuadList::push_back(std::shared_ptr<Quad> quad)
{
    if (empty())
    {
        head = tail = quad;
    }
    else
    {
        tail->next = quad;
        tail = quad;
    }
}

QuadList QuadList::concat(QuadList& list1, QuadList& list2)
{
    if (list1.empty() && list2.empty())
    {
        return QuadList();
    }
    else if (list1.empty())
    {
        return QuadList(list2.head, list2.tail);
    }
    else if (list2.empty())
    {
        return QuadList(list1.head, list1.tail);
    }
    else
    {
        list1.tail->next = list2.head;
        return QuadList(list1.head, list2.tail);
    }
}
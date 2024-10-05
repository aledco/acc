#include <iostream>
#include <cassert>
#include "Quad.hpp"

/********************************************************************************/
/*                           Operand Factories                                  */
/********************************************************************************/

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


/********************************************************************************/
/*                              Quad Factories                                  */
/********************************************************************************/

std::shared_ptr<Quad> Quad::MakeGlobalOp(std::shared_ptr<Operand> arg1)
{
    assert(arg1 != nullptr && arg1->type == OperandType::Variable);
    return std::make_shared<Quad>(QuadOp::Global, arg1, nullptr, nullptr);
}

std::shared_ptr<Quad> Quad::MakeStringOp(std::shared_ptr<Operand> arg1)
{
    assert(arg1 != nullptr && arg1->type == OperandType::StrConst);
    return std::make_shared<Quad>(QuadOp::String, arg1, nullptr, nullptr);
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

QuadList QuadList::append(QuadList& list, std::shared_ptr<Quad> quad)
{
    auto list2 = QuadList(quad, quad);
    return QuadList::concat(list, list2);
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

/********************************************************************************/
/*                                   Dump                                       */
/********************************************************************************/

void Operand::dump()
{
    switch (type)
    {
        case OperandType::IntConst:
            std::cerr << iconst;
            break;
        case OperandType::StrConst:
        case OperandType::Label:
            std::cerr << strconst;
            break;
        case OperandType::Variable:
            std::cerr << symbol->name;
            break;
    }
}

void Quad::dump()
{
    switch (op)
    {
        case QuadOp::Global:
            std::cerr << "global ";
            res->dump();
            std::cerr << " ";
            res->symbol->type->dump();
            std::cerr << "\n";
            break;
        case QuadOp::String:
            std::cerr << "string ";
            res->dump();
            std::cerr << "\n";
            break;
        case QuadOp::Add:
            res->dump();
            std::cerr << " = ";
            arg1->dump();
            std::cerr << " + ";
            arg2->dump();
            std::cerr << "\n";
            break;
        case QuadOp::Sub:
            res->dump();
            std::cerr << " = ";
            arg1->dump();
            std::cerr << " - ";
            arg2->dump();
            std::cerr << "\n";
            break;
        case QuadOp::Mul:
            res->dump();
            std::cerr << " = ";
            arg1->dump();
            std::cerr << " * ";
            arg2->dump();
            std::cerr << "\n";
            break;
        case QuadOp::Div:
            res->dump();
            std::cerr << " = ";
            arg1->dump();
            std::cerr << " / ";
            arg2->dump();
            std::cerr << "\n";
            break;
        case QuadOp::Mod:
            res->dump();
            std::cerr << " = ";
            arg1->dump();
            std::cerr << " % ";
            arg2->dump();
            std::cerr << "\n";
            break;
        case QuadOp::Neg:
            res->dump();
            std::cerr << " = -";
            arg1->dump();
            std::cerr << "\n";
            break;
        case QuadOp::Deref:
            res->dump();
            std::cerr << " = *";
            arg1->dump();
            std::cerr << "\n";
            break;
        case QuadOp::Addr:
            res->dump();
            std::cerr << " = &";
            arg1->dump();
            std::cerr << "\n";
            break;
        case QuadOp::Copy:
            res->dump();
            std::cerr << " = ";
            arg1->dump();
            std::cerr << "\n";
            break;
        case QuadOp::LIndex:
            res->dump();
            std::cerr << "[";
            arg2->dump();
            std::cerr << "] = ";
            arg1->dump();
            std::cerr << "\n";
            break;
        case QuadOp::RIndex:
            res->dump();
            std::cerr << " = ";
            arg1->dump();
            std::cerr << "[";
            arg2->dump();
            std::cerr << "]\n";
            break;
        case QuadOp::Label:
            std::cerr << "label ";
            arg1->dump();
            std::cerr << "\n";
            break;
        case QuadOp::Goto:
            std::cerr << "goto ";
            arg1->dump();
            std::cerr << "\n";
            break;
        case QuadOp::IfEq:
            std::cerr << "if (";
            arg1->dump();
            std::cerr << " == ";
            arg2->dump();
            std::cerr << ") goto ";
            res->dump();
            std::cerr << "\n";
            break;
        case QuadOp::IfNeq:
            std::cerr << "if (";
            arg1->dump();
            std::cerr << " != ";
            arg2->dump();
            std::cerr << ") goto ";
            res->dump();
            std::cerr << "\n";
            break;
        case QuadOp::IfLt:
            std::cerr << "if (";
            arg1->dump();
            std::cerr << " < ";
            arg2->dump();
            std::cerr << ") goto ";
            res->dump();
            std::cerr << "\n";
            break;
        case QuadOp::IfLeq:
            std::cerr << "if (";
            arg1->dump();
            std::cerr << " <= ";
            arg2->dump();
            std::cerr << ") goto ";
            res->dump();
            std::cerr << "\n";
            break;
        case QuadOp::IfGt:
            std::cerr << "if (";
            arg1->dump();
            std::cerr << " > ";
            arg2->dump();
            std::cerr << ") goto ";
            res->dump();
            std::cerr << "\n";
            break;
        case QuadOp::IfGeq:
            std::cerr << "if (";
            arg1->dump();
            std::cerr << " >= ";
            arg2->dump();
            std::cerr << ") goto ";
            res->dump();
            std::cerr << "\n";
            break;
        case QuadOp::Enter:
            std::cerr << "enter ";
            arg1->dump();
            std::cerr << "\n";
            break;
        case QuadOp::Return:
            if (arg1 != nullptr)
            {
                std::cerr << "return ";
                arg1->dump();
                std::cerr << "\n";
            }
            else
            {
                std::cerr << "return\n";
            }

            break;
        case QuadOp::Param:
            std::cerr << "param ";
            arg1->dump();
            std::cerr << "\n";
            break;
        case QuadOp::Call:
            std::cerr << "call ";
            arg1->dump();
            std::cerr << ", ";
            arg2->dump();
            std::cerr << "\n";
            break;
        case QuadOp::Retrieve:
            std::cerr << "retrieve ";
            res->dump();
            std::cerr << "\n";
            break;
    }
}

void QuadList::dump()
{
    for (auto& quad = head; quad != nullptr; quad = quad->next)
    {
        quad->dump();
    }
}

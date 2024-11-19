#include <iostream>
#include <cassert>
#include "Quad.hpp"

/********************************************************************************/
/*                           Operand Factories                                  */
/********************************************************************************/

/**
 * Makes a int constant operand.
 */
std::shared_ptr<Operand> Operand::MakeIntConstOperand(long val, int nbytes)
{
    return std::make_shared<Operand>(OperandType::IntConst, val, nbytes);
}

/**
 * Makes a str constant operand.
 */
std::shared_ptr<Operand> Operand::MakeStrConstOperand(std::string val)
{
    return std::make_shared<Operand>(OperandType::StrConst, val);
}

/**
 * Makes a variable operand.
 */
std::shared_ptr<Operand> Operand::MakeVariableOperand(std::shared_ptr<Symbol> val)
{
    return std::make_shared<Operand>(OperandType::Variable, val);
}

/**
 * Makes a label operand.
 */
std::shared_ptr<Operand> Operand::MakeLabelOperand()
{
    static int label_counter = 0;
    auto label = "__L" + std::to_string(label_counter) + "__";
    label_counter++;
    return std::make_shared<Operand>(OperandType::Label, label);
}

/**
 * Makes an expression operand.
 */
std::shared_ptr<Operand> Operand::MakeExprTypeOperand(std::shared_ptr<Type> val)
{
    return std::make_shared<Operand>(OperandType::ExprType, val);
}

/********************************************************************************/
/*                              Quad Factories                                  */
/********************************************************************************/

/**
 * Makes a global quad operation.
 */
std::shared_ptr<Quad> Quad::MakeGlobalOp(std::shared_ptr<Operand> arg1)
{
    assert(arg1 != nullptr && arg1->type == OperandType::Variable);
    return std::make_shared<Quad>(QuadOp::Global, arg1, nullptr, nullptr);
}

/**
 * Makes a string quad operation.
 */
std::shared_ptr<Quad> Quad::MakeStringOp(std::shared_ptr<Operand> arg1)
{
    assert(arg1 != nullptr && arg1->type == OperandType::StrConst);
    return std::make_shared<Quad>(QuadOp::String, arg1, nullptr, nullptr);
}

/**
 * Makes a binary quad operation.
 */
std::shared_ptr<Quad> Quad::MakeBinOp(QuadOp op, std::shared_ptr<Operand> arg1, std::shared_ptr<Operand> arg2, std::shared_ptr<Operand> res)
{
    assert(op == QuadOp::Add || op == QuadOp::Sub || op == QuadOp::Mul || op == QuadOp::Div || op == QuadOp::Mod);
    return std::make_shared<Quad>(op, arg1, arg2, res);
}

/**
 * Makes an unary quad operation.
 */
std::shared_ptr<Quad> Quad::MakeUnOp(QuadOp op, std::shared_ptr<Operand> arg1, std::shared_ptr<Operand> res)
{
    assert(op == QuadOp::Neg || op == QuadOp::RDeref || op == QuadOp::AddrOf || op == QuadOp::Copy);
    return std::make_shared<Quad>(op, arg1, nullptr, res);
}

// /**
//  * Makes a right index quad operation.
//  */
// std::shared_ptr<Quad> Quad::MakeRIndexOp(std::shared_ptr<Operand> array, std::shared_ptr<Operand> index, std::shared_ptr<Operand> res)
// {
//     return std::make_shared<Quad>(QuadOp::RIndex, array, index, res);
// }

// /**
//  * Makes a left index quad operation.
//  */
// std::shared_ptr<Quad> Quad::MakeLIndexOp(std::shared_ptr<Operand> arg1, std::shared_ptr<Operand> index, std::shared_ptr<Operand> array)
// {
//     return std::make_shared<Quad>(QuadOp::LIndex, arg1, index, array);
// }

/**
 * Makes an addr of quad operation.
 */
std::shared_ptr<Quad> Quad::MakeAddrOfOp(std::shared_ptr<Operand> arg1, std::shared_ptr<Operand> res)
{
    return std::make_shared<Quad>(QuadOp::AddrOf, arg1, nullptr, res);
}

/**
 * Makes a right deref quad operation.
 */
std::shared_ptr<Quad> Quad::MakeRDerefOp(std::shared_ptr<Operand> arg1, std::shared_ptr<Operand> res)
{
    return std::make_shared<Quad>(QuadOp::RDeref, arg1, nullptr, res);
}

/**
 * Makes a left deref quad operation.
 */
std::shared_ptr<Quad> Quad::MakeLDerefOp(std::shared_ptr<Operand> arg1, std::shared_ptr<Operand> res)
{
    return std::make_shared<Quad>(QuadOp::LDeref, arg1, nullptr, res);
}

/**
 * Makes an add pointer quad operation.
 */
std::shared_ptr<Quad> Quad::MakeAddPtrOp(std::shared_ptr<Operand> arg1, std::shared_ptr<Operand> arg2, std::shared_ptr<Operand> res)
{
    assert(arg1->type == OperandType::Variable);
    return std::make_shared<Quad>(QuadOp::AddPtr, arg1, arg2, res);
}

/**
 * Makes a label quad operation.
 */
std::shared_ptr<Quad> Quad::MakeLabelOp(std::shared_ptr<Operand> label)
{
    return std::make_shared<Quad>(QuadOp::Label, label, nullptr, nullptr);
}

/**
 * Makes a goto quad operation.
 */
std::shared_ptr<Quad> Quad::MakeGotoOp(std::shared_ptr<Operand> label)
{
    return std::make_shared<Quad>(QuadOp::Goto, label, nullptr, nullptr);
}

/**
 * Makes an if quad operation.
 */
std::shared_ptr<Quad> Quad::MakeIfOp(QuadOp op, std::shared_ptr<Operand> arg1, std::shared_ptr<Operand> arg2, std::shared_ptr<Operand> res)
{
    assert(op == QuadOp::IfEq || op == QuadOp::IfNeq || op == QuadOp::IfLt || op == QuadOp::IfLeq || op == QuadOp::IfGt || op == QuadOp::IfGeq);
    return std::make_shared<Quad>(op, arg1, arg2, res);
}

/**
 * Makes an enter quad operation.
 */
std::shared_ptr<Quad> Quad::MakeEnterOp(std::shared_ptr<Operand> func)
{
    return std::make_shared<Quad>(QuadOp::Enter, func, nullptr, nullptr);
}

/**
 * Makes a return quad operation.
 */
std::shared_ptr<Quad> Quad::MakeReturnOp(std::shared_ptr<Operand> arg1)
{
    return std::make_shared<Quad>(QuadOp::Return, arg1, nullptr, nullptr);
}

/**
 * Makes a return quad operation.
 */
std::shared_ptr<Quad> Quad::MakeReturnOp()
{
    return std::make_shared<Quad>(QuadOp::Return, nullptr, nullptr, nullptr);
}

/**
 * Makes a param quad operation.
 */
std::shared_ptr<Quad> Quad::MakeParamOp(std::shared_ptr<Operand> arg1)
{
    return std::make_shared<Quad>(QuadOp::Param, arg1, nullptr, nullptr);
}

/**
 * Makes a call quad operation.
 */
std::shared_ptr<Quad> Quad::MakeCallOp(std::shared_ptr<Operand> func, std::shared_ptr<Operand> nargs, std::shared_ptr<Operand> res)
{
    return std::make_shared<Quad>(QuadOp::Call, func, nargs, res);
}

/**
 * Makes a cast operation.
 */
std::shared_ptr<Quad> Quad::MakeCastOp(std::shared_ptr<Operand> expr, std::shared_ptr<Operand> type, std::shared_ptr<Operand> res)
{
    return std::make_shared<Quad>(QuadOp::Cast, expr, type, res);
}

/********************************************************************************/
/*                                    QuadList                                  */
/********************************************************************************/

/**
 * Pushes a quad to the list.
 */
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

/**
 * Appends a quad to the list.
 */
QuadList QuadList::append(QuadList& list, std::shared_ptr<Quad> quad)
{
    auto list2 = QuadList(quad, quad);
    return QuadList::concat(list, list2);
}

/**
 * Concats two lists.
 */
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

/**
 * Dumps the operand.
 */
void Operand::dump()
{
    switch (type)
    {
        case OperandType::IntConst:
            std::cerr << iconst.value;
            break;
        case OperandType::StrConst:
        case OperandType::Label:
            std::cerr << strconst;
            break;
        case OperandType::Variable:
            std::cerr << symbol->get_name();
            break;
        case OperandType::ExprType:
            expr_type->dump();
            break;
    }
}

/**
 * Dumps the quad.
 */
void Quad::dump()
{
    switch (op)
    {
        case QuadOp::Global:
            std::cerr << "global ";
            arg1->dump();
            std::cerr << " ";
            arg1->symbol->type->dump();
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
        case QuadOp::Copy:
            res->dump();
            std::cerr << " = ";
            arg1->dump();
            std::cerr << "\n";
            break;
        // case QuadOp::LIndex:
        //     res->dump();
        //     std::cerr << "[";
        //     arg2->dump();
        //     std::cerr << "] = ";
        //     arg1->dump();
        //     std::cerr << "\n";
        //     break;
        // case QuadOp::RIndex:
        //     res->dump();
        //     std::cerr << " = ";
        //     arg1->dump();
        //     std::cerr << "[";
        //     arg2->dump();
        //     std::cerr << "]\n";
        //     break;
        case QuadOp::AddrOf:
            res->dump();
            std::cerr << " = &";
            arg1->dump();
            std::cerr << "\n";
            break;
        case QuadOp::RDeref:
            res->dump();
            std::cerr << " = *";
            arg1->dump();
            std::cerr << "\n";
            break;
        case QuadOp::LDeref:
            std::cerr << "*";
            res->dump();
            std::cerr << " = ";
            arg1->dump();
            std::cerr << "\n";
            break;
        case QuadOp::AddPtr:
            res->dump();
            std::cerr << " = ";
            arg1->dump();
            std::cerr << " + ";
            arg2->dump();
            std::cerr << "\n";
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
            res->dump();

            std::cerr << " = call ";
            arg1->dump();
            std::cerr << ", ";
            arg2->dump();
            std::cerr << "\n";
            break;
        case QuadOp::Cast:
            res->dump();
            std::cerr << " = (";
            arg2->dump();
            std::cerr << ")";
            arg1->dump();
            std::cerr << "\n";
            break;
    }
}

/**
 * Dumps the quad list.
 */
void QuadList::dump()
{
    for (auto& quad = head; quad != nullptr; quad = quad->next)
    {
        quad->dump();
    }
}

#pragma once

#include <string>
#include <memory>
#include "SymbolTable.hpp"

enum class QuadOp
{
    Global,
    String,
    Add,
    Sub,
    Mul,
    Div,
    Mod,
    Neg,
    Deref,
    Addr,
    Copy,
    LIndex, // a[i] = x
    RIndex, // x = a[i]
    Label,
    Goto,
    IfEq,
    IfNeq,
    IfLt,
    IfLeq,
    IfGt,
    IfGeq,
    Enter,
    Return,
    Param,
    Call,
    Retrieve
};

enum class OperandType
{
    IntConst,
    StrConst,
    Variable,
    Label
};

struct Operand
{
    OperandType type;
    union 
    {
        long iconst;
        std::string strconst;
        std::shared_ptr<Symbol> symbol;
        std::string label;
    } value;
};

struct Quad
{
    QuadOp op;
    std::shared_ptr<Operand> arg1;
    std::shared_ptr<Operand> arg2;
    std::shared_ptr<Operand> res;
    Quad(QuadOp op, std::shared_ptr<Operand> arg1, std::shared_ptr<Operand> arg2, std::shared_ptr<Operand> res) : op(op), arg1(arg1), arg2(arg2), res(res) {}

    static std::shared_ptr<Quad> MakeBinOp(QuadOp op, std::shared_ptr<Operand> arg1, std::shared_ptr<Operand> arg2, std::shared_ptr<Operand> res);
    static std::shared_ptr<Quad> MakeUnOp(QuadOp op, std::shared_ptr<Operand> arg1, std::shared_ptr<Operand> res);
    static std::shared_ptr<Quad> MakeRIndexOp(std::shared_ptr<Operand> array, std::shared_ptr<Operand> index, std::shared_ptr<Operand> res);
    static std::shared_ptr<Quad> MakeLIndexOp(std::shared_ptr<Operand> arg1, std::shared_ptr<Operand> index, std::shared_ptr<Operand> array);
    static std::shared_ptr<Quad> MakeLabelOp(std::shared_ptr<Operand> label);
    static std::shared_ptr<Quad> MakeGotoOp(std::shared_ptr<Operand> label);
    static std::shared_ptr<Quad> MakeIfOp(QuadOp op, std::shared_ptr<Operand> arg1, std::shared_ptr<Operand> arg2, std::shared_ptr<Operand> res);
    static std::shared_ptr<Quad> MakeEnterOp(std::shared_ptr<Operand> func);
    static std::shared_ptr<Quad> MakeReturnOp(std::shared_ptr<Operand> arg1);
    static std::shared_ptr<Quad> MakeReturnOp();
    static std::shared_ptr<Quad> MakeParamOp(std::shared_ptr<Operand> arg1);
    static std::shared_ptr<Quad> MakeCallOp(std::shared_ptr<Operand> func, std::shared_ptr<Operand> nargs);
    static std::shared_ptr<Quad> MakeRetrieveOp(std::shared_ptr<Operand> res);
};

class QuadList
{
private:
    std::shared_ptr<Quad> head;
    std::shared_ptr<Quad> tail;

public:
    QuadList(): head(nullptr), tail(nullptr) {}
    QuadList(std::shared_ptr<Quad> head, std::shared_ptr<Quad> tail): head(head), tail(tail) {} 
};
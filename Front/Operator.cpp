#include <iostream>
#include "Operator.hpp"

BinOp getBinOp(std::string op)
{
    if (op == "+")
    {
        return BinOp::Plus;
    }
    else if (op == "-")
    {
        return BinOp::Minus;
    }
    else if (op == "*")
    {
        return BinOp::Times;
    }
    else if (op == "/")
    {
        return BinOp::Divide;
    }
    else if (op == "%")
    {
        return BinOp::Modulo;
    }
    else if (op == "=")
    {
        return BinOp::Assign;
    }

    std::cerr << "Error: unrecognized binary operator " << op << "\n";
    throw std::exception();
}

std::string toString(BinOp op)
{
    switch (op)
    {
        case BinOp::Plus:
            return "+";
        case BinOp::Minus:
            return "-";
        case BinOp::Times:
            return "*";
        case BinOp::Divide:
            return "/";
        case BinOp::Modulo:
            return "%";
        case BinOp::Assign:
            return "=";
    }
}

UnOp getUnOp(std::string op)
{
    if (op == "-")
    {
        return UnOp::Negation;
    }
    else if (op == "*")
    {
        return UnOp::Deref;
    }
    else if (op == "&")
    {
        return UnOp::AddrOf;
    }

    // TODO do the rest later

    std::cerr << "Error: unrecognized unary operator " << op << "\n";
    throw std::exception();
}

std::string toString(UnOp op)
{
    switch (op)
    {
        case UnOp::Negation:
            return "-";
        case UnOp::Deref:
            return "*";
        case UnOp::AddrOf:
            return "&";
        case UnOp::Pre_PlusPlus:
            return "++<";
        case UnOp::Post_PlusPlus:
            return ">++";
        case UnOp::Pre_MinusMinus:
            return "--<";
        case UnOp::Post_MinusMinus:
            return ">--";
    }
}

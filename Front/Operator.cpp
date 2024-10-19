#include <iostream>
#include "cassert"
#include "Operator.hpp"

/**
 * Gets the BinOp for the string.
 */
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

    assert(false && (std::string("unrecognized unary operator ") + op).c_str());
}

/**
 * Gets the string representation of the BinOp.
 */
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

/**
 * Gets the UnOp for the string.
 */
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

    assert(false && (std::string("unrecognized unary operator ") + op).c_str());
}

/**
 * Gets the string representation of the UnOp.
 */
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

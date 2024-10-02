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

    // TODO error
    std::exit(1);
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

    // TODO error
    std::exit(1);
}

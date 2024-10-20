#include <iostream>
#include <algorithm>
#include "cassert"
#include "Operator.hpp"

/**
 * Gets the BinOp for the string.
 */
const BinOp getBinOp(const std::string op)
{
    if (binop_map.find(op) != binop_map.end())
    {
        return binop_map.at(op);
    }

    assert(false && (std::string("unrecognized unary operator ") + op).c_str());
}

/**
 * Gets the string representation of the BinOp.
 */
const std::string toString(const BinOp op)
{

    for (auto& pair : binop_map)
    {
        if (pair.second == op)
        {
            return pair.first;
        }
    }

    return "??";
}

/**
 * Gets the UnOp for the string.
 */
const UnOp getUnOp(const std::string op)
{
    if (unop_map.find(op) != unop_map.end())
    {
        return unop_map.at(op);
    }

    assert(false && (std::string("unrecognized unary operator ") + op).c_str());
}

/**
 * Gets the string representation of the UnOp.
 */
const std::string toString(const UnOp op)
{
    for (auto& pair : unop_map)
    {
        if (pair.second == op)
        {
            return pair.first;
        }
    }

    return "??";
}

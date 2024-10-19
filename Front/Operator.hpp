#pragma once

#include <string>
#include <vector>

/**
 * Encodes operator precedence.
 */
inline std::vector<std::vector<std::string>> operator_precedence = {
    { "*", "/", "%" },
    { "+", "-" }, // TODO add more later
    { "=" }
};

/**
 * Represents a binary operation type.
 */
enum class BinOp
{
    Plus,
    Minus,
    Times,
    Divide,
    Modulo,
    Assign
};

/**
 * Represents a unary operation type.
 */
enum class UnOp
{
    Negation,
    Deref,
    AddrOf,
    Pre_PlusPlus,
    Post_PlusPlus,
    Pre_MinusMinus,
    Post_MinusMinus
};

BinOp getBinOp(std::string op);
std::string toString(BinOp op);

UnOp getUnOp(std::string op);
std::string toString(UnOp op);

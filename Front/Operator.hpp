#pragma once

#include <string>
#include <vector>

inline std::vector<std::vector<std::string>> operator_precedence = {
    { "*", "/", "%" },
    { "+", "-" } // TODO add more later
};

enum class BinOp
{
    Plus,
    Minus,
    Times,
    Divide,
    Modulo
};

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
UnOp getUnOp(std::string op);

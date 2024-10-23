#pragma once

#include <string>
#include <vector>
#include <map>

/**
 * Encodes operator precedence.
 */
const std::vector<std::vector<std::string>> operator_precedence = {
    { "*", "/", "%" },              /*  3  */   
    { "+", "-" },                   /*  4  */   
    { "<", "<=", ">", ">=" },       /*  6  */   
    { "==", "!=" },                 /*  7  */   
    { "&&" },                       /* 11  */   
    { "||" },                       /* 12  */   
    { "=", "+=", "-=", "*=", "%=" } /* 14  */   
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
    Equal,
    NotEqual,
    LessThan,
    LessThanEqual,
    GreaterThan,
    GreaterThanEqual,
    And,
    Or,
    Assign,
    PlusAssign,
    MinusAssign,
    TimesAssign,
    DivideAssign,
    ModuloAssign
};

const std::map<const std::string, const BinOp> binop_map = {
    {"*", BinOp::Times}, {"/", BinOp::Divide}, {"%", BinOp::Modulo},
    {"+", BinOp::Plus}, {"-", BinOp::Minus},
    {"<", BinOp::LessThan}, {"<=", BinOp::LessThanEqual}, {">", BinOp::GreaterThan}, {">=", BinOp::GreaterThanEqual},
    {"==", BinOp::Equal}, {"!=", BinOp::NotEqual},
    {"&&", BinOp::And},
    {"||", BinOp::Or}, 
    {"=", BinOp::Assign}, {"+=", BinOp::PlusAssign}, {"-=", BinOp::MinusAssign}, {"*=", BinOp::TimesAssign}, {"/=", BinOp::DivideAssign}, {"%=", BinOp::ModuloAssign},
};

/**
 * Represents a unary operation type.
 */
enum class UnOp
{
    Negation,
    Not,
    Deref,
    AddrOf,
    PlusPlus,
    MinusMinus,
};

const std::map<const std::string, const UnOp> unop_map = {
    {"++", UnOp::PlusPlus}, {"--", UnOp::MinusMinus},
    {"-", UnOp::Negation}, {"!", UnOp::Not}, {"*", UnOp::Deref}, {"&", UnOp::AddrOf},
};


const BinOp get_BinOp(const std::string op);
const std::string to_string(const BinOp op);

const UnOp get_UnOp(const std::string op);
const std::string to_string(const UnOp op);

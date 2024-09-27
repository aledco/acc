#pragma once

#include <vector>
#include <memory>

enum class TypeType
{
    Void,
    Int,
    Function,
    Array // TODO add more
};

struct Type
{
    TypeType type;
    std::shared_ptr<Type> alt_type;

    Type(TypeType type) : type(type) {}
    Type(TypeType type, std::shared_ptr<Type> alt_type) : type(type), alt_type(alt_type) {}

    int size();
};

// struct Type
// {
//     inline virtual int size() { return 0; }
// };

// struct VoidType : Type
// {
// };

// struct IntType : Type
// {
//     inline virtual int size() { return 4; }
// };

// struct FunctionType : Type
// {
//     std::shared_ptr<Type> return_type;
//     std::vector<std::shared_ptr<Type>> param_types;

//     FunctionType(std::shared_ptr<Type> return_type, std::vector<std::shared_ptr<Type>> param_types) : return_type(return_type), param_types(param_types) {}
// };

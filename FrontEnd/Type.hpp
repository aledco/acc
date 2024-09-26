#pragma once

#include <vector>

struct Type
{
    inline virtual int size() { return 0; }
};

struct IntType : Type
{
    inline virtual int size() { return 4; }
};

struct FunctionType : Type
{
    Type return_type;
    std::vector<Type> param_types;

    FunctionType(Type& return_type, std::vector<Type>& param_types) : return_type(return_type), param_types(param_types) {}
};
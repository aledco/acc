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

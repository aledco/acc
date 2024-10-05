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

    bool operator== (const Type& other)
    {
        switch (type)
        {
            case TypeType::Void:
            case TypeType::Int:
                return type == other.type;
            case TypeType::Function:
            case TypeType::Array:
                if (type != other.type)
                {
                    return false;
                }

                return alt_type == other.alt_type;
        }
    }

    int size();
    void dump();
};

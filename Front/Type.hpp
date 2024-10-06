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
    std::shared_ptr<Type> ret_type;
    std::vector<std::shared_ptr<Type>> param_types;
    std::shared_ptr<Type> elem_type;

    Type(TypeType type) : type(type) {}
    Type(TypeType type, std::shared_ptr<Type> ret_type, std::vector<std::shared_ptr<Type>> param_types) : type(type), ret_type(ret_type), param_types(param_types)  {}
    Type(TypeType type, std::shared_ptr<Type> elem_type) : type(type), elem_type(elem_type) {}

    bool operator== (const Type& other)
    {
        switch (type)
        {
            case TypeType::Void:
            case TypeType::Int:
                return type == other.type;
            case TypeType::Function:
                if (type != other.type || ret_type != ret_type || param_types.size() != other.param_types.size())
                {
                    return false;
                }

                for (auto i = 0; i < param_types.size(); i++)
                {
                    if (param_types[i] != other.param_types[i])
                    {
                        return false;
                    }
                }

                return true;
            case TypeType::Array:
                if (type != other.type)
                {
                    return false;
                }

                return ret_type == other.ret_type;
        }
    }

    bool operator!= (const Type& other) { return !operator==(other); }

    int size();
    void dump();
};

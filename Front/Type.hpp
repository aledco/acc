#pragma once

#include <vector>
#include <memory>
#include <optional>

/**
 * The type of the type.
 */
enum class TypeType
{
    Void,
    Int,
    Char,
    Function,
    Array,
    Pointer
};

/**
 * Represents the type of a value.
 */
struct Type
{
    TypeType type;
    
    /* for array and pointer types */
    std::shared_ptr<Type> elem_type;
    std::optional<int> num_elems;

    /* for function types */
    std::shared_ptr<Type> ret_type;
    std::vector<std::shared_ptr<Type>> param_types;
    bool is_extern;
    bool is_defined;

    Type(TypeType type) : type(type) {}
    Type(TypeType type, std::shared_ptr<Type> ret_type, std::vector<std::shared_ptr<Type>> param_types, bool is_extern, bool is_defined) : 
        type(type), 
        ret_type(ret_type),
        param_types(param_types),
        is_extern(is_extern),
        is_defined(is_defined)
    {}
    Type(TypeType type, std::shared_ptr<Type> elem_type) : type(type), elem_type(elem_type), num_elems({}) {}
    Type(TypeType type, std::shared_ptr<Type> elem_type, int num_elems) : type(type), elem_type(elem_type), num_elems(num_elems) {}

    bool operator== (const Type& other)
    {
        switch (type)
        {
            case TypeType::Void:
            case TypeType::Int:
            case TypeType::Char:
                return type == other.type;
            case TypeType::Function:
                if (type != other.type || *ret_type != *ret_type || param_types.size() != other.param_types.size())
                {
                    return false;
                }

                for (auto i = 0; i < param_types.size(); i++)
                {
                    if (*param_types[i] != *other.param_types[i])
                    {
                        return false;
                    }
                }

                return true;
            case TypeType::Array:
            case TypeType::Pointer:
                if (other.type != TypeType::Array && TypeType::Array != TypeType::Pointer)
                {
                    return false;
                }

                return *elem_type == *other.elem_type;
        }
    }

    bool operator!= (const Type& other) { return !operator==(other); }

    int size();
    void dump();
};

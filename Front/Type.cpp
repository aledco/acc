#include <iostream>
#include "Type.hpp"

/**
 * Gets the size of the type in bytes.
 */
int Type::size()
{
    switch (type)
    {
        case TypeType::Void:
            return 0;
        case TypeType::Int:
            return 4;
        case TypeType::Char:
                return 1;
        default:
            return 0; // TODO handle other types later
    }
}

/**
 * Dumps the type.
 */
void Type::dump()
{
    switch (type)
    {
        case TypeType::Void:
            std::cerr << "void";
            break;
        case TypeType::Int:
            std::cerr << "int";
            break;
        case TypeType::Char:
            std::cerr << "char";
            break;
        case TypeType::Function:
            std::cerr << "(";
            for (auto i = 0; i < param_types.size(); i++)
            {
                param_types[i]->dump();
                if (i < param_types.size() - 1)
                {
                    std::cerr << ", ";
                }
            }

            std::cerr << ")";
            std::cerr << " -> ";
            ret_type->dump();
            break;
        case TypeType::Array:
            elem_type->dump();
            std::cerr << "[]";
            break;
        case TypeType::Pointer:
            elem_type->dump();
            std::cerr << "*";
            break;
    }
}
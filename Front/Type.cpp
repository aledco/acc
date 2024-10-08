#include <iostream>
#include "Type.hpp"

int Type::size()
{
    switch (type)
    {
        case TypeType::Void:
            return 0;
        case TypeType::Int:
            return 4;
        default:
            return 0; // TODO handle other types later
    }
}

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
    }
}
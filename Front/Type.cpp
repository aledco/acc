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
            std::cerr << "->";
            alt_type->dump();
            break;
        case TypeType::Array:
            alt_type->dump();
            std::cerr << "[]";
            break;
    }
}
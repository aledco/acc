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
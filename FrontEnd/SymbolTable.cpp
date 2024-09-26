#include "SymbolTable.hpp"

void SymbolTable::add_symbol(std::string name, Type type, bool isparam)
{
    if (table.find(name) != table.end())
    {
        // TODO error, name has already been defined
        std::exit(1);
    }

    table[name] = Symbol(name, type, isparam);
}

Symbol& SymbolTable::lookup(std::string name)
{
    if (table.find(name) != table.end())
    {
        return table[name];
    }

    if (parent)
    {
        return parent.value().lookup(name);
    }

    // TODO error name not defined
    std::exit(1);
}

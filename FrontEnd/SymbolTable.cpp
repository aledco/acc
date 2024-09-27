#include "SymbolTable.hpp"

std::shared_ptr<Symbol> SymbolTable::add_symbol(std::string name, std::shared_ptr<Type> type)
{
    if (table.find(name) != table.end())
    {
        // TODO error, name has already been defined
        std::exit(1);
    }

    auto symbol = std::make_shared<Symbol>(name, type);
    table[name] = symbol;
    return symbol;
}

std::shared_ptr<Symbol> SymbolTable::lookup(std::string name)
{
    if (table.find(name) != table.end())
    {
        return table[name];
    }

    if (parent)
    {
        return parent->lookup(name);
    }

    // TODO error name not defined
    std::exit(1);
}

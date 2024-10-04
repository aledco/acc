#include <iostream>
#include "SymbolTable.hpp"

std::shared_ptr<Symbol> SymbolTable::add_symbol(Token& name, std::shared_ptr<Type> type)
{
    if (table.find(name.value) != table.end())
    {
        std::cerr << "Name Error: around " << name.span.end.line << ":" << name.span.end.col << ": " << name.value << " already defined\n";
        throw std::exception();
    }

    auto symbol = std::make_shared<Symbol>(name.value, type);
    table[name.value] = symbol;
    return symbol;
}

std::shared_ptr<Symbol> SymbolTable::lookup(Token& name)
{
    if (table.find(name.value) != table.end())
    {
        return table[name.value];
    }

    if (parent)
    {
        return parent->lookup(name);
    }

    std::cerr << "Name Error: around " << name.span.end.line << ":" << name.span.end.col << ": unknown identifier " << name.value << "\n";
    throw std::exception();
}

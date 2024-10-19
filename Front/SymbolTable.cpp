#include <iostream>
#include <cassert>
#include "SymbolTable.hpp"

/**
 * Adds a symbol to the symbol table.
 */
std::shared_ptr<Symbol> SymbolTable::add_symbol(Token& name, std::shared_ptr<Type> type)
{
    if (table.find(name.value) != table.end())
    {
        std::cerr << "Name Error: around " << name.span.end.line << ":" << name.span.end.col << ": " << name.value << " already defined\n";
        throw std::exception();
    }

    return add_symbol(name.value, type);
}

/**
 * Adds a symbol to the symbol table.
 */
std::shared_ptr<Symbol> SymbolTable::add_symbol(std::string name, std::shared_ptr<Type> type)
{
    assert(table.find(name) == table.end());
    auto symbol = std::make_shared<Symbol>(name, type);
    table[name] = symbol;
    return symbol;
}

/**
 * Looks up a symbol in the symbol table. Throws an exception if the symbol is not found.
 */
std::shared_ptr<Symbol> SymbolTable::lookup(Token& name)
{
    auto symbol = try_lookup(name);
    if (symbol == nullptr)
    {
        std::cerr << "Name Error: around " << name.span.end.line << ":" << name.span.end.col << ": unknown identifier " << name.value << "\n";
        throw std::exception();
    }
    
    return symbol;
}

/**
 * Looks up a symbol in the symbol table. Throws an exception if the symbol is not found.
 */
std::shared_ptr<Symbol> SymbolTable::lookup(std::string name)
{
    if (table.find(name) != table.end())
    {
        return table[name];
    }

    assert(parent);
    return parent->lookup(name);
}

/**
 * Looks up a symbol in the symbol table. Returns nullptr if the symbol is not found.
 */
std::shared_ptr<Symbol> SymbolTable::try_lookup(Token& name)
{
    if (table.find(name.value) != table.end())
    {
        return table[name.value];
    }

    if (parent)
    {
        return parent->lookup(name);
    }

    return nullptr;
}

/**
 * Creates a temporary variable and adds it to the symbol table.
 */
std::shared_ptr<Symbol> SymbolTable::new_temp(std::shared_ptr<Type> type)
{
    auto name = "t." +  std::to_string(temp_count);
    temp_count++;
    auto temp = add_symbol(name, type);
    temp->is_temp = true;
    temps.push_back(temp);
    return temp;
}

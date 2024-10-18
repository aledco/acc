#include <iostream>
#include <cassert>
#include "SymbolTable.hpp"

std::shared_ptr<Symbol> SymbolTable::add_symbol(Token& name, std::shared_ptr<Type> type)
{
    if (table.find(name.value) != table.end())
    {
        std::cerr << "Name Error: around " << name.span.end.line << ":" << name.span.end.col << ": " << name.value << " already defined\n";
        throw std::exception();
    }

    return add_symbol(name.value, type);
}

std::shared_ptr<Symbol> SymbolTable::add_symbol(std::string name, std::shared_ptr<Type> type)
{
    assert(table.find(name) == table.end());
    auto symbol = std::make_shared<Symbol>(name, type);
    table[name] = symbol;
    return symbol;
}

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

 std::shared_ptr<Symbol> SymbolTable::lookup(std::string name)
 {
    if (table.find(name) != table.end())
    {
        return table[name];
    }

    assert(parent);
    return parent->lookup(name);
 }

std::shared_ptr<Symbol> SymbolTable::new_temp(std::shared_ptr<Type> type)
{
    for (auto& temp : free_temps)
    {
        if (temp->type == type)
        {
            free_temps.remove(temp);
            return temp;
        }
    }

    auto name = "$temp" +  std::to_string(temp_count);
    temp_count++;
    return add_symbol(name, type);
}

void SymbolTable::free_temp(std::shared_ptr<Symbol> temp)
{
    free_temps.push_back(temp);
}

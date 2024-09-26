#pragma once

#include <unordered_map>
#include <string>
#include <optional>
#include "Type.hpp"

struct Symbol
{
    std::string name;
    Type type;
    bool isparam;

    Symbol(std::string name, Type type, bool isparam) : name(name), type(type), isparam(isparam) {}
};

class SymbolTable
{
private:
    std::optional<SymbolTable&> parent;
    std::unordered_map<std::string, Symbol> table;

public:
    SymbolTable(): parent({}) {}
    SymbolTable(SymbolTable& parent): parent(parent) {}

    void add_symbol(std::string name, Type type, bool isparam);
    Symbol& lookup(std::string name);
};
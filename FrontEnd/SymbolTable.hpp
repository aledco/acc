#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include "Type.hpp"

struct Symbol
{
    std::string name;
    Type type;
    bool isparam;

    Symbol(std::string name, Type type, bool isparam): name(name), type(type), isparam(isparam) {}
};

class SymbolTable
{
private:
    std::shared_ptr<SymbolTable> parent;
    std::unordered_map<std::string, std::shared_ptr<Symbol>> table;

public:
    SymbolTable(): parent(nullptr) {}
    SymbolTable(std::shared_ptr<SymbolTable> parent): parent(parent) {}

    std::shared_ptr<Symbol> add_symbol(std::string name, Type type, bool isparam);
    std::shared_ptr<Symbol> lookup(std::string name);
};
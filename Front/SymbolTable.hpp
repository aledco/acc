#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include "Type.hpp"

struct Symbol
{
    std::string name;
    std::shared_ptr<Type> type;

    Symbol(std::string name, std::shared_ptr<Type> type): name(name), type(type) {}
};

class SymbolTable
{
private:
    std::shared_ptr<SymbolTable> parent;
    std::unordered_map<std::string, std::shared_ptr<Symbol>> table;

public:
    SymbolTable(): parent(nullptr) {}
    SymbolTable(std::shared_ptr<SymbolTable> parent): parent(parent) {}

    std::shared_ptr<Symbol> add_symbol(std::string name, std::shared_ptr<Type> type);
    std::shared_ptr<Symbol> lookup(std::string name);
};
#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <list>
#include "Lexer.hpp"
#include "Type.hpp"
#include "CodegenSymbolData.hpp"

struct Symbol
{
    std::string name;
    std::shared_ptr<Type> type;
    bool is_parameter = false;
    bool is_temp = false;
    CodegenSymbolData symbol_data;

    Symbol(std::string name, std::shared_ptr<Type> type): name(name), type(type) {}
};

class SymbolTable
{
private:
    std::shared_ptr<SymbolTable> parent;
    std::unordered_map<std::string, std::shared_ptr<Symbol>> table;

    long temp_count = 0;
    std::list<std::shared_ptr<Symbol>> free_temps;

public:
    int scope;

    SymbolTable(): parent(nullptr), scope(0) {}
    SymbolTable(std::shared_ptr<SymbolTable> parent): parent(parent), scope(parent->scope + 1) {}

    std::shared_ptr<Symbol> add_symbol(Token& name, std::shared_ptr<Type> type);
    std::shared_ptr<Symbol> add_symbol(std::string name, std::shared_ptr<Type> type);
    std::shared_ptr<Symbol> lookup(Token& name);
    std::shared_ptr<Symbol> lookup(std::string name);
    std::shared_ptr<Symbol> try_lookup(Token& name);

    std::shared_ptr<Symbol> new_temp(std::shared_ptr<Type> type);
    void free_temp(std::shared_ptr<Symbol> temp);
};
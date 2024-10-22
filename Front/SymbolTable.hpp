#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <vector>
#include "Lexer.hpp"
#include "Type.hpp"
#include "CodegenSymbolData.hpp"

#define GLOBAL_SCOPE 0

/**
 * Represents a symbol in the symbol table.
 */
struct Symbol
{
private:
    std::string name;

public:
    std::shared_ptr<Type> type;
    int scope;

    bool is_parameter = false;
    bool is_temp = false;

    CodegenSymbolData symbol_data; // stores codegen specific information
                                   // backend supplies this struct

    Symbol(std::string name, std::shared_ptr<Type> type, int scope): name(name), type(type), scope(scope) {}

    std::string get_name();
};

/**
 * Represents a symbol table.
 */
class SymbolTable
{
private:
    std::shared_ptr<SymbolTable> parent;
    std::vector<SymbolTable *> children;
    std::unordered_map<std::string, std::shared_ptr<Symbol>> table;

    long temp_count = 0;
    std::vector<std::shared_ptr<Symbol>> temps;

public:
    int scope;

    SymbolTable(): parent(nullptr), scope(GLOBAL_SCOPE) {}
    SymbolTable(std::shared_ptr<SymbolTable> parent): parent(parent), scope(parent->scope + 1) 
    {
        parent->children.push_back(this);
    }

    std::shared_ptr<Symbol> add_symbol(Token& name, std::shared_ptr<Type> type);
    std::shared_ptr<Symbol> add_symbol(std::string name, std::shared_ptr<Type> type);
    std::shared_ptr<Symbol> lookup(Token& name);
    std::shared_ptr<Symbol> lookup(std::string name);
    std::shared_ptr<Symbol> try_lookup(Token& name);

    std::vector<std::shared_ptr<Symbol>> get_all_variables();

    std::shared_ptr<Symbol> new_temp(std::shared_ptr<Type> type);
};
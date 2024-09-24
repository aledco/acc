#pragma once

#include <string>
#include <vector>
#include <memory>

enum class TokenType 
{
    Id,
    Op,
    Sep,
    Type
};

struct Token
{
    TokenType type;
    std::string value;
};

class Lexer
{
private:
    std::string input;
    size_t index = 0;
    std::vector<std::unique_ptr<Token>> tokens;

    void lex();
    bool eof() const { return index >= input.length(); }
    char current() const { return input[index]; }
    std::string slice(int length) { return input.substr(index, length); }
    bool whitespace() const { return std::isspace(current()); }
public:
    Lexer(const std::string& input) : input(input) {}

    auto begin() { return tokens.begin(); }
    auto end() { return tokens.end(); }
};
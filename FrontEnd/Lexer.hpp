#pragma once

#include <string>
#include <vector>
#include <optional>
#include <memory>

enum class TokenType 
{
    Integer,
    Id,
    Keyword,
    Op,
    Sep
};

struct Token
{
    TokenType type;
    std::string value;

    Token(const TokenType type, const std::string& value) : type(type), value(value) {}
};

std::vector<std::string> seperators = {
    ";",
    ",",
    "(",
    ")",
    "{",
    "}",
    "[",
    "]"
};

std::vector<std::string> operators = {
    "+",
    "-",
    "*",
    "/",
    "%",
    "&",
    "++",
    "--",
    "+=",
    "-=",
    "*=",
    "/=",
    "%="
};

std::vector<std::string> keywords = {
    "void",
    "int", // TODO add more later
    "return"
};

bool vector_contains_char(std::vector<std::string>&, char c)
{
    for (auto sep : seperators)
    {
        if (sep.find(c) != std::string::npos) 
        {
            return true;
        }
    }

    return false;
}

class Lexer
{
private:
    std::string input;
    size_t index = 0;
    std::vector<std::unique_ptr<Token>> tokens;

    void lex();

    std::optional<std::unique_ptr<Token>> attempt_lex_keyword();
    std::unique_ptr<Token> lex_integer();
    std::unique_ptr<Token> lex_id();
    std::unique_ptr<Token> Lexer::lex_sep();
    std::unique_ptr<Token> Lexer::lex_op();

    bool eof() const { return index >= input.length(); }
    char current() const { return input[index]; }
    std::string slice(int length) { return input.substr(index, length); }
    void advance() { index++; }
    void advance(int i) { index += i; }

    bool iswhitespace() const { return std::isspace(current()); }
    bool isdigit() const { return std::isdigit(current()); }
    bool isfloat() const { return isdigit() || current() == '.'; }
    bool isalpha() const { return std::isalpha(current()); } 
    bool isalnum() const { return std::isalnum(current()); }
    bool isid() const { return isalnum() || current() == '_'; }
    bool issep() const { return vector_contains_char(seperators, current()); }
    bool isop() const { return vector_contains_char(operators, current()); }

public:
    Lexer(const std::string& input) : input(input) {}

    auto begin() { return tokens.begin(); }
    auto end() { return tokens.end(); }
};
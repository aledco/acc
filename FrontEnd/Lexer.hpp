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

inline std::vector<std::string> seperators = {
    ";",
    ",",
    "(",
    ")",
    "{",
    "}",
    "[",
    "]"
};

inline std::vector<std::string> operators = {
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

inline std::vector<std::string> keywords = {
    "void",
    "int", // TODO add more later
    "return"
};


inline bool vector_contains_char(std::vector<std::string>&, char c)
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
    std::size_t index = 0;
    std::vector<std::unique_ptr<Token>> tokens;

    void lex();

    std::optional<std::unique_ptr<Token>> attempt_lex_keyword();
    std::unique_ptr<Token> lex_integer();
    std::unique_ptr<Token> lex_id();
    std::unique_ptr<Token> lex_sep();
    std::unique_ptr<Token> lex_op();

    _GLIBCXX_NORETURN void error(); 

    inline bool eof() const { return index >= input.length(); }
    inline char current() const { return input[index]; }
    inline std::string slice(int length) { return input.substr(index, length); }
    inline void advance() { index++; }
    inline void advance(int i) { index += i; }

    inline bool iswhitespace() const { return std::isspace(current()); }
    inline bool isdigit() const { return std::isdigit(current()); }
    inline bool isfloat() const { return isdigit() || current() == '.'; }
    inline bool isalpha() const { return std::isalpha(current()); } 
    inline bool isalnum() const { return std::isalnum(current()); }
    inline bool isid() const { return isalnum() || current() == '_'; }
    inline bool issep() const { return vector_contains_char(seperators, current()); }
    inline bool isop() const { return vector_contains_char(operators, current()); }

    inline void add(std::unique_ptr<Token>& token) { tokens.push_back(std::move(token)); }
public:
    Lexer(const std::string& input) : input(input)
    {
        // TODO standardize input
        lex();
    };

    auto begin() { return tokens.begin(); }
    auto end() { return tokens.end(); }
    auto size() { return tokens.size(); }
};

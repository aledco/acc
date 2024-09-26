#pragma once

#include <string>
#include <vector>
#include <optional>
#include <memory>

inline std::string TokenType_Id = "ID";
inline std::string TokenType_Int = "INT";

struct Token
{
    std::string type;
    std::string value;
    Token(const std::string type, const std::string value) : type(type), value(value) {}
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
    std::vector<Token> tokens;

    void lex();

    std::optional<Token> attempt_lex_keyword();
    Token lex_integer();
    Token lex_id();
    Token lex_sep();
    Token lex_op();

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

    inline void add(Token& token) { tokens.push_back(token); }
public:
    Lexer(const std::string& input) : input(input)
    {
        // TODO standardize input
        lex();
    };

    auto begin() { return tokens.begin(); }
    auto end() { return tokens.end(); }
    std::size_t size() { return tokens.size(); }
    inline Token& operator[](int index) { return tokens[index]; }
};

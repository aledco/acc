#pragma once

#include <string>
#include <vector>
#include <optional>
#include <memory>
#include "Span.hpp"

inline std::string TokenType_Id = "ID";
inline std::string TokenType_Int = "INT";

/**
 * A token produced by lexical analysis.
 */
struct Token
{
    std::string type;
    std::string value;
    Span span;
    Token(const std::string type, const std::string value, const Span span) : type(type), value(value), span(Span(span.start, span.end)) {}
};

// The list of sperator tokens.
const inline std::vector<std::string> seperators = {
    ";",
    ",",
    "(",
    ")",
    "{",
    "}",
    "[",
    "]"
};

// The list of operator tokens.
const inline std::vector<std::string> operators = {
    "+",
    "-",
    "*",
    "/",
    "%",
    "&",
    "=",
    "!",
    "==",
    "!=",
    "<",
    "<=",
    ">",
    ">=",
    "++",
    "--",
    "+=",
    "-=",
    "*=",
    "/=",
    "%="
};

// The list of keywords.
const inline std::vector<std::string> keywords = {
    "void",
    "int", // TODO add more later
    "return",
    "extern"
};

/**
 * The lexer. Takes the source program as input and produces a vector of tokens.
 */
class Lexer
{
private:
    std::string input;
    std::size_t index = 0;
    std::vector<Token> tokens;

    /**
     * The lexer context.
     */
    struct LexerContext
    {
        Point pos;
        LexerContext(): pos(Point(1, 1)) {}
    };

    void lex();

    std::optional<Token> attempt_lex_keyword(LexerContext& context);
    Token lex_integer(LexerContext& context);
    Token lex_id(LexerContext& context);
    Token lex_sep(LexerContext& context);
    Token lex_op(LexerContext& context);
    
    inline bool eof() const { return index >= input.length(); }
    inline char current() const { return input[index]; }
    inline std::string slice(int length) { return input.substr(index, length); }
    inline void advance() { index++; }
    inline void advance(int i) { index += i; }

    inline bool isnewline() const { return current() == '\n'; }
    inline bool iswhitespace() const { return std::isspace(current()); }
    inline bool isdigit() const { return std::isdigit(current()); }
    inline bool isfloat() const { return isdigit() || current() == '.'; }
    inline bool isalpha() const { return std::isalpha(current()); } 
    inline bool isalnum() const { return std::isalnum(current()); }
    inline bool isid() const { return isalnum() || current() == '_'; }
    inline bool issep() const { return vector_contains_char(seperators, current()); }
    inline bool isop() const { return vector_contains_char(operators, current()); }

    inline void add(Token& token) { tokens.push_back(token); }
    inline Span mkSpan(LexerContext& context, std::string& value) { return Span(context.pos, context.pos + Point(0, value.length())); }

    static bool vector_contains_char(const std::vector<std::string>& vec, char c);
public:
    Lexer(const std::string& input) : input(input)
    {
        lex();
    };

    auto begin() { return tokens.begin(); }
    auto end() { return tokens.end(); }
    std::size_t size() { return tokens.size(); }
    inline Token& operator[](int index) { return tokens[index]; }
};

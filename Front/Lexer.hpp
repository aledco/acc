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
const std::vector<std::string> seperators = {
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
const std::vector<std::string> operators = {
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
const std::vector<std::string> keywords = {
    "void",
    "int",
    "if",
    "else",
    "while,",
    "return",
    "extern"
};

const std::string single_line_comment = "//";
const std::string multi_line_comment_start = "/*";
const std::string multi_line_comment_end = "*/";

/**
 * The lexer. Takes the source program as input and produces a vector of tokens.
 */
class Lexer
{
private:
    std::string input;
    std::size_t index = 0;
    std::vector<Token> tokens;

    int line = 1;
    int col = 1;

    void lex();

    std::optional<Token> attempt_lex_keyword();
    Token lex_integer();
    Token lex_id();
    Token lex_sep();
    Token lex_op();
    void lex_comment();

    void advance();
    void advance(const int n);

    inline bool eof() const { return index >= input.length(); }
    inline char current() const { return input[index]; }
    inline std::string slice(const int length) const { return input.substr(index, length); }

    inline Point pos() { return Point(line, col); }
    inline Span span(const std::string& value) { return Span(pos(), pos() + Point(0, value.length())); }

    inline bool isnewline() const { return current() == '\n'; }
    inline bool iswhitespace() const { return std::isspace(current()); }
    inline bool iscomment() const { return index < input.size() - 1 && (slice(2) == single_line_comment || slice(2) == multi_line_comment_start); }
    inline bool isdigit() const { return std::isdigit(current()); }
    inline bool isfloat() const { return isdigit() || current() == '.'; }
    inline bool isalpha() const { return std::isalpha(current()); } 
    inline bool isalnum() const { return std::isalnum(current()); }
    inline bool isid() const { return isalnum() || current() == '_'; }
    inline bool issep() const { return vector_contains_char(seperators, current()); }
    inline bool isop() const { return vector_contains_char(operators, current()); }

    inline void add(Token& token) { tokens.push_back(token); }

    static bool vector_contains_char(const std::vector<std::string>& vec, const char c);
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

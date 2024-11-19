#include <cassert>
#include <iostream>
#include <algorithm>
#include "Error.hpp"
#include "Lexer.hpp"

/**
 * Performs the lexical analysis.
 */
void Lexer::lex()
{
    while (!eof())
    {
        if (isalpha()) 
        {
            // attempt to lex a keyword
            auto token = attempt_lex_keyword();
            if (token)
            {
                add(token.value());
                continue;
            }
        }

        if (isnewline())
        {
            advance();
        }
        else if (iswhitespace()) 
        {
            advance();
        }
        else if (iscomment())
        {
            lex_comment();
        }
        else if (isdigit())
        {
            auto token = lex_integer();
            add(token);
        }
        else if (ischar())
        {
            auto token = lex_char();
            add(token);
        }
        else if (isstring())
        {
            auto token = lex_string();
            add(token);
        }
        else if (isid() && !isdigit())
        {
            auto token = lex_id();
            add(token);
        }
        else if (issep())
        {
            auto token = lex_sep();
            add(token);
        }
        else if (isop())
        {
            auto token = lex_op();
            add(token);
        }
        else 
        {
            throw SyntaxError(pos());
        }
    }
}

/**
 * Advances the input pointer by one, and updates line and column numbers.
 */
void Lexer::advance()
{
    if (eof())
    {
        return;
    }

    if (isnewline())
    {
        line++;
        col = 1;
    }
    else
    {
        col++;
    }

    index++;
}

/**
 * Advances the input pointer by one, and updates line and column numbers.
 */
void Lexer::advance(const int n)
{
    for (auto i = 0; i < n; i++)
    {
        advance();
    }
}

/**
 * Attepts to lex a keyword.
 */
std::optional<Token> Lexer::attempt_lex_keyword()
{
    for (auto keyword : keywords) 
    {
        auto n = keyword.length();
        if (index + n < input.length())
        {
            if (slice(n) == keyword)
            {
                advance(n);
                return Token(keyword, keyword, span(keyword));
            }
        }
    }

    return {};
}

/**
 * Lexes an integer.
 */
Token Lexer::lex_integer()
{
    std::string value = "";
    while (!eof() && isdigit())
    {
        value += current();
        advance();
    }

    return Token(TokenType_Int, value, span(value));
}

/**
 * Lexes a char.
 */
Token Lexer::lex_char()
{
    assert(current() == char_quote);
    advance();
    std::string value(1, current());
    advance();
    assert(current() == char_quote);
    advance();
    return Token(TokenType_Char, value, span(value));
}

/**
 * Lexes a string.
 */
Token Lexer::lex_string()
{
    assert(current() == string_quote);
    advance();

    std::string value = "";
    while (!eof() && isalpha())
    {
        value += current();
        advance();
    }

    assert(current() == string_quote);
    advance();
    return Token(TokenType_String, value, span(value));
}

/**
 * Lexes an identifier.
 */
Token Lexer::lex_id()
{
    assert(!isdigit() && "Lexer is trying to parse an id that starts with an integer");

    std::string value = "";
    while (!eof() && isid())
    {
        value += current();
        advance();
    }

    return Token(TokenType_Id, value, span( value));
}

/**
 * Lexes a seperator.
 */
Token Lexer::lex_sep()
{
    auto ismatch = [=](std::string value)
    {
        return std::find(seperators.begin(), seperators.end(), value) != seperators.end();
    };

    std::string value = "";
    while (!eof() && issep())
    {
        if (ismatch(value) && !ismatch(value + current()))
        {
            break;
        }

        value += current();
        advance();
    }

    if (std::find(seperators.begin(), seperators.end(), value) == seperators.end())
    {
        throw SyntaxError(pos());
    }

    return Token(value, value, span( value));
}

/**
 * Lexes an operator.
 */
Token Lexer::lex_op()
{
    auto ismatch = [=](std::string value)
    {
        return std::find(operators.begin(), operators.end(), value) != operators.end();
    };

    std::string value = "";
    while (!eof() && isop())
    {
        if (ismatch(value) && !ismatch(value + current()))
        {
            break;
        }

        value += current();
        advance();
    }

    if (std::find(operators.begin(), operators.end(), value) == operators.end())
    {
        throw SyntaxError(pos());
    }

    return Token(value, value, span( value));
}

/**
 * Lexes a comment.
 */
void Lexer::lex_comment()
{
    if (slice(2) == single_line_comment)
    {
        advance(2);
        while (!eof() && !isnewline())
        {
            advance();
        }
    }
    else
    {
        advance(2);
        while (!eof() && slice(2) != multi_line_comment_end)
        {
            advance();
        }

        if (eof())
        {
            throw SyntaxError(pos());
        }

        advance(2);
    }
}

/**
 * Determines if any string in the vector of strings contains a char.
 */
bool Lexer::vector_contains_char(const std::vector<std::string>& vec, const char c)
{
    for (auto s : vec)
    {
        if (s.find(c) != std::string::npos) 
        {
            return true;
        }
    }

    return false;
}

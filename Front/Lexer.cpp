#include <cassert>
#include <iostream>
#include <algorithm>
#include "Lexer.hpp"

void Lexer::lex()
{
    // TODO handle the rest of the tokens later, like floats

    LexerContext context;
    while (!eof())
    {
        if (isalpha()) 
        {
            // attempt to lex a keyword
            auto token = attempt_lex_keyword(context);
            if (token)
            {
                add(token.value());
                context.pos.col += token.value().value.length();
                continue;
            }
        }

        // TODO handle comments

        if (isnewline())
        {
            advance();
            context.pos = Point(context.pos.line + 1, 1);
        }
        else if (iswhitespace()) 
        {
            advance();
            context.pos.col += 1;
        }
        else if (isdigit())
        {
            auto token = lex_integer(context);
            context.pos.col += token.value.length();
            add(token);
        }
        else if (isid() && !isdigit())
        {
            auto token = lex_id(context);
            context.pos.col += token.value.length();
            add(token);
        }
        else if (issep())
        {
            auto token = lex_sep(context);
            context.pos.col += token.value.length();
            add(token);
        }
        else if (isop())
        {
            auto token = lex_op(context);
            context.pos.col += token.value.length();
            add(token);
        }
        else 
        {
            error(context);
        }
    }
}

std::optional<Token> Lexer::attempt_lex_keyword(LexerContext& context)
{
    for (auto keyword : keywords) 
    {
        auto n = keyword.length();
        if (index + n < input.length())
        {
            if (slice(n) == keyword)
            {
                advance(n);
                return Token(keyword, keyword, mkSpan(context, keyword));
            }
        }
    }

    return {};
}

Token Lexer::lex_integer(LexerContext& context)
{
    std::string value = "";
    while (!eof() && isdigit())
    {
        value += current();
        advance();
    }

    return Token(TokenType_Int, value, mkSpan(context, value));
}

Token Lexer::lex_id(LexerContext& context)
{
    assert(!isdigit() && "Lexer is trying to parse an id that starts with an integer");

    std::string value = "";
    while (!eof() && isid())
    {
        value += current();
        advance();
    }

    return Token(TokenType_Id, value, mkSpan(context, value));
}

Token Lexer::lex_sep(LexerContext& context)
{
    std::string value = "";
    while (!eof() && issep())
    {
        if (std::find(seperators.begin(), seperators.end(), value + current()) == seperators.end())
        {
            break;
        }

        value += current();
        advance();
    }

    return Token(value, value, mkSpan(context, value));
}

Token Lexer::lex_op(LexerContext& context)
{
    std::string value = "";
    while (!eof() && isop())
    {
        if (std::find(operators.begin(), operators.end(), value + current()) == operators.end())
        {
            break;
        }

        value += current();
        advance();
    }

    return Token(value, value, mkSpan(context, value));
}

_GLIBCXX_NORETURN void Lexer::error(LexerContext& context)
{
    std::cerr << "Syntax Error: " << context.pos.line << ":" << context.pos.col << std::endl;
    throw std::exception();
}

bool vector_contains_char(std::vector<std::string>& vec, char c)
{
    for (auto sep : vec)
    {
        if (sep.find(c) != std::string::npos) 
        {
            return true;
        }
    }

    return false;
}
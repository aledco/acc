#include <cassert>
#include <iostream>
#include <algorithm>
#include "Lexer.hpp"

void Lexer::lex()
{
    // TODO handle the rest of the tokens later, like floats
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

        // TODO handle comments

        if (iswhitespace()) 
        {
            advance();
        }
        else if (isdigit())
        {
            auto token = lex_integer();
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
            error();
        }
    }
}

std::optional<std::unique_ptr<Token>> Lexer::attempt_lex_keyword()
{
    for (auto keyword : keywords) 
    {
        if (index + keyword.length() < input.length())
        {
            if (slice(keyword.length()) == keyword)
            {
                advance(keyword.length());
                return std::make_unique<Token>(TokenType::Keyword, keyword);
            }
        }
    }

    return {};
}

std::unique_ptr<Token> Lexer::lex_integer()
{
    std::string value = "";
    while (!eof() && isdigit())
    {
        value += current();
        advance();
    }

    return std::make_unique<Token>(TokenType::Integer, value);
}

std::unique_ptr<Token> Lexer::lex_id()
{
    assert(!isdigit() && "Lexer is trying to parse an id that starts with an integer");

    std::string value = "";
    while (!eof() && isid())
    {
        value += current();
        advance();
    }

    return std::make_unique<Token>(TokenType::Id, value);
}

std::unique_ptr<Token> Lexer::lex_sep()
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

    return std::make_unique<Token>(TokenType::Sep, value);
}

std::unique_ptr<Token> Lexer::lex_op()
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

    return std::make_unique<Token>(TokenType::Op, value);
}

_GLIBCXX_NORETURN void Lexer::error()
{
    std::cerr << "Syntax Error" << std::endl; // TODO keep track of line numbers, output entire token error happend at
    std::exit(1);
} 
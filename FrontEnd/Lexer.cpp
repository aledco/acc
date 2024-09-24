#include <cassert>
#include "Lexer.hpp"

void Lexer::lex()
{
    // TODO handle the rest of the tokens later, like floats
    while (!eof())
    {
        if (isalpha()) 
        {
            // attempt to lex a keyword
            std::optional<std::unique_ptr<Token>> token = attempt_lex_keyword();
            if (token)
            {
                tokens.push_back(token.value());
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
            tokens.push_back(token);
        }
        else if (isid() && !isdigit())
        {
            auto token = lex_id();
            tokens.push_back(token);
        }
        else if (issep())
        {
            auto token = lex_sep();
            tokens.push_back(token);
        }
        else if (isop())
        {
            auto token = lex_op();
            tokens.push_back(token);
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

    return std::make_unique<Token>(Token(TokenType::Integer, value));
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

    return std::make_unique<Token>(Token(TokenType::Id, value));
}

std::unique_ptr<Token> Lexer::lex_sep()
{
    std::string value = "";
    while (!eof() && issep())
    {
        value += current();
        advance();
    }

    return std::make_unique<Token>(Token(TokenType::Sep, value));
}

std::unique_ptr<Token> Lexer::lex_op()
{
    std::string value = "";
    while (!eof() && isop())
    {
        value += current();
        advance();
    }

    return std::make_unique<Token>(Token(TokenType::Op, value));
}
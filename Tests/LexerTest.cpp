#include <iostream>
#include <vector>
#include <gtest/gtest.h>
#include "test_util.hpp"
#include "Lexer.hpp"

TEST(LexerTest, Test1)
{
    auto input = read_input(1);
    Lexer lexer(input);
    
    for (auto& token : lexer)
    {
        std::cerr << "token.value = " << token.value << std::endl;
    }

    std::vector<Token> expected = {
        Token(TokenType::Keyword, "void"),
        Token(TokenType::Id, "main"),
        Token(TokenType::Sep, "("),
        Token(TokenType::Sep, ")"),
        Token(TokenType::Sep, "{"),
        Token(TokenType::Keyword, "return"),
        Token(TokenType::Integer, "0"),
        Token(TokenType::Sep, ";"),
        Token(TokenType::Sep, "}"),
    };

    EXPECT_EQ(expected.size(), lexer.size());

    int i = 0;
    for (auto& token : lexer)
    {
        EXPECT_EQ(expected[i].type, token.type);
        EXPECT_EQ(expected[i].value, token.value);
        i++;
    }
}

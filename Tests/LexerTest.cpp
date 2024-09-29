#include <iostream>
#include <vector>
#include <gtest/gtest.h>
#include "test_util.hpp"
#include "Lexer.hpp"

static void compare(Lexer& lexer, std::vector<Token>& expected)
{
    EXPECT_EQ(expected.size(), lexer.size());

    int i = 0;
    for (auto& token : lexer)
    {
        EXPECT_EQ(expected[i].type, token.type);
        EXPECT_EQ(expected[i].value, token.value);
        i++;
    }
}

TEST(Lexer, Test1)
{
    auto input = read_input(1);
    Lexer lexer(input);
    
    std::vector<Token> expected = {
        Token("int", "int"),
        Token(TokenType_Id, "main"),
        Token("(", "("),
        Token(")", ")"),
        Token("{", "{"),
        Token("return", "return"),
        Token(TokenType_Int, "0"),
        Token(";", ";"),
        Token("}", "}"),
    };

    compare(lexer, expected);
}

TEST(Lexer, Test2)
{
    auto input = read_input(2);
    Lexer lexer(input);
    
    std::vector<Token> expected = {
        Token("int", "int"),
        Token(TokenType_Id, "main"),
        Token("(", "("),
        Token(")", ")"),
        Token("{", "{"),
        Token("return", "return"),
        Token(TokenType_Int, "1"),
        Token(";", ";"),
        Token("}", "}"),
    };

    compare(lexer, expected);
}

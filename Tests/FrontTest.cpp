#include <iostream>
#include <vector>
#include <gtest/gtest.h>
#include "TestUtils.hpp"
#include "Environment.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"

TEST(Front, Lexer)
{
    for (auto& input : all_inputs)
    {
        EXPECT_NO_THROW({
            Lexer lexer(input);
        });
    }
}

TEST(Front, Parser)
{
    for (auto& input : all_inputs)
    {
        Lexer lexer(input);
        EXPECT_NO_THROW({
            Parser parser(lexer);
            auto program = parser.parse();
        });
    }
}

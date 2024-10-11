#include <iostream>
#include <vector>
#include <tuple>
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
            Lexer lexer(input.content);
        }) << "Error for test " << input.test_id;
    }
}

TEST(Front, Parser)
{
    for (auto& input : all_inputs)
    {
        Lexer lexer(input.content);
        EXPECT_NO_THROW({
            Parser parser(lexer);
            auto program = parser.parse();
        }) << "Error for test " << input.test_id;
    }
}

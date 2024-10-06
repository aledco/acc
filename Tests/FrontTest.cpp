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
            Lexer lexer(std::get<1>(input));
        }) << "Error for test " << std::get<0>(input);
    }
}

TEST(Front, Parser)
{
    for (auto& input : all_inputs)
    {
        Lexer lexer(std::get<1>(input));
        EXPECT_NO_THROW({
            Parser parser(lexer);
            auto program = parser.parse();
        }) << "Error for test " << std::get<0>(input);
    }
}

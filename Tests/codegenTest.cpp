#include <iostream>
#include <vector>
#include <gtest/gtest.h>
#include "test_util.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"

TEST(Parser2, Test2)
{
    auto input = read_input(1);
    Lexer lexer(input);
    Parser parser(lexer);
    auto program = parser.parse();

    program->codegen();

    EXPECT_TRUE(true);
}

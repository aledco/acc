#include <iostream>
#include <vector>
#include <gtest/gtest.h>
#include "test_util.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"

static void NormalExit()
{

}

TEST(Front, Lexer)
{
    auto input = read_input(1);
    Lexer lexer(input); // TODO need a way to test that no errors happened
    EXPECT_NEQ(lexer.size(), 0);
}

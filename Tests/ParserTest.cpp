#include <iostream>
#include <vector>
#include <gtest/gtest.h>
#include "test_util.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "SyntaxTree.hpp"

TEST(Parser, Test1)
{
    auto input = read_input(1);
    Lexer lexer(input);
    Parser parser(lexer);
    auto program = parser.parse();
    
    EXPECT_EQ(program->functions.size(), 1);
    
    auto func = program->functions[0].get();
    EXPECT_EQ(func->function->name, "main");
    EXPECT_EQ(func->body->statements.size(), 1);

    auto statement = func->body->statements[0].get();
    auto return_stmt = dynamic_cast<Return *>(statement);
    EXPECT_NE(return_stmt, nullptr);
    EXPECT_NE(return_stmt->expr, nullptr);

    auto int_const = dynamic_cast<IntegerConstant *>(return_stmt->expr.get());
    EXPECT_NE(int_const, nullptr);
    EXPECT_EQ(int_const->value, 0);
}

TEST(Parser, Test2)
{
    auto input = read_input(2);
    Lexer lexer(input);
    Parser parser(lexer);
    auto program = parser.parse();
    
    EXPECT_EQ(program->functions.size(), 1);
    
    auto func = program->functions[0].get();
    EXPECT_EQ(func->function->name, "main");
    EXPECT_EQ(func->body->statements.size(), 1);

    auto statement = func->body->statements[0].get();
    auto return_stmt = dynamic_cast<Return *>(statement);
    EXPECT_NE(return_stmt, nullptr);
    EXPECT_NE(return_stmt->expr, nullptr);

    auto int_const = dynamic_cast<IntegerConstant *>(return_stmt->expr.get());
    EXPECT_NE(int_const, nullptr);
    EXPECT_EQ(int_const->value, 1);
}

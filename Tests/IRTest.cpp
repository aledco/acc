#include <iostream>
#include <vector>
#include <gtest/gtest.h>
#include "TestUtils.hpp"
#include "Environment.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Quad.hpp"

TEST(IR, Codegen)
{
    for (auto& input : all_inputs)
    {
        Lexer lexer(input.content);
        Parser parser(lexer);
        auto program = parser.parse();
        EXPECT_NO_THROW({
            program->ir_codegen();
        }) << "Error for test " << input.test_id;

        EXPECT_FALSE(program->ir_list.empty());
    }
}

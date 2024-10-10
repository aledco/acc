#include <iostream>
#include <vector>
#include <gtest/gtest.h>
#include "TestUtils.hpp"
#include "Environment.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Codegen.hpp"

TEST(Codegen, All)
{
    for (auto& input : all_inputs)
    {
        Lexer lexer(std::get<1>(input));
        Parser parser(lexer);
        auto program = parser.parse();

        EXPECT_NO_THROW({
            codegen(program);
        }) << "Error for test " << std::get<0>(input);

        // TODO compile input with clang, compare outputs
    }
}

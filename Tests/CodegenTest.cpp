#include <iostream>
#include <vector>
#include <stdlib.h>
#include <gtest/gtest.h>
#include "TestUtils.hpp"
#include "Environment.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Codegen.hpp"

static void execute(int test_id, std::string cmd, bool check_status = true)
{
    auto ec = system(cmd.c_str());
    if (check_status)
    {
        EXPECT_EQ(ec, 0) << "command: " << cmd << " failed to execute for test " << test_id;
    }
}

TEST(Codegen, All)
{
    for (auto& input : all_inputs)
    {
        Lexer lexer(input.content);
        Parser parser(lexer);
        auto program = parser.parse();
        program->ir_codegen();

        auto output_files = get_output_files(input.test_id);
        {
            auto ll_file = std::ofstream(output_files.ll_filepath);
            EXPECT_NO_THROW({
                codegen(program, &ll_file);
            }) << "Error for test " << input.test_id;
        }
        
        // link the LLVM files into an executable
        execute(input.test_id, "llc-18 " + output_files.ll_filepath + " -o " + output_files.s_filepath);
        execute(input.test_id, "as " + output_files.s_filepath + " -o " + output_files.obj_filepath);
        execute(input.test_id, "gcc " + output_files.obj_filepath + " " + test_code_path + " -o " + output_files.exe_filepath + " -no-pie");
        execute(input.test_id, "chmod u+x " + output_files.exe_filepath);

        // run the generated executable
        execute(input.test_id, "./" + output_files.exe_filepath + " > " + output_files.out_filepath + " ; echo exit status: $? >> " + output_files.out_filepath, false);

        // link the C files using clang to compare output
        execute(input.test_id, "clang-18 -c Tests/test-link/print.c -o Tests/test-link/print.o");
        execute(input.test_id, "clang-18 " + input.filepath + " Tests/test-link/print.o -o " + output_files.ref_exe_filepath);
        execute(input.test_id, "./" + output_files.ref_exe_filepath + " > " + output_files.ref_out_filepath + " ; echo exit status: $? >> " + output_files.ref_out_filepath, false);

        execute(input.test_id, "diff " + output_files.ref_out_filepath + " " + output_files.out_filepath + " > " + output_files.diff_filepath);

        std::ifstream diff(output_files.diff_filepath);
        EXPECT_TRUE(diff.good()) << "failed to create diff for test " << input.test_id;

        std::stringstream buffer;
        buffer << diff.rdbuf();
        auto diff_content = buffer.str();
        EXPECT_TRUE(diff_content.empty()) << "actual output did not match expected output for test " << input.test_id;
    }
}

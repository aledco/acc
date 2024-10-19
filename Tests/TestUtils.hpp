#pragma once

#include <gtest/gtest.h>
#include <string>
#include <fstream>
#include <vector>

#ifdef __INTELLISENSE__
    #pragma diag_suppress 20
#endif

const inline std::string test_code_path = "acc-link/test-code.ll"; 

struct Input
{
    int test_id;
    std::string filepath;
    std::string content;
    Input(int test_id, std::string filepath, std::string content) : test_id(test_id), filepath(filepath), content(content) {}
};

struct OutputFiles
{
    std::string ll_filepath;
    std::string s_filepath;
    std::string exe_filepath;
    std::string out_filepath;
    std::string ref_exe_filepath;
    std::string ref_out_filepath;
    std::string diff_filepath;
    OutputFiles(std::string outdir) :
        ll_filepath(outdir + "/act.ll"),
        s_filepath(outdir + "/act.s"),
        exe_filepath(outdir + "/act.exe"),
        out_filepath(outdir + "/act.out"),
        ref_exe_filepath(outdir + "/ref.exe"),
        ref_out_filepath(outdir + "/ref.out"),
        diff_filepath(outdir + "/diff.out")
    {}
};

Input read_input(int test_id);
std::vector<Input> read_all_inputs();

void clear_outdir();
OutputFiles get_output_files(int test_id);

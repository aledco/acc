#pragma once

#include <vector>
#include <llvm/IR/Value.h>
#include <llvm/IR/BasicBlock.h>

struct CodegenResult
{
    llvm::Value *value;
    CodegenResult() : value(nullptr) {};
    CodegenResult(llvm::Value *value) : value(value) {};
};
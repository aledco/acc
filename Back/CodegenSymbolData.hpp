#pragma once

#include <llvm/IR/Value.h>

struct CodegenSymbolData
{
    llvm::Value *value = nullptr;
    inline bool is_allocated() { return value != nullptr; }
};
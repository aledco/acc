#pragma once

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <memory>
#include <map>
#include "Quad.hpp"

#ifdef __INTELLISENSE__
    #pragma diag_suppress 135
    #pragma diag_suppress 283
    #pragma diag_suppress 77
    #pragma diag_suppress 65
    #pragma diag_suppress 439
    #pragma diag_suppress 40
#endif

/**
 * The codegen context.
 */
struct CodegenContext
{
    std::unique_ptr<llvm::LLVMContext> llvm_context;
    std::unique_ptr<llvm::IRBuilder<>> llvm_builder;
    std::unique_ptr<llvm::Module> llvm_module;

    llvm::Function *llvm_function;
    llvm::BasicBlock *llvm_block;
    std::map<std::shared_ptr<Operand>, llvm::BasicBlock *> block_map;
    std::vector<llvm::Value *> param_stack;
    CodegenContext();
};
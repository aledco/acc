#include "CodegenContext.hpp"

CodegenContext::CodegenContext()
{
    llvm_context = std::make_unique<llvm::LLVMContext>();
    llvm_builder = std::make_unique<llvm::IRBuilder<>>(*llvm_context);
    llvm_module = std::make_unique<llvm::Module>("main_module", *llvm_context);
}
#include <vector>
#include <memory>
#include <string>
#include <llvm/ADT/APInt.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/Module.h>
#include "Type.hpp"
#include "CodegenContext.hpp"
#include "Codegen.hpp"

// disable VSCode errors for LLVM code
#ifdef __INTELLISENSE__
    #pragma diag_suppress 20
    #pragma diag_suppress 70
    #pragma diag_suppress 276
    #pragma diag_suppress 757
#endif

static llvm::Type *get_llvm_type(std::shared_ptr<Type> type, CodegenContext& context)
{
    switch (type->type)
    {
        case TypeType::Void:
            return llvm::Type::getVoidTy(*context.llvm_context);
        case TypeType::Int:
            return llvm::Type::getInt32Ty(*context.llvm_context);
        default:
            return nullptr; // TODO handle later
    }
}

static llvm::Value *codegen(std::shared_ptr<Quad> quad, CodegenContext& context)
{
    return nullptr; // TODO
}

static llvm::Value *codegen(std::shared_ptr<BasicBlock> block, int bn, CodegenContext& context)
{
    auto llvm_block = llvm::BasicBlock::Create(*context.llvm_context, "block" + std::to_string(bn), context.llvm_function);
    context.llvm_builder->SetInsertPoint(llvm_block);
    
    for (auto quad = block->qlist.get_head(); quad != nullptr; quad = quad->next)
    {
        auto value = codegen(quad, context);
    }

    return nullptr; // TODO need to figure out how to link the values
}

static llvm::Value *codegen(std::shared_ptr<FunctionDef> def, CodegenContext& context)
{
    auto llvm_function = context.llvm_module->getFunction(def->function->name);
    if (llvm_function == nullptr)
    {
        std::vector<llvm::Type *> llvm_param_types;
        for (auto& param : def->params)
        {
            llvm_param_types.push_back(get_llvm_type(param->type, context));
        }

        auto llvm_return_type = get_llvm_type(def->function->type->ret_type, context);
        auto llvm_function_type = llvm::FunctionType::get(llvm_return_type, llvm_param_types, false);
        llvm_function = llvm::Function::Create(llvm_function_type, llvm::Function::ExternalLinkage, def->function->name, context.llvm_module.get());

        // set the names for the args
        std::size_t i = 0;
        for (auto &arg : llvm_function->args())
        {
            arg.setName(def->params[i]->name);
        } 
    }

    // record all the args in the named_values map
    context.named_values.clear();
    for (auto &arg : llvm_function->args())
    {
        context.named_values[std::string(arg.getName())] = &arg;
    }

    context.llvm_function = llvm_function;

    for (auto i =- 0; i < def->cfg.size(); i++)
    {
        auto block_value = codegen(def->cfg[i], i, context);
        // TODO need to figure out how to link the values
    }

    llvm::verifyFunction(*llvm_function);
    return llvm_function;
}

void codegen(std::shared_ptr<Program> program)
{
    CodegenContext context;
    for (auto function : program->functions)
    {
        auto llvm_function = codegen(function, context);
    }

    context.llvm_module->dump();
}
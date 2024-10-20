#include <vector>
#include <array>
#include <memory>
#include <string>
#include <iostream>
#include <cassert>
#include <llvm/ADT/APInt.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/raw_os_ostream.h>
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

/**
 * Gets the LLVM type.
 */
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

/**
 * Create an LLVM block for the IR block.
 */
static llvm::BasicBlock *create_llvm_block(std::shared_ptr<BasicBlock> block, int bn, CodegenContext& context)
{
    auto llvm_block = llvm::BasicBlock::Create(*context.llvm_context, "block_" + context.llvm_function->getName() + std::to_string(bn), context.llvm_function);
    if (block->qlist.get_head()->op == QuadOp::Label)
    {
        context.block_map[block->qlist.get_head()->arg1] = llvm_block;
    }

    return llvm_block;
}

/**
 * Allocates a variable on the stack.
 */
static void allocate(std::shared_ptr<Operand> operand, CodegenContext& context)
{
    assert(operand->type == OperandType::Variable);

    if (operand->symbol->is_temp || operand->symbol->is_parameter || operand->symbol->symbol_data.is_allocated())
    {
        return;
    }
    else
    {
        operand->symbol->symbol_data.value = context.llvm_builder->CreateAlloca(
            get_llvm_type(operand->symbol->type, context), 
            nullptr, 
            operand->symbol->name);
    }
}

/**
 * Stores a value in a variable.
 */
static void store(std::shared_ptr<Operand> operand, llvm::Value *val, CodegenContext& context)
{
    assert(operand->type == OperandType::Variable);

    if (operand->symbol->is_temp || operand->symbol->is_parameter)
    {
        operand->symbol->symbol_data.value = val;
    }
    else
    {   
        context.llvm_builder->CreateStore(val, operand->symbol->symbol_data.value, false);
    }
}

/**
 * Generates LLVM code for an operand.
 */
static llvm::Value *codegen(std::shared_ptr<Operand> operand, CodegenContext& context)
{
    switch (operand->type)
    {
        case OperandType::IntConst:
            return llvm::ConstantInt::get(*context.llvm_context, llvm::APInt(32, operand->iconst));
        case OperandType::StrConst:
            return nullptr; // TODO
        case OperandType::Variable:
            if (operand->symbol->is_temp || operand->symbol->is_parameter)
            {
                return operand->symbol->symbol_data.value;
            }
            else
            {
                if (!operand->symbol->symbol_data.is_allocated())
                {
                    allocate(operand, context); // TODO this will have the effect of allocating a variable on its first use, do we want that?
                }
                
                return context.llvm_builder->CreateLoad(
                    get_llvm_type(operand->symbol->type, context), 
                    operand->symbol->symbol_data.value);
            }
        case OperandType::Label:
            return nullptr; // TODO
    }
}

/**
 * Generates LLVM code for a param instruction.
 */
static llvm::Value *codegen_param(std::shared_ptr<Quad> quad, CodegenContext& context)
{
    auto value = codegen(quad->arg1, context);
    context.param_stack.push_back(value);
    return value;
}


/**
 * Generates LLVM code for a call instruction.
 */
static llvm::Value *codegen_call(std::shared_ptr<Quad> quad, CodegenContext& context)
{
    assert(quad->arg1->type == OperandType::Variable);
    assert(quad->arg2->type == OperandType::IntConst);

    allocate(quad->res, context);

    auto func_name = quad->arg1->symbol->name;
    auto func = context.llvm_module->getFunction(func_name);
    assert(func != nullptr);

    auto nargs = quad->arg2->iconst;
    llvm::SmallVector<llvm::Value *> args(nargs);
    for (int i = nargs - 1; i >= 0; i--)
    {
        args[i] = context.param_stack.back();
        context.param_stack.pop_back();
    }

    auto func_call = context.llvm_builder->CreateCall(func, args);
    store(quad->res, func_call, context);
    return func_call;
}

/**
 * Generates LLVM code for a binary instruction.
 */
static llvm::Value *codegen_binop(std::shared_ptr<Quad> quad, CodegenContext& context)
{
    assert(quad->res->type == OperandType::Variable);

    auto arg1 = codegen(quad->arg1, context);
    auto arg2 = codegen(quad->arg2, context);
    allocate(quad->res, context);

    llvm::Value *res;
    switch (quad->op)
    {
        case QuadOp::Add:
            res = context.llvm_builder->CreateAdd(arg1, arg2, quad->res->symbol->name);
            break;
        case QuadOp::Sub:
            res = context.llvm_builder->CreateSub(arg1, arg2, quad->res->symbol->name);
            break;
        case QuadOp::Mul:
            res = context.llvm_builder->CreateMul(arg1, arg2, quad->res->symbol->name);
            break;
        case QuadOp::Div:
            // TODO if types are unsigned, need to use UDiv
            res = context.llvm_builder->CreateSDiv(arg1, arg2, quad->res->symbol->name);
            break;
        case QuadOp::Mod:
            // TODO if types are unsigned, need to use URem
            res = context.llvm_builder->CreateSRem(arg1, arg2, quad->res->symbol->name);
            break;
        default:
            break;
    }

    store(quad->res, res, context);
    return res;
}

/**
 * Generates LLVM code for an unary instruction.
 */
static llvm::Value *codegen_unop(std::shared_ptr<Quad> quad, CodegenContext& context)
{
    assert(quad->res->type == OperandType::Variable);

    auto arg1 = codegen(quad->arg1, context);
    allocate(quad->res, context);

    llvm::Value *res;
    switch (quad->op)
    {
        case QuadOp::Neg:
            res = context.llvm_builder->CreateNeg(arg1);
            break;
        case QuadOp::Deref:
        case QuadOp::Addr:
            break; // TODO
        case QuadOp::Copy:
            res = arg1;
            break;
        default:
            break;
    }

    store(quad->res, res, context);
    return res;
}

/**
 * Generates LLVM code for a return instruction.
 */
static llvm::Value *codegen_return(std::shared_ptr<Quad> quad, CodegenContext& context)
{
    if (quad->arg1 == nullptr)
    {
        return context.llvm_builder->CreateRetVoid();
    }
    else
    {
        auto arg1 = codegen(quad->arg1, context);
        return context.llvm_builder->CreateRet(arg1);
    }
}

/**
 * Generates LLVM code for a goto instruction.
 */
static llvm::Value *codegen_goto(std::shared_ptr<Quad> quad, CodegenContext& context)
{
    assert(context.block_map.find(quad->arg1) != context.block_map.end());
    auto block = context.block_map[quad->arg1];
    return context.llvm_builder->CreateBr(block);
}

/**
 * Generates LLVM code for an if instruction.
 */
static llvm::Value *codegen_if(std::shared_ptr<Quad> quad, CodegenContext& context)
{
    auto arg1 = codegen(quad->arg1, context);
    auto arg2 = codegen(quad->arg2, context);

    llvm::Value *cond;
    switch (quad->op)
    {
        case QuadOp::IfEq:
            cond = context.llvm_builder->CreateCmp(llvm::CmpInst::Predicate::ICMP_EQ, arg1, arg2);
            break;
        case QuadOp::IfNeq:
            cond = context.llvm_builder->CreateCmp(llvm::CmpInst::Predicate::ICMP_NE, arg1, arg2);
            break;
        case QuadOp::IfLt:
            cond = context.llvm_builder->CreateCmp(llvm::CmpInst::Predicate::ICMP_SLT, arg1, arg2);
            break;
        case QuadOp::IfLeq:
            cond = context.llvm_builder->CreateCmp(llvm::CmpInst::Predicate::ICMP_SLE, arg1, arg2);
            break;
        case QuadOp::IfGt:
            cond = context.llvm_builder->CreateCmp(llvm::CmpInst::Predicate::ICMP_SGT, arg1, arg2);
            break;
        case QuadOp::IfGeq:
            cond = context.llvm_builder->CreateCmp(llvm::CmpInst::Predicate::ICMP_SGE, arg1, arg2);
            break;
        default:
            assert(false && "shouldn't get here");
    }

    auto true_block = context.block_map[quad->res];
    auto false_block = context.llvm_builder->GetInsertBlock()->getNextNode();
    return context.llvm_builder->CreateCondBr(cond, true_block, false_block);
}

/**
 * Generates LLVM code for an instruction.
 */
static llvm::Value *codegen(std::shared_ptr<Quad> quad, CodegenContext& context)
{
    switch (quad->op)
    {
        case QuadOp::Global:
        case QuadOp::String:
            break; // TODO
        case QuadOp::Add:
        case QuadOp::Sub:
        case QuadOp::Mul:
        case QuadOp::Div:
        case QuadOp::Mod:
            return codegen_binop(quad, context);
        case QuadOp::Neg:
        case QuadOp::Deref:
        case QuadOp::Addr:
        case QuadOp::Copy:
            return codegen_unop(quad, context);
        case QuadOp::LIndex:
        case QuadOp::RIndex:
            break; // TODO
        case QuadOp::Goto:
            return codegen_goto(quad, context);
        case QuadOp::IfEq:
        case QuadOp::IfNeq:
        case QuadOp::IfLt:
        case QuadOp::IfLeq:
        case QuadOp::IfGt:
        case QuadOp::IfGeq:
            return codegen_if(quad, context);
        case QuadOp::Return:
            return codegen_return(quad, context);
        case QuadOp::Param:
            return codegen_param(quad, context);
        case QuadOp::Call:
            return codegen_call(quad, context);
        case QuadOp::Enter:
        case QuadOp::Label:
            break;
        default:
            break;
    }

    return nullptr; // TODO
}

/**
 * Generates LLVM code for a basic block.
 */
static llvm::Value *codegen(std::shared_ptr<BasicBlock> block, CodegenContext& context)
{
    context.llvm_builder->SetInsertPoint(context.llvm_block);
    for (auto quad = block->qlist.get_head(); quad != nullptr; quad = quad->next)
    {
        codegen(quad, context);
    }

    return context.llvm_block;
}

/**
 * Generates LLVM code for a function prototype.
 */
static llvm::Function *codegen_prototype(std::shared_ptr<FunctionDef> def, CodegenContext& context)
{
    std::vector<llvm::Type *> llvm_param_types;
    for (auto& param : def->params)
    {
        llvm_param_types.push_back(get_llvm_type(param->type, context));
    }

    auto llvm_return_type = get_llvm_type(def->function->type->ret_type, context);
    auto llvm_function_type = llvm::FunctionType::get(llvm_return_type, llvm_param_types, false);
    auto llvm_function = llvm::Function::Create(llvm_function_type, llvm::Function::ExternalLinkage, def->function->name, context.llvm_module.get());

    // set the names for the args
    std::size_t i = 0;
    for (auto &arg : llvm_function->args())
    {
        arg.setName(def->params[i]->name);
        i++;
    }

    return llvm_function;
}

/**
 * Generates LLVM code for a function.
 */
static llvm::Function *codegen(std::shared_ptr<FunctionDef> def, CodegenContext& context)
{
    if (def->is_proto())
    {
        return codegen_prototype(def, context);
    }

    auto llvm_function = context.llvm_module->getFunction(def->function->name);
    if (llvm_function == nullptr)
    {
        llvm_function = codegen_prototype(def, context); 
    }

    // set the values for the args
    std::size_t i = 0;
    for (auto &arg : llvm_function->args())
    {
        def->params[i]->symbol_data.value = &arg;
        i++;
    }

    context.llvm_function = llvm_function;

    // create an LLVM block for each block in the CFG
    std::vector<llvm::BasicBlock *> llvm_blocks;
    for (auto i =- 0; i < def->cfg.size(); i++)
    {
        auto block = create_llvm_block(def->cfg[i], i, context);
        llvm_blocks.push_back(block);
    }

    assert(def->cfg.size() == llvm_blocks.size());

    // generate LLVM code for each block in the CFG
    for (auto i =- 0; i < def->cfg.size(); i++)
    {
        context.llvm_block = llvm_blocks[i];
        codegen(def->cfg[i], context);
    }

    llvm::verifyFunction(*llvm_function);
    return llvm_function;
}

/**
 * Generates LLVM code for the program.
 */
void codegen(std::shared_ptr<Program> program, std::ostream *file)
{
    CodegenContext context;
    for (auto function : program->functions)
    {
        auto llvm_function = codegen(function, context);
    }

    llvm::raw_os_ostream stream(*file);
    context.llvm_module->print(stream, nullptr);
}

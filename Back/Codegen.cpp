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
        case TypeType::Char:
            return llvm::Type::getInt8Ty(*context.llvm_context);
        case TypeType::Array:
        {
            auto elem_type = get_llvm_type(type->elem_type, context);
            if (type->num_elems)
            {
                return llvm::ArrayType::get(elem_type, type->num_elems.value());
            }
            else
            {
                return elem_type->getPointerTo();
            }
        }
        case TypeType::Pointer:
        {
            auto elem_type = get_llvm_type(type->elem_type, context);
            return elem_type->getPointerTo();
        }
        default:
            return nullptr; // TODO handle later
    }
}

static llvm::Constant *get_default_value(std::shared_ptr<Type> type, CodegenContext& context)
{
    switch (type->type) 
    {
        case TypeType::Void:
            return nullptr;
        case TypeType::Int:
            return context.llvm_builder->getInt32(0);
        case TypeType::Char:
            return context.llvm_builder->getInt8(0);
        case TypeType::Function:
        case TypeType::Array:
        case TypeType::Pointer:
            return nullptr;
    }
}

/**
 * Create an LLVM block for the IR block.
 */
static llvm::BasicBlock *create_llvm_block(std::shared_ptr<BasicBlock> block, int bn, CodegenContext& context)
{
    auto llvm_block = llvm::BasicBlock::Create(*context.llvm_context, "block_" + context.llvm_function->getName() + std::to_string(bn), context.llvm_function);
    context.block_map[block->qlist.get_head()->arg1] = llvm_block;
    return llvm_block;
}

/**
 * Determines if the quad instruction is an LLVM terminator instruction.
 */
static bool is_terminator(std::shared_ptr<Quad> quad)
{
    switch (quad->op)
    {
        case QuadOp::Goto:
        case QuadOp::IfEq:
        case QuadOp::IfNeq:
        case QuadOp::IfLt:
        case QuadOp::IfLeq:
        case QuadOp::IfGt:
        case QuadOp::IfGeq:
        case QuadOp::Return:
            return true;
        default:
            return false;
    }
}

/**
 * Stores a value in a variable.
 */
static void store(std::shared_ptr<Symbol> symbol, llvm::Value *val, CodegenContext& context)
{
    if (symbol->type->type == TypeType::Array)
    {
        symbol->symbol_data.value = val;
    }
    else
    {
        if (symbol->is_temp)
        {
            symbol->symbol_data.value = val;
        }
        else
        {   
            context.llvm_builder->CreateStore(val, symbol->symbol_data.value, false);
        }
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
            return context.llvm_builder->getIntN(operand->iconst.nbytes * 8, operand->iconst.value);
        case OperandType::StrConst:
            return nullptr;
        case OperandType::Variable:
            if (operand->symbol->type->type == TypeType::Array)
            {
                return operand->symbol->symbol_data.value;
            }
            else
            {
                if (operand->symbol->is_temp)
                {
                    return operand->symbol->symbol_data.value;
                }
                else
                {
                    return context.llvm_builder->CreateLoad(
                        get_llvm_type(operand->symbol->type, context), 
                        operand->symbol->symbol_data.value);
                }
            }
        case OperandType::Label:
        case OperandType::ExprType:
            return nullptr;
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

    auto func_name = quad->arg1->symbol->get_name();
    auto func = context.llvm_module->getFunction(func_name);
    assert(func != nullptr);

    auto nargs = quad->arg2->iconst;
    llvm::SmallVector<llvm::Value *> args(nargs.value);
    for (int i = nargs.value - 1; i >= 0; i--)
    {
        args[i] = context.param_stack.back();
        context.param_stack.pop_back();
    }

    auto func_call = context.llvm_builder->CreateCall(func, args);
    store(quad->res->symbol, func_call, context);
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

    llvm::Value *res;
    switch (quad->op)
    {
        case QuadOp::Add:
            res = context.llvm_builder->CreateAdd(arg1, arg2);
            break;
        case QuadOp::Sub:
            res = context.llvm_builder->CreateSub(arg1, arg2);
            break;
        case QuadOp::Mul:
            res = context.llvm_builder->CreateMul(arg1, arg2);
            break;
        case QuadOp::Div:
            // TODO if types are unsigned, need to use UDiv
            res = context.llvm_builder->CreateSDiv(arg1, arg2);
            break;
        case QuadOp::Mod:
            // TODO if types are unsigned, need to use URem
            res = context.llvm_builder->CreateSRem(arg1, arg2);
            break;
        default:
            break;
    }

    store(quad->res->symbol, res, context);
    return res;
}

/**
 * Generates LLVM code for an unary instruction.
 */
static llvm::Value *codegen_unop(std::shared_ptr<Quad> quad, CodegenContext& context)
{
    assert(quad->res->type == OperandType::Variable);

    auto arg1 = codegen(quad->arg1, context);

    llvm::Value *res;
    switch (quad->op)
    {
        case QuadOp::Neg:
            res = context.llvm_builder->CreateNeg(arg1);
            break;
        case QuadOp::RDeref:
            assert(quad->arg1->type == OperandType::Variable);
            res = context.llvm_builder->CreateLoad(get_llvm_type(quad->arg1->symbol->type->elem_type, context), arg1);
            break;
        case QuadOp::AddrOf:
            break; // TODO
        case QuadOp::Copy:
            if (quad->res->symbol->type->type == TypeType::Array && quad->res->symbol->type->num_elems)
            {
                auto nelems = quad->res->symbol->type->num_elems.value();
                auto elem_size = quad->res->symbol->type->elem_type->size();
                auto mem_size = nelems * elem_size;
                res = codegen(quad->res, context);
                context.llvm_builder->CreateMemCpy(res, {}, arg1, {}, mem_size);
            }
            else
            {
                res = arg1;
            }
            
            res = arg1;
            break;
        default:
            break;
    }

    store(quad->res->symbol, res, context);
    return res;
}

/**
 * Generates LLVM code for a left deref instruction.
 */
static llvm::Value *codegen_lderef(std::shared_ptr<Quad> quad, CodegenContext& context)
{
    auto arg1 = codegen(quad->arg1, context);
    auto res = codegen(quad->res, context);
    return context.llvm_builder->CreateStore(arg1, res);
}

/**
 * Generates LLVM code for an add pointer instruction.
 */
static llvm::Value *codegen_addptr(std::shared_ptr<Quad> quad, CodegenContext& context)
{
    auto arg1 = codegen(quad->arg1, context);
    auto arg2 = codegen(quad->arg2, context);
    auto element_size = quad->arg1->symbol->type->elem_type->size();
    llvm::Value *offset;
    if (element_size > 1)
    {
        auto multiplier = llvm::ConstantInt::get(arg2->getType(), element_size);
        offset = context.llvm_builder->CreateMul(arg2, multiplier);
    }
    else
    {
        offset = arg2;
    }
    
    auto res = context.llvm_builder->CreatePtrAdd(arg1, offset);
    store(quad->res->symbol, res, context);
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
    auto false_block = context.llvm_block->getNextNode();
    return context.llvm_builder->CreateCondBr(cond, true_block, false_block);
}

/**
 * Generates LLVM code for a cast instruction.
 */
static llvm::Value *codegen_cast(std::shared_ptr<Quad> quad, CodegenContext& context)
{
    assert(quad->arg2->type == OperandType::ExprType);
    
    auto arg1 = codegen(quad->arg1, context);
    auto llvm_to_type = get_llvm_type(quad->arg2->expr_type, context);

    llvm::Value *res;
    if (llvm_to_type->isIntegerTy() && arg1->getType()->isIntegerTy())
    {
        res = context.llvm_builder->CreateIntCast(arg1, llvm_to_type, true); // TODO if signed types are added need to adjust
    }
    else if (llvm_to_type->isIntegerTy() && arg1->getType()->isPointerTy())
    {
        res = context.llvm_builder->CreateCast(llvm::Instruction::CastOps::PtrToInt, arg1, llvm_to_type);
    }
    else if (llvm_to_type->isPointerTy() && arg1->getType()->isIntegerTy())
    {
        res = context.llvm_builder->CreateCast(llvm::Instruction::CastOps::IntToPtr, arg1, llvm_to_type);
    }
    else if (llvm_to_type->isPointerTy() && arg1->getType()->isPointerTy())
    {
        res = context.llvm_builder->CreateCast(llvm::Instruction::CastOps::BitCast, arg1, llvm_to_type);
    }
    else if (llvm_to_type->isPointerTy() && arg1->getType()->isArrayTy())
    {
        auto elem_type = get_llvm_type(quad->arg1->symbol->type->elem_type, context);
        res = context.llvm_builder->CreateGEP(elem_type, arg1, context.llvm_builder->getInt32(0));
    }
    else
    {
        arg1->getType()->dump();
        llvm_to_type->dump();
        assert(false && "unsupported cast");
    }
    
    store(quad->res->symbol, res, context);
    return res;
}

/**
 * Generates LLVM code for a string instruction.
 */
static llvm::Value *codegen_string(std::shared_ptr<Quad> quad, CodegenContext& context)
{
    // TODO need to do a builder->CreateMemCpy, look at test.ll for reference
    assert(quad->arg1->type == OperandType::StrConst);
    auto res = context.llvm_builder->CreateGlobalString(quad->arg1->strconst);
    store(quad->res->symbol, res, context);
    return res;
}

/**
 * Generates LLVM code for a global variable.
 */
static llvm::Value *codegen_global(std::shared_ptr<Quad> quad, CodegenContext& context)
{
    assert(quad->arg1->type == OperandType::Variable);
    auto llvm_global = new llvm::GlobalVariable(
        *context.llvm_module.get(),
        get_llvm_type(quad->arg1->symbol->type, context),
        false /* isConstant */, 
        llvm::GlobalValue::LinkageTypes::CommonLinkage,
        get_default_value(quad->arg1->symbol->type, context),
        quad->arg1->symbol->get_name());

    quad->arg1->symbol->symbol_data.value = llvm_global;
    return llvm_global;
}

/**
 * Generates LLVM code for an instruction.
 */
static llvm::Value *codegen(std::shared_ptr<Quad> quad, CodegenContext& context)
{
    switch (quad->op)
    {
        case QuadOp::Global:
            return codegen_global(quad, context);
        case QuadOp::Add:
        case QuadOp::Sub:
        case QuadOp::Mul:
        case QuadOp::Div:
        case QuadOp::Mod:
            return codegen_binop(quad, context);
        case QuadOp::Neg:
        case QuadOp::RDeref:
        case QuadOp::AddrOf:
        case QuadOp::Copy:
            return codegen_unop(quad, context);
        case QuadOp::LDeref:
            return codegen_lderef(quad, context);
        case QuadOp::AddPtr:
            return codegen_addptr(quad, context);
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
        case QuadOp::Cast:
            return codegen_cast(quad, context);
        case QuadOp::String:
            return codegen_string(quad, context);
        case QuadOp::Enter:
        case QuadOp::Label:
            break;
    }

    return nullptr;
}

/**
 * Generates LLVM code for a basic block.
 */
static llvm::Value *codegen(std::shared_ptr<BasicBlock> block, CodegenContext& context)
{
    context.llvm_builder->SetInsertPoint(context.llvm_block);
    for (auto quad = block->qlist.begin(); quad != block->qlist.end(); quad = quad->next)
    {
        codegen(quad, context);
    }

    if (!is_terminator(block->qlist.get_tail()))
    {
        auto next_inst = block->qlist.get_tail()->next;
        if (next_inst == nullptr)
        {
            // if this is the last block in the function, insert a return
            auto default_value = get_default_value(context.function_def->function->type->ret_type, context);
            if (default_value == nullptr)
            {
                context.llvm_builder->CreateRetVoid();
            }
            else
            {
                context.llvm_builder->CreateRet(default_value);
            }
        }
        else
        {
            // insert a jump to the next instruction
            auto next_block = context.llvm_block->getNextNode();
            context.llvm_builder->CreateBr(next_block);
        }
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
    auto llvm_function = llvm::Function::Create(llvm_function_type, llvm::Function::ExternalLinkage, def->function->get_name(), context.llvm_module.get());
    return llvm_function;
}

/**
 * Generates LLVM code for a function.
 */
static llvm::Function *codegen(std::shared_ptr<FunctionDef> def, CodegenContext& context)
{
    context.function_def = def;

    if (def->is_proto())
    {
        return codegen_prototype(def, context);
    }

    auto llvm_function = context.llvm_module->getFunction(def->function->get_name());
    if (llvm_function == nullptr)
    {
        llvm_function = codegen_prototype(def, context); 
    }

    context.llvm_function = llvm_function;

    // create an LLVM block for each block in the CFG
    std::vector<llvm::BasicBlock *> llvm_blocks;
    for (auto i = 0; i < def->cfg.size(); i++)
    {
        auto block = create_llvm_block(def->cfg[i], i, context);
        llvm_blocks.push_back(block);
    }

    assert(def->cfg.size() == llvm_blocks.size() && llvm_blocks.size() > 0);

    // allocate the stack variables for the function
    context.llvm_builder->SetInsertPoint(llvm_blocks.front());
    for (auto s : def->symbol_table->get_all_variables())
    {
        auto llvm_type = get_llvm_type(s->type, context);
        s->symbol_data.value = context.llvm_builder->CreateAlloca(llvm_type, nullptr, s->get_name());
    }

    // store each argument into the allocated stack variable
    for (auto i = 0; i < context.function_def->params.size(); i++)
    {
        store(context.function_def->params[i], context.llvm_function->getArg(i), context);
    }

    // generate LLVM code for each block in the CFG
    for (auto i = 0; i < def->cfg.size(); i++)
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

    for (auto global : program->globals)
    {
        for (auto quad = global->ir_list.get_head(); quad != nullptr; quad = quad->next)
        {
            codegen(quad, context);
        }
    }

    for (auto function : program->functions)
    {
        codegen(function, context);
    }

    llvm::raw_os_ostream stream(*file);
    context.llvm_module->print(stream, nullptr);
}

#include <vector>
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

static llvm::Value *codegen(std::shared_ptr<Operand> operand, CodegenContext& context)
{
    switch (operand->type)
    {
        case OperandType::IntConst:
            return llvm::ConstantInt::get(*context.llvm_context, llvm::APInt(32, operand->iconst));
        case OperandType::StrConst:
            return nullptr; // TODO
        case OperandType::Variable:
            return context.llvm_builder->CreateLoad(
                get_llvm_type(operand->symbol->type, context), 
                context.named_values[operand->symbol->name]);
        case OperandType::Label:
            return nullptr; // TODO
    }
}

static llvm::Value *codegen_binop(std::shared_ptr<Quad> quad, CodegenContext& context)
{
    assert(quad->res->type == OperandType::Variable);
    auto arg1 = codegen(quad->arg1, context);
    auto arg2 = codegen(quad->arg2, context);
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

    res = context.llvm_builder->CreateAlloca(
        get_llvm_type(quad->res->symbol->type, context), 
        nullptr, 
        quad->res->symbol->name);
    context.llvm_builder->CreateStore(arg1, res, false);
    context.named_values[quad->res->symbol->name] = res; // TODO named values wont work for nested symbol tables, need to store these in the symbol table
                                                         // What I could do is have BackEnd inheirit from SymbolTable to augment it with additional info,
                                                         // FrontEnd uses augmented SymbolTable
    return res;
}

static llvm::Value *codegen_unop(std::shared_ptr<Quad> quad, CodegenContext& context)
{
    assert(quad->res->type == OperandType::Variable);

    auto arg1 = codegen(quad->arg1, context);
    
    switch (quad->op)
    {
        case QuadOp::Neg:
            arg1 = context.llvm_builder->CreateNeg(arg1);
            break;
        case QuadOp::Deref:
        case QuadOp::Addr:
            break; // TODO
        case QuadOp::Copy:
        default:
            break;
    }

    auto res = context.llvm_builder->CreateAlloca(
        get_llvm_type(quad->res->symbol->type, context), 
        nullptr, 
        quad->res->symbol->name);
    context.llvm_builder->CreateStore(arg1, res, false);
    context.named_values[quad->res->symbol->name] = res;
    return res;
}

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
        case QuadOp::Label:
        case QuadOp::Goto:
        case QuadOp::IfEq:
        case QuadOp::IfNeq:
        case QuadOp::IfLt:
        case QuadOp::IfLeq:
        case QuadOp::IfGt:
        case QuadOp::IfGeq:
        case QuadOp::Enter:
            break; // TODO
        case QuadOp::Return:
            return codegen_return(quad, context);
        case QuadOp::Param:
        case QuadOp::Call:
        case QuadOp::Retrieve:
            break; // TODO
        default:
            break;
    }

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
    }

    return llvm_function;
}

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

static void codegen_test_functions(CodegenContext& context);

void codegen(std::shared_ptr<Program> program, std::ostream *file, bool link_test)
{
    CodegenContext context;
    for (auto function : program->functions)
    {
        auto llvm_function = codegen(function, context);
    }

    if (link_test)
    {
        codegen_test_functions(context);
    }

    if (file == nullptr)
    {
        context.llvm_module->dump();
    }
    else
    {
        llvm::raw_os_ostream stream(*file);
        context.llvm_module->print(stream, nullptr);
    }
}

static llvm::Function *codegen_println(CodegenContext& context);
static llvm::Function *codegen_printf_declaration(CodegenContext& context);

static void codegen_test_functions(CodegenContext& context)
{
    codegen_printf_declaration(context);
    codegen_println(context);
}

static llvm::Function *codegen_printf_declaration(CodegenContext& context)
{
    auto char_ptr_type = llvm::PointerType::get(context.llvm_builder->getInt8Ty(), 0);
    auto printf_type = llvm::FunctionType::get(llvm::IntegerType::get(*context.llvm_context.get(), 32), char_ptr_type, true);

    // auto printf_type = llvm::FunctionType::get(context.llvm_builder->getInt32Ty(), { context.llvm_builder->getPtrTy() }, false);
    // auto printf_type = llvm::TypeBuilder<int(char *, ...), false>::get(llvm::getGlobalContext());

    auto func_printf = llvm::Function::Create(printf_type, llvm::GlobalValue::ExternalLinkage, "printf", context.llvm_module.get());
    func_printf->setCallingConv(llvm::CallingConv::C);

    //llvm::AttrListPtr func_printf_PAL;
    //func_printf->setAttributes(func_printf_PAL);


    std::cerr << "HERE\n";

    return func_printf;
}

static llvm::Function *codegen_println(CodegenContext& context)
{
    // TODO I think I will need to define printf, then call it with a format string for println
    // TODO generate code for function call and test this

    auto format_specifier = context.llvm_builder->CreateGlobalString("%d\n");

    auto llvm_function_type = llvm::FunctionType::get(context.llvm_builder->getVoidTy(), context.llvm_builder->getInt32Ty(), false);
    auto llvm_function = llvm::Function::Create(llvm_function_type, llvm::Function::ExternalLinkage, "println", context.llvm_module.get());

    auto entry = llvm::BasicBlock::Create(*context.llvm_context, "println_entry", llvm_function);
    context.llvm_builder->SetInsertPoint(entry);

    auto printf_func = context.llvm_module->getFunction("printf");
    llvm::Value *args[] = { format_specifier, llvm_function->getArg(0) };
    auto printf_call = context.llvm_builder->CreateCall(printf_func, args);
    context.llvm_builder->CreateRetVoid();

    llvm::verifyFunction(*llvm_function);
    return llvm_function;
}
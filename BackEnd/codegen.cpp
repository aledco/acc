#include <iostream>
#include <memory>
#include <vector>

// #include <clang/Driver/Driver.h>
// #include <clang/Driver/Compilation.h>
// #include <clang/Frontend/TextDiagnosticPrinter.h>

#include <llvm/ADT/APInt.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/Module.h>

// #include <llvm/Target/TargetOptions.h>
// #include <llvm/ADT/STLExtras.h>
// #include <llvm/Support/FileSystem.h>
// #include <llvm/Support/TargetSelect.h>
// #include <llvm/IR/LegacyPassManager.h>
// #include <llvm/Target/TargetMachine.h>

#include "SyntaxTree.hpp"
#include "Type.hpp"
#include "CodegenContext.hpp"
#include "CodegenResult.hpp"

static llvm::Type *get_llvm_type(CodegenContext& context, std::shared_ptr<Type> type)
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

// TODO get this function to work, need to link to clang, add namespace prefixes, etc
// static void generate_object_file(CodegenContext& context)
// {
//     auto TargetTriple = llvm::getDefaultTargetTriple();
//     llvm::InitializeAllTargetInfos();
//     llvm::InitializeAllTargets();
//     llvm::InitializeAllTargetMCs();
//     llvm::InitializeAllAsmParsers();
//     llvm::InitializeAllAsmPrinters();

//     std::string Error;
//     auto Target = TargetRegistry::lookupTarget(TargetTriple, Error);
//     auto CPU = "generic";
//     auto Features = "";

//     TargetOptions opt;
//     auto RM = Optional<Reloc::Model>();
//     auto TargetMachine = Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);

//     module->setDataLayout(TargetMachine->createDataLayout());
//     module->setTargetTriple(TargetTriple);

//     auto Filename = "output.o";
//     std::error_code EC;
//     llvm::raw_fd_ostream dest(Filename, EC, sys::fs::OF_None);

//     legacy::PassManager pass;
//     auto FileType = CGFT_ObjectFile;

//     if (TargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType))
//     {
//         errs() << "TargetMachine can't emit a file of this type";
//         return;
//     }
//     pass.run(*module);
//     dest.flush();

//     //llvm::sIntrusiveRefCntPtr<clang::DiagnosticOptions> DiagOpts = new clang::DiagnosticOptions;
//     //clang::TextDiagnosticPrinter *DiagClient = new clang::TextDiagnosticPrinter(errs(), &*DiagOpts);
//     //IntrusiveRefCntPtr<clang::DiagnosticIDs> DiagID(new clang::DiagnosticIDs());
//     //clang::DiagnosticsEngine Diags(DiagID, &*DiagOpts, DiagClient);
//     //clang::driver::Driver TheDriver("/usr/bin/clang++-12", TargetTriple, Diags);

//     auto args = llvm::ArrayRef<const char *>{"-g", "output.o", "-o", "main"};

//     //std::unique_ptr<clang::driver::Compilation> C(TheDriver.BuildCompilation(args));

//     if (C && !C->containsError())
//     {
//         SmallVector<std::pair<int, const clang::driver::Command *>, 4> FailingCommands;
//         TheDriver.ExecuteCompilation(*C, FailingCommands);
//     }

//     remove(Filename);
// }

std::unique_ptr<CodegenResult> IntegerConstant::codegen(CodegenContext& context)
{
    // TODO use expr.type.size() for num bytes once type checking is implemented
    auto llvm_value = llvm::ConstantInt::get(*context.llvm_context, llvm::APInt(32, value));
    return std::make_unique<CodegenResult>(llvm_value);
}

std::unique_ptr<CodegenResult> Variable::codegen(CodegenContext& context)
{
    auto alloca = context.named_values[symbol->name];
    auto llvm_value = context.llvm_builder->CreateLoad(alloca->getAllocatedType(), alloca, symbol->name);
    return std::make_unique<CodegenResult>(llvm_value);
}

std::unique_ptr<CodegenResult> CompoundStatement::codegen(CodegenContext& context)
{
    // TODO once control structures are implemeted, will need to create the control flow graph to seperate the body into basic blocks
    for (auto &statement : statements)
    {
        auto result = statement->codegen(context);
    }

    return std::make_unique<CodegenResult>();
}

std::unique_ptr<CodegenResult> VariableDeclaration::codegen(CodegenContext& context)
{
    auto llvm_type = get_llvm_type(context, type);
    auto alloca = context.llvm_builder->CreateAlloca(llvm_type, nullptr, symbol->name);
    context.named_values[symbol->name] = alloca; // TODO using named_values this way isn't as flexible as a symbol table, may override variable in outer scope 
    return std::make_unique<CodegenResult>(alloca);
}

std::unique_ptr<CodegenResult> Return::codegen(CodegenContext& context)
{
    if (expr != nullptr)
    {
        auto result = expr->codegen(context);
        context.llvm_builder->CreateRet(result->value);
    }
    else
    {
        context.llvm_builder->CreateRetVoid();
    }

    return std::make_unique<CodegenResult>();
}

std::unique_ptr<CodegenResult> FunctionDef::codegen(CodegenContext& context)
{
    // first, check for the function from a previous declaration
    auto llvm_function = context.llvm_module->getFunction(function->name);
    if (llvm_function == nullptr)
    {
        std::vector<llvm::Type *> llvm_param_types;
        for (auto& param : params)
        {
            llvm_param_types.push_back(get_llvm_type(context, param->type));
        }

        auto llvm_return_type = get_llvm_type(context, function->type->alt_type);
        auto llvm_function_type = llvm::FunctionType::get(llvm_return_type, llvm_param_types, false);
        llvm_function = llvm::Function::Create(llvm_function_type, llvm::Function::ExternalLinkage, function->name, context.llvm_module.get());

        // set the names for the args
        std::size_t i = 0;
        for (auto &arg : llvm_function->args())
        {
            arg.setName(params[i]->name);
        } 
    }

    // create a basic block for the function entry
    auto entry_block = llvm::BasicBlock::Create(*context.llvm_context, "entry", llvm_function);
    context.llvm_builder->SetInsertPoint(entry_block);

    // record all the args in the named_values map
    context.named_values.clear();
    for (auto &arg : llvm_function->args())
    {
        context.named_values[std::string(arg.getName())] = &arg;
    }

    context.llvm_function = llvm_function;

    auto result = body->codegen(context);
    if (result == nullptr)
    {
        // ERROR generating function
        llvm_function->eraseFromParent();
        return nullptr;
    }

    
    llvm::verifyFunction(*llvm_function);
    return std::make_unique<CodegenResult>(llvm_function);
}

std::unique_ptr<CodegenResult> Program::codegen(CodegenContext& context)
{
    for (auto &function : functions)
    {
        auto result = function->codegen(context);
        //result->value->print(llvm::errs());
        result->value->dump();
    }

    return nullptr;
}

void Program::codegen()
{
    CodegenContext context;
    codegen(context);
}

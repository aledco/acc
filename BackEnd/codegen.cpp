#include <iostream>
#include <memory>
#include "SyntaxTree.hpp"
#include "CodegenContext.hpp"

void IntegerConstant::codegen(CodegenContext& context)
{
}

void CompoundStatement::codegen(CodegenContext& context)
{
}

void Return::codegen(CodegenContext& context)
{
}

void FunctionDef::codegen(CodegenContext& context)
{
}

void Program::codegen(CodegenContext& context)
{
    
}

void Program::codegen()
{
    CodegenContext context;
    codegen(context);
}

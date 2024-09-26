#include <vector>
#include "Parser.hpp"

std::unique_ptr<Program> Parser::parse()
{
    ParserContext context;
    context.global_symbol_table = std::make_shared<SymbolTable>();

    std::vector<std::unique_ptr<FunctionDef>> functions;
    while (is_currently({"void", "int"}))
    {
        functions.push_back(std::move(parse_function(context)));
    }

    return std::make_unique<Program>(functions);
}

// SyntaxTree Parser::parse_varable_declaration()
// {

// }

std::unique_ptr<FunctionDef> Parser::parse_function(ParserContext& context)
{
    context.local_symbol_table = std::make_shared<SymbolTable>(context.global_symbol_table);
    
    auto return_type = parse_type(context);
    auto function_name_token = match(TokenType_Id);

    match("(");

    std::vector<std::shared_ptr<Symbol>> param_symbols;
    std::vector<std::shared_ptr<Type>> param_types;
    while (is_currently({ "void", "int" }))
    {
        auto param_type = parse_type(context);
        param_types.push_back(param_type);

        auto param_token = match(TokenType_Id);
        if (is_currently({ "," }))
        {
            match(","); // TODO this will allow us to not use commas
        }

        auto param_symbol = context.local_symbol_table->add_symbol(param_token.value, param_type, true);
        param_symbols.push_back(param_symbol);
    }

    match(")");

    auto function_type = std::make_shared<FunctionType>(return_type, param_types);
    auto function_symbol = context.local_symbol_table->add_symbol(function_name_token.value, function_type, false);
    
    auto body = parse_compound_statement(context);
    return std::make_unique<FunctionDef>(function_symbol, param_symbols, std::move(body));
}

std::unique_ptr<Statement> Parser::parse_statement(ParserContext& context)
{
    if (is_currently({ "return" }))
    {
        return parse_return_statement(context);
    }
    else if (is_currently({ "{" }))
    {
        return parse_compound_statement(context);
    }

    // TODO error
    exit(1);
}

std::unique_ptr<CompoundStatement> Parser::parse_compound_statement(ParserContext& context)
{
    match("{");;
    std::vector<std::unique_ptr<Statement>> statements;
    while (is_currently({ "return", "{" }))
    {
        statements.push_back(std::move(parse_statement(context)));
    }

    match("}");

    return std::make_unique<CompoundStatement>(statements);
}

std::unique_ptr<Return> Parser::parse_return_statement(ParserContext& context)
{
    match("return");
    if (is_currently({ TokenType_Int }))
    {
        auto expr = parse_expression(context);
        return std::make_unique<Return>(expr);
    }
    else
    {
        return std::make_unique<Return>();
    }
}

std::unique_ptr<Expression> Parser::parse_expression(ParserContext& context)
{
    if (is_currently({ TokenType_Int }))
    {
        return parse_integer_constant(context);
    }

    // TODO error
    std::exit(1);
}

std::unique_ptr<IntegerConstant> Parser::parse_integer_constant(ParserContext& context)
{
    auto token = match(TokenType_Int);
    return std::make_unique<IntegerConstant>(std::stol(token.value));
}

std::shared_ptr<Type> Parser::parse_type(ParserContext& context)
{
    if (is_currently({ "void" }))
    {
        match("void");
        return std::make_shared<VoidType>();
    }
    else if (is_currently({ "int" }))
    {
        match("int");
        return std::make_shared<IntType>();
    }
    
    // TODO error
    std::exit(1);
}

Token& Parser::match(std::string token_type)
{
    if (current().type == token_type)
    {
        auto& prev = current();
        next();
        return prev;
    }
    
    // TODO error
    std::exit(1);
}

bool Parser::is_currently(std::initializer_list<std::string> options)
{
    for (auto& option : options)
    {
        if (current().type == option)
        {
            return true;
        }
    }

    return false;
}

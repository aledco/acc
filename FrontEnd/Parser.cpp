#include "Parser.hpp"

SyntaxTree Parser::parse()
{

    return {};
}

// SyntaxTree Parser::parse_varable_declaration()
// {

// }

FunctionDef Parser::parse_function(ParserContext& context)
{
    context.local_symbol_table = std::make_shared<SymbolTable>(context.global_symbol_table);
    
    auto return_type = parse_type(context);
    auto function_name_token = match(TokenType_Id);

    match("(");

    std::vector<std::shared_ptr<Symbol>> params;
    while (is_currently({ "void", "int" }))
    {
        auto param_type = parse_type(context);
        auto param_token = match(TokenType_Id);
        if (is_currently({ "," }))
        {
            match(","); // TODO this will allow us to not use commas
        }

        auto paramSymbol = context.local_symbol_table->add_symbol(param_token.value, param_type, true);
        params.push_back(paramSymbol);
    }

    match(")");

    FunctionType type(return_type);
}

Statement Parser::parse_statement(ParserContext& context)
{
    if (is_currently({ "return" }))
    {
        return parse_return_statement(context);
    }

    // TODO error
    exit(1);
}

CompoundStatement Parser::parse_compound_statement(ParserContext& context)
{
    std::vector<Statement> statements;
    while (is_currently({ "return" }))
    {
        statements.push_back(parse_statement(context));
    }

    return CompoundStatement(statements);
}

Return Parser::parse_return_statement(ParserContext& context)
{
    match("return");

    std::unique_ptr<Expression> expr = nullptr;
    if (is_currently({ TokenType_Int }))
    {
        expr = std::make_unique<Expression>(parse_expression());
    }

    return Return(std::move(expr));
}

Expression Parser::parse_expression(ParserContext& context)
{
    if (is_currently({ TokenType_Int }))
    {
        return parse_integer_constant(context);
    }

    // TODO error
    std::exit(1);
}

IntegerConstant Parser::parse_integer_constant(ParserContext& context)
{
    auto token = match(TokenType_Int);
    return IntegerConstant(std::stol(token.value));
}

Type Parser::parse_type(ParserContext& context)
{
    if (is_currently({ "void" }))
    {
        match("void");
        return VoidType();
    }
    else if (is_currently({ "int" }))
    {
        match("int");
        return IntType();
    }
    
    // TODO error
    std::exit(1);
}

Token& Parser::match(std::string token_type)
{
    if (current().type == token_type)
    {
        auto prev = current();
        next();
        return prev;
    }
    
    // TODO error
    std::exit(1);
}

Token& Parser::match_type()
{
    if (current().type == "void")
    {
        return match("void");
    }
    else if (current().type == "int")
    {
        return match("int");
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
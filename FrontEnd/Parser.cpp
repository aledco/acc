#include <vector>
#include <algorithm>
#include "Parser.hpp"

std::shared_ptr<Program> Parser::parse()
{
    ParserContext context;
    context.global_symbol_table = std::make_shared<SymbolTable>();
    context.current_symbol_table = context.global_symbol_table;

    std::vector<std::shared_ptr<FunctionDef>> functions;
    while (is_currently({"void", "int"}))
    {
        functions.push_back(std::move(parse_function(context)));
    }

    return std::make_shared<Program>(functions);
}

std::shared_ptr<FunctionDef> Parser::parse_function(ParserContext& context)
{
    context.current_symbol_table = std::make_shared<SymbolTable>(context.global_symbol_table);
    
    auto return_type = parse_type(context);
    auto function_name_token = match(TokenType_Id);

    match("(");

    std::vector<std::shared_ptr<Symbol>> params;
    if (is_currently({ "void", "int" }))
    {
        auto param = parse_parameter(context);
        params.push_back(param);
    }
    while (is_currently({ "," }))
    {
        match(",");
        auto param = parse_parameter(context);
        params.push_back(param);
    }

    match(")");

    std::vector<std::shared_ptr<Type>> param_types;
    std::transform(std::begin(params), std::end(params), std::begin(param_types), 
        [](const auto& param) -> auto { return param->type; });
    
    auto function_type = std::make_shared<Type>(TypeType::Function, return_type);
    auto function_symbol = context.current_symbol_table->add_symbol(function_name_token.value, function_type);
    
    auto body = parse_compound_statement(context);
    return std::make_shared<FunctionDef>(function_symbol, params, std::move(body));
}

std::shared_ptr<Symbol> Parser::parse_parameter(ParserContext& context)
{
    auto param_type = parse_type(context);
    auto param_token = match(TokenType_Id);
    return context.current_symbol_table->add_symbol(param_token.value, param_type);
}

std::shared_ptr<Statement> Parser::parse_statement(ParserContext& context)
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

std::shared_ptr<CompoundStatement> Parser::parse_compound_statement(ParserContext& context)
{
    match("{");;
    std::vector<std::shared_ptr<Statement>> statements;
    while (is_currently({ "return", "{" }))
    {
        statements.push_back(parse_statement(context));
    }

    match("}");

    return std::make_shared<CompoundStatement>(statements);
}

std::shared_ptr<VariableDeclaration> parse_variable_declaration(ParserContext& context)
{
    auto type = parse_type(context);
    auto token = match(TokenType_Id);
    return context.current_symbol_table->add_symbol(token.value, type);
}

std::shared_ptr<Return> Parser::parse_return_statement(ParserContext& context)
{
    match("return");
    if (is_currently({ TokenType_Int }))
    {
        auto expr = parse_expression(context);
        match(";");
        return std::make_shared<Return>(expr);
    }
    else
    {
        match(";");
        return std::make_shared<Return>();
    }
}

std::shared_ptr<Expression> Parser::parse_expression(ParserContext& context)
{
    if (is_currently({ TokenType_Int }))
    {
        return parse_integer_constant(context);
    }

    // TODO error
    std::exit(1);
}

std::shared_ptr<Variable> parse_variable(ParserContext& context)
{
    var symbol = parse_identifier(context);
    return std::make_shared<Variable>(symbol);
}

std::shared_ptr<IntegerConstant> Parser::parse_integer_constant(ParserContext& context)
{
    auto token = match(TokenType_Int);
    return std::make_shared<IntegerConstant>(std::stol(token.value));
}

std::shared_ptr<Type> Parser::parse_type(ParserContext& context)
{
    if (is_currently({ "void" }))
    {
        match("void");
        return std::make_shared<Type>(TypeType::Void);
    }
    else if (is_currently({ "int" }))
    {
        match("int");
        return std::make_shared<Type>(TypeType::Int);
    }
    
    // TODO error
    std::exit(1);
}

std::shared_ptr<Symbol> Parser::parse_identifier(ParserContext& context)
{
    auto token = match(TokenType_Id);
    return context.current_symbol_table->lookup(token.value);
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

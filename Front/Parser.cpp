#include <vector>
#include <algorithm>
#include "Operator.hpp"
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
    if (is_currently({ "void", "int" }))
    {
        return parse_variable_declaration(context);
    }
    else if (is_currently({ "return" }))
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
    while (is_currently({ "void", "int", "return", "{" }))
    {
        statements.push_back(parse_statement(context));
    }

    match("}");

    return std::make_shared<CompoundStatement>(statements);
}

std::shared_ptr<VariableDeclaration> Parser::parse_variable_declaration(ParserContext& context)
{
    auto type = parse_type(context);
    auto token = match(TokenType_Id);
    auto symbol = context.current_symbol_table->add_symbol(token.value, type);
    return std::make_shared<VariableDeclaration>(type, symbol);
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
    return parse_expression(context, operator_precedence.size() - 1);
}

std::shared_ptr<Expression> Parser::parse_expression(ParserContext& context, int p)
{
    if (p == -1)
    {
        return parse_unary(context);
    }

    auto lhs = parse_expression(context, p-1);
    while (is_currently(operator_precedence[p]))
    {
        auto op = match(operator_precedence[p]);
        auto rhs = parse_expression(context, p-1);
        lhs = std::make_shared<BinaryOperation>(getBinOp(op.value), lhs, rhs);
    }

    return lhs;
}

// unary := (+ | - | * | & | ++ | --)? term | term (++ | --)?;
std::shared_ptr<Expression> Parser::parse_unary(ParserContext& context)
{
    if (is_currently({ "-", "*", "&" }))
    {
        auto op = match({ "-", "*", "&" });
        auto expr = parse_term(context);
        return std::make_shared<UnaryOperation>(getUnOp(op.value), expr);
    }

    // TODO handle increment operators later

    // TODO error
    std::exit(1);
}

// term := constant | id; 
std::shared_ptr<Expression> Parser::parse_term(ParserContext& context)
{
    if (is_currently({ TokenType_Int }))
    {
        return parse_integer_constant(context);
    }
    else if (is_currently({ TokenType_Id }))
    {
        auto symbol = parse_identifier(context);
        if (is_currently({ "(" }))
        {
            // TODO parse function
        }
        else if (is_currently({ "[" }))
        {
            // TODO parse array
        }
        else
        {
            return std::make_shared<Variable>(symbol);
        }
    }

    // TODO add parens, function calls, arrays, etc

    // TODO error
    std::exit(1);
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

Token& Parser::match(std::initializer_list<std::string> token_types)
{
    std::vector<std::string> vec = token_types;
    return match(vec);
}

Token& Parser::match(std::vector<std::string> token_types)
{
    for (auto& token_type : token_types)
    {
        if (current().type == token_type)
        {
            return match(token_type);
        }
    }
    
    // TODO error
    std::exit(1);
}

bool Parser::is_currently(std::initializer_list<std::string> options)
{
    std::vector<std::string> vec = options;
    return is_currently(vec);
}

bool Parser::is_currently(std::vector<std::string> options)
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

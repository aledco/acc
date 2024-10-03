#include <algorithm>
#include <iostream>
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
    std::shared_ptr<Statement> statement;
    if (is_currently({ "void", "int" }))
    {
        statement = parse_variable_declaration(context);
    }
    else if (is_currently({ "return" }))
    {
        statement = parse_return_statement(context);
    }
    else if (is_currently({ "{" }))
    {
        statement = parse_compound_statement(context);
    }
    else if (is_currently({ TokenType_Int, TokenType_Id, "(", "-", "*", "&" })) // TODO add any other tokens that can start an expression
    {
        statement = parse_expression(context);
    }
    else
    {
        error({ "void", "int", "return", "{", TokenType_Int, TokenType_Id, "(", "-", "*", "&" }); // TODO should make these long lists of tokens constants
    }

    match(";");
    return statement;
}

std::shared_ptr<CompoundStatement> Parser::parse_compound_statement(ParserContext& context)
{
    match("{");;
    std::vector<std::shared_ptr<Statement>> statements;
    while (is_currently({ "void", "int", "return", "{", TokenType_Int, TokenType_Id, "(", "-", "*", "&" }))
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
    if (is_currently({ TokenType_Int, TokenType_Id, "(", "-", "*", "&" }))
    {
        auto expr = parse_expression(context);
        return std::make_shared<Return>(expr);
    }
    else
    {
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
    else if (is_currently({ TokenType_Int, TokenType_Id }))
    {
        return parse_term(context);
    }

    // TODO handle increment operators later

    error({ "-", "*", "&", TokenType_Int, TokenType_Id });
}

// term := constant | id; 
std::shared_ptr<Expression> Parser::parse_term(ParserContext& context)
{
    if (is_currently({ TokenType_Int }))
    {
        auto token = match(TokenType_Int);
        return std::make_shared<IntegerConstant>(std::stol(token.value));
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

    error({ TokenType_Int, TokenType_Id });
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
    
    error({ "void", "int" });
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
    
    error({ token_type });
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
    
    error(token_types);
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

_GLIBCXX_NORETURN void Parser::error(std::initializer_list<std::string> token_types)
{
    std::vector<std::string> vec = token_types;
    error(vec);
}

_GLIBCXX_NORETURN void Parser::error(std::vector<std::string> token_types)
{
    std::cerr << "Parse Error: around " << current().span.end.line << ":" << current().span.end.col << ": expected ";
    for (auto i = 0; i < token_types.size(); i++)
    {
        std::cerr << token_types[i]; 
        if (i < token_types.size() - 1)
        {
            std::cerr << ", ";
        }
    }

    std::cerr << " got " << current().value << "\n";
    std::exit(1);
}
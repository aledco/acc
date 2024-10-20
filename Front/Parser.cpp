#include <algorithm>
#include <iostream>
#include "Span.hpp"
#include "Operator.hpp"
#include "Error.hpp"
#include "Parser.hpp"


/**
 * Parses the program and retunrs the abstract syntax tree.
 */
std::shared_ptr<Program> Parser::parse()
{
    ParserContext context;
    std::vector<std::shared_ptr<FunctionDef>> functions;
    Span span;
    while (is_currently({"void", "int", "extern"}))
    {   
        auto f = parse_function(context);
        functions.push_back(f);
        span += f->span;
    }

    auto program = std::make_shared<Program>(span, functions, context.global_symbol_table);
    program->typecheck();
    return program;
}

/**
 * Parses a function.
 */
std::shared_ptr<FunctionDef> Parser::parse_function(ParserContext& context)
{
    context.push_symbol_table();

    Span span = current().span;

    bool is_extern = false;
    if (is_currently({ "extern" })) 
    {
        match("extern");
        is_extern = true;
    }

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
    for (auto& param : params)
    {
        param_types.push_back(param->type);
    }

    if (is_currently({";"})) 
    {
        auto token = match(";");
        span += token.span;
        auto function_type = std::make_shared<Type>(TypeType::Function, return_type, param_types, is_extern, false);
        auto function_symbol = context.global_symbol_table->add_symbol(function_name_token, function_type);
        context.pop_symbol_table();
        return std::make_shared<FunctionDef>(span, function_symbol, params, context.current_symbol_table()); // prototype
    }
    else
    {
        auto function_symbol = context.global_symbol_table->try_lookup(function_name_token);
        if (function_symbol != nullptr)
        {
            if (function_symbol->type->is_defined)
            {
                throw Error(span, "Error", "multiple definitions of function " + function_name_token.value);
            }
            else
            {
                function_symbol->type->is_defined = true;
            }
        }
        else
        {
            auto function_type = std::make_shared<Type>(TypeType::Function, return_type, param_types, is_extern, true);
            function_symbol = context.global_symbol_table->add_symbol(function_name_token, function_type);
        }

        auto body = parse_compound_statement(context);
        span += body->span;
        context.pop_symbol_table();
        return std::make_shared<FunctionDef>(span, function_symbol, params, body, context.current_symbol_table());
    }
}

/**
 * Parses a parameter.
 */
std::shared_ptr<Symbol> Parser::parse_parameter(ParserContext& context)
{
    auto param_type = parse_type(context);
    auto param_token = match(TokenType_Id);
    auto sym = context.current_symbol_table()->add_symbol(param_token, param_type);
    sym->is_parameter = true;
    return sym;
}

/**
 * Parses a statement.
 */
std::shared_ptr<Statement> Parser::parse_statement(ParserContext& context)
{
    std::shared_ptr<Statement> statement;
    if (is_currently({ "void", "int" }))
    {
        statement = parse_variable_declaration(context);
        return statement;
    }
    else if (is_currently({ "if" }))
    {
        statement = parse_if_statement(context);
        return statement;
    }
    else if (is_currently({ "return" }))
    {
        statement = parse_return_statement(context);
        return statement;
    }
    else if (is_currently({ "{" }))
    {
        statement = parse_compound_statement(context);
        return statement;
    }
    else if (is_currently({ TokenType_Int, TokenType_Id, "(", "-", "*", "&" }))
    {
        statement = parse_expression(context);
        auto token = match(";");
        statement->span += token.span;
        return statement;
    }
    else
    {
        throw ParseError(current(), { "void", "int", "return", "{", TokenType_Int, TokenType_Id, "(", "-", "*", "&" });
    }
}

/**
 * Parses a compound statement.
 */
std::shared_ptr<CompoundStatement> Parser::parse_compound_statement(ParserContext& context)
{
    context.push_symbol_table();

    Span span = current().span;
    match("{");
    std::vector<std::shared_ptr<Statement>> statements;
    while (is_currently({ "void", "int", "if", "return", "{", TokenType_Int, TokenType_Id, "(", "-", "*", "&" }))
    {
        statements.push_back(parse_statement(context));
    }

    auto token = match("}");
    span += token.span;

    auto symbol_table = context.current_symbol_table();
    context.pop_symbol_table();
    return std::make_shared<CompoundStatement>(span, statements, symbol_table);
}

/**
 * Parses a variable declaration.
 */
std::shared_ptr<VariableDeclaration> Parser::parse_variable_declaration(ParserContext& context)
{
    Span span = current().span;
    auto type = parse_type(context);
    auto id_token = match(TokenType_Id);
    auto semi_token = match(";");
    span += semi_token.span;
    auto symbol = context.current_symbol_table()->add_symbol(id_token, type);
    return std::make_shared<VariableDeclaration>(span, type, symbol, context.current_symbol_table());
}

/**
 * Parses an if statement.
 */
std::shared_ptr<IfStatement> Parser::parse_if_statement(ParserContext& context)
{
    Span span = current().span;
    match("if");
    match("(");
    auto guard = parse_expression(context);
    match (")");
    auto then_stmt = parse_statement(context);
    if (is_currently({ "else" }))
    {
        match("else");
        auto else_stmt = parse_statement(context);
        span += else_stmt->span;
        return std::make_shared<IfStatement>(span, guard, then_stmt, else_stmt, context.current_symbol_table());
    }
    else
    {
        span += then_stmt->span;
        return std::make_shared<IfStatement>(span, guard, then_stmt, context.current_symbol_table());
    }
}

/**
 * Parses a return statement.
 */
std::shared_ptr<Return> Parser::parse_return_statement(ParserContext& context)
{
    Span span = current().span;
    match("return");
    if (is_currently({ TokenType_Int, TokenType_Id, "(", "-", "*", "&" }))
    {
        auto expr = parse_expression(context);
        auto token = match(";");
        span += token.span;
        return std::make_shared<Return>(span, expr, context.current_symbol_table());
    }
    else
    {
        auto token = match(";");
        span += token.span;
        return std::make_shared<Return>(span, context.current_symbol_table());
    }
}

/**
 * Parses an expression.
 */
std::shared_ptr<Expression> Parser::parse_expression(ParserContext& context)
{
    return parse_expression(context, operator_precedence.size() - 1);
}

/**
 * Parses anm expression for the given precedence level p.
 */
std::shared_ptr<Expression> Parser::parse_expression(ParserContext& context, int p)
{
    if (p == -1)
    {
        return parse_unary(context);
    }

    Span span = current().span;
    auto lhs = parse_expression(context, p-1);
    while (is_currently(operator_precedence[p]))
    {
        auto op = match(operator_precedence[p]);
        auto rhs = parse_expression(context, p-1);
        span = span + rhs->span;
        lhs = std::make_shared<BinaryOperation>(span, getBinOp(op.value), lhs, rhs, context.current_symbol_table());
    }

    return lhs;
}

/**
 * Parses an unary expression.
 */
std::shared_ptr<Expression> Parser::parse_unary(ParserContext& context)
{
    if (is_currently({ "-", "*", "&" }))
    {
        Span span = current().span;
        auto op = match({ "-", "*", "&" });
        auto expr = parse_term(context);
        span += expr->span;
        return std::make_shared<UnaryOperation>(span, getUnOp(op.value), expr, context.current_symbol_table());
    }
    else if (is_currently({ TokenType_Int, TokenType_Id, "(" }))
    {
        return parse_term(context);
    }

    // TODO handle increment operators later

    throw ParseError(current(), { "-", "*", "&", TokenType_Int, TokenType_Id, "(" });
}

/**
 * Parses a term.
 */
std::shared_ptr<Expression> Parser::parse_term(ParserContext& context)
{
    if (is_currently({ TokenType_Int }))
    {
        auto token = match(TokenType_Int);
        return std::make_shared<IntegerConstant>(token.span, std::stol(token.value), context.current_symbol_table());
    }
    else if (is_currently({ TokenType_Id }))
    {
        Span span = current().span;
        auto symbol = parse_identifier(context);
        if (is_currently({ "(" }))
        {
            match("(");

            std::vector<std::shared_ptr<Expression>> args;
            if (is_currently({ TokenType_Int, TokenType_Id, "(", "-", "*", "&" }))
            {
                args.push_back(parse_expression(context));
                while (is_currently({ "," }))
                {
                    match(",");
                    args.push_back(parse_expression(context));
                }
            }

            auto token = match(")");
            span += token.span;
            return std::make_shared<FunctionCall>(span, symbol, args, context.current_symbol_table());
        }
        else if (is_currently({ "[" }))
        {
            // TODO parse array
        }
        else
        {
            return std::make_shared<Variable>(span, symbol, context.current_symbol_table());
        }
    }
    else if (is_currently({ "(" }))
    {
        match("(");
        auto expr = parse_expression(context);
        match(")");
        return expr;
    }

    // TODO add parens, function calls, arrays, etc

    throw ParseError(current(), { TokenType_Int, TokenType_Id, "(" });
}

/**
 * Parses a type.
 */
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
    
    throw ParseError(current(), { "void", "int" });
}

/**
 * Parses an identifier.
 */
std::shared_ptr<Symbol> Parser::parse_identifier(ParserContext& context)
{
    auto token = match(TokenType_Id);
    return context.current_symbol_table()->lookup(token);
}

/**
 * Matches the token type against the current token, and returns the matching token.
 * Throws an error if the token type does not match.
 */
Token& Parser::match(const std::string token_type)
{
    if (current().type == token_type)
    {
        auto& prev = current();
        next();
        return prev;
    }
    
    throw ParseError(current(), { token_type });
}

/**
 * Matches the token types against the current token, and returns the matching token.
 * Throws an error if the token types do not match.
 */
Token& Parser::match(const std::initializer_list<std::string> token_types)
{
    std::vector<std::string> vec = token_types;
    return match(vec);
}

/**
 * Matches the token types against the current token, and returns the matching token.
 * Throws an error if the token types do not match.
 */
Token& Parser::match(const std::vector<std::string> token_types)
{
    for (auto& token_type : token_types)
    {
        if (current().type == token_type)
        {
            return match(token_type);
        }
    }
    
    throw ParseError(current(), token_types);
}

/**
 * Determines if the current token is one of the provided token types.
 */
bool Parser::is_currently(const std::initializer_list<std::string> options)
{
    std::vector<std::string> vec = options;
    return is_currently(vec);
}

/**
 * Determines if the current token is one of the provided token types.
 */
bool Parser::is_currently(const std::vector<std::string> options)
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

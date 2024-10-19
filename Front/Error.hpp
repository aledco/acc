#pragma once

#include <exception>
#include <string>
#include <vector>
#include <memory>
#include "Span.hpp"
#include "Lexer.hpp"

class Error : public std::exception
{
private:
    std::string header;
    std::string msg;
    std::string msg_override = "";

protected:
    Span span;
    inline virtual std::string get_message() const { return "around " + span.start.to_string() + " " + msg_override; }

public:
    Error(const Span& span) : span(span), header("Error") {}
    Error(const Span& span, std::string header) : span(span), header(header) {}
    Error(const Span& span, std::string header, std::string msg_override) : span(span), header(header), msg_override(msg_override) {}

    const char *what()
    {
        msg = header + ": " + get_message() + "\n";
        return msg.c_str();
    }
};

class SyntaxError : public Error 
{
protected:
    std::string get_message() const override;
public:
    SyntaxError(Point& pos) : Error(Span(pos, pos), "Syntax Error") {} 
};

class ParseError : public Error 
{
private:
    Token& actual;
    std::vector<std::string> token_types;

protected:
    std::string get_message() const override;
public:
    ParseError(Token&  actual, std::vector<std::string> token_types) : Error(actual.span, "Parse Error"), actual(actual), token_types(token_types) {}
    ParseError(Token& actual, std::initializer_list<std::string> token_types) : Error(actual.span, "Parse Error"), actual(actual), token_types(token_types) {} 
};

class NameError : public Error 
{
protected:
    std::string name;
public:
    NameError(Token &token) : Error(token.span, "Name Error"), name(token.value) {} 
};

class AlreadyDefinedNameError : public NameError 
{
protected:
    std::string get_message() const override;
public:
    AlreadyDefinedNameError(Token &token) : NameError(token) {} 
};

class UndefinedNameError : public NameError 
{
protected:
    std::string get_message() const override;
public:
    UndefinedNameError(Token &token) : NameError(token) {} 
};

class TypeError : public Error 
{
private:
    std::string type_error;
protected:
    std::string get_message() const override;
public:
    TypeError(Span span, std::string type_error) : Error(span, "Type Error"), type_error(type_error) {} 
};
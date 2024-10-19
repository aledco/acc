#include "Error.hpp"

std::string SyntaxError::get_message() const
{
    return "around " + span.start.to_string();
}

std::string ParseError::get_message() const
{
    std::string expected = "";
    for (auto i = 0; i < token_types.size(); i++)
    {
        expected += token_types[i]; 
        if (i < token_types.size() - 1)
        {
            expected +=  ", ";
        }
    }

    return "around " + span.start.to_string() + " expected " + expected + " got " + actual.value;
}

std::string AlreadyDefinedNameError::get_message() const
{
    return "around " + span.start.to_string() + " " + name + " already defined";
}

std::string UndefinedNameError::get_message() const
{
    return "around " + span.start.to_string() + " " + name + " is not defined";
}

std::string TypeError::get_message() const
{
    return "around " + span.start.to_string() + ": " + type_error;
}
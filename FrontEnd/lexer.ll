%{
    #include "lexer.hpp"
    #include "parser.tab.hpp"    

    #undef YY_DECL
    #define YY_DECL int Lexer::yylex(std::string *const yylval)   
%}

%option c++ noyywrap

%option yyclass="Lexer"

%%

[[:space:]] ;
Hello { return yy::parser::token::HELLO; }
[[:alpha:]]+ { *yylval = std::string(yytext, yytext + yyleng); return yy::parser::token::WORLD; }
. { return yytext[0]; }



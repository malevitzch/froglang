%define api.value.type {Tokens::Token}
//%define api.pure full
//%define parse.assert
//%define api.header.include {"tokens.hpp"}
%language "c++"
%parse-param {FrogLexer &lexer}

%code requires {
    struct Token;
    #include "tokens.hpp"
    #include "froglexer.hpp"
}
%code {
    #define yylex lexer.yylex
}

%{
%}

%token NUMBER

%%
program:
    | NUMBER
    ;
;

%%

void yy::parser::error(const std::string &message)
{
    std::cerr << "Error: " << message << std::endl;
}

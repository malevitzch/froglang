%define api.value.type {Token*}
%define api.parser.class {Parser}
//%define api.pure full
//%define parse.assert
//%define api.header.include {"tokens.hpp"}
%language "c++"

%code requires {
    #include "tokens.hpp"
    struct Token; // Forward declaration
    class yyFlexLexer;
    int yylex(Token** yylval);
    //int yylex(Token*& yylval, yyFlexLexer& lexer);
}
%code {
    #include <iostream>
}

%{
#include "tokens.hpp"
%}

%token NUMBER

%%
program:
    | NUMBER
    ;
;

%%

%define api.value.type {Token*}
%define api.parser.class {Parser}
%define api.pure full
%define parse.assert
//%define api.header.include {"tokens.hpp"}
%language "c++"

%code requires {
    #include <string>
    struct Token; // Forward declaration
    class yyFlexLexer; 
    int yylex(Token*& yylval, yyFlexLexer& lexer);
}
%code {
    #include "tokens.hpp"
    #include <iostream>
}

%{
#include "tokens.hpp"
%}

%%

program:
    { std::cout << "Program parsed successfully!" << std::endl; }
;

%%

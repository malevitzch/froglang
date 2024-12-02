
%define api.value.type {Tokens::Token}
%language "c++"
%parse-param {FrogLexer &lexer}

%code requires {
    #include "tokens.hpp"
    #include "froglexer.hpp"
}
%code {
    #define yylex lexer.yylex
}

%{

%}
//TODO: finish inputting tokens
%token NUMBER
%token IDENTIFIER
%token PLUS

%left PLUS

%%
program: number {std::cout<<"FINISHED\n";}
    ;

number: NUMBER {std::cout<<"Converted\n";}
    | number PLUS number {std::cout<<"Added\n";}
%%

void yy::parser::error(const std::string &message)
{
    std::cerr << "Error: " << message << std::endl;
}


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
%token MINUS
%token STAR
%token SLASH

%token LBRACE
%token RBRACE
%token SEMICOLON

%left PLUS MINUS
%left STAR SLASH

%%
block: LBRACE statements RBRACE {std::cout<<"FINISHED\n";}
    ;
statements: {std::cout<<"CONJURED STATEMENTS\n";}
    | statement statements {std::cout<<"combined\n";}
    ;
statement: expression SEMICOLON {std::cout<<"STATEMENT\n";}
    ;
expression: number {std::cout<<"EXPR\n";}
    ;
number: NUMBER {std::cout<<"Converted\n";}
    | number PLUS number {std::cout<<"Added\n";}
    | number MINUS number {std::cout<<"Subtracted\n";}
    | number STAR number {std::cout<<"Multiplied\n";}
    | number SLASH number {std::cout<<"Divided\n";}
%%

void yy::parser::error(const std::string &message)
{
    std::cerr << "Error: " << message << std::endl;
}

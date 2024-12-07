
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

%token FUNCTION

%token LPAREN
%token RPAREN

%token LBRACE
%token RBRACE
%token SEMICOLON

%left PLUS MINUS
%left STAR SLASH

%%
program: global_obj YYEOF {std::cout<<"FINISHED\n";}
    ;
global_obj: function {std::cout<<"DECLARATION\n";}
    ;
function: function_declaration block {std::cout<<"FUNC";}
    ;
arglist: LPAREN RPAREN {std::cout<<"FUNCTION ARGS\n";}
    ;
function_declaration: FUNCTION arglist {std::cout<<"DECLARED\n";}
    ;
block: LBRACE statements RBRACE {std::cout<<"BLOCK\n";}
    ;
statements: /* empty */
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
    ;
%%

void yy::parser::error(const std::string &message)
{
    std::cerr << "Error: " << message << std::endl;
}

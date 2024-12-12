
%define api.value.type {Tokens::Token}
%language "c++"
%parse-param {FrogLexer &lexer}
%define parse.error verbose

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
program: global_obj {std::cout<<"FINISHED\n";}
    ;

global_obj: function {std::cout<<"DECLARATION\n";}
    ;
function: function_declaration block {std::cout<<"FUNC\n";}
    ;
function_declaration: FUNCTION IDENTIFIER arglist {std::cout<<"DECLARED\n";}
    ;

block: LBRACE statements RBRACE {std::cout<<"BLOCK\n";}
    ;
statements: /* empty */ {std::cout<<"CONJURED STATEMENTS\n";}
    | statement statements {std::cout<<"combined\n";}
    ;
statement: expression SEMICOLON {std::cout<<"STATEMENT\n";}
    ;
expression: NUMBER {std::cout<<"Converted\n";}
    | expression PLUS expression {std::cout<<"Added\n";}
    | expression MINUS expression {std::cout<<"Subtracted\n";}
    | expression STAR expression {std::cout<<"Multiplied\n";}
    | expression SLASH expression {std::cout<<"Divided\n";}
    ;
arglist: LPAREN RPAREN {std::cout<<"FUNCTION ARGS\n";}
    ;
%%

void yy::parser::error(const std::string &message)
{
    std::cerr << "Error: " << message << std::endl;
}

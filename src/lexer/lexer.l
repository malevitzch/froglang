%{
#include "tokens.hpp"
#include "parser.hpp"
#include "froglexer.hpp"

#include <iostream>
#include <fstream>
#include <map>
#include <memory>

using namespace Tokens;

std::map<std::string, Tokens::TokType> keywords = {
  {"if", Tokens::TokType::IF},
  {"loop", Tokens::TokType::LOOP},
  {"return", Tokens::TokType::RETURN},
};
#undef  YY_DECL
#define YY_DECL int FrogLexer::yylex(Tokens::Token *const yylval)
%}

%option c++
%option noyywrap
%option yyclass="FrogLexer"

%%

[a-zA-Z_]+[a-zA-Z_0-9]* {
  if(keywords.contains(yytext)) {
    std::cout << "Keyword : [" << yytext << "]\n";
    *yylval = Tokens::Token(keywords[yytext]);
    //TODO: return the right token here
  }
  else {
    std::cout << "IDENTIFIER(" << yytext << ")\n";
    *yylval = Tokens::Token(Tokens::TokType::IDENTIFIER, yytext);
    return yy::parser::token::IDENTIFIER;
  }
}
[0-9]+ {
  std::cout << "NUMBER(" << yytext << ")\n";
  *yylval = Tokens::Token(Tokens::TokType::NUMBER, yytext);
  return yy::parser::token::NUMBER;
}
"->" {
  std::cout << "ARROW\n";
  *yylval = Tokens::Token(Tokens::TokType::ARROW, yytext);
}
"+" { 
  std::cout << "PLUS\n";
  *yylval = Tokens::Token(Tokens::TokType::PLUS, yytext);
  return yy::parser::token::PLUS;
}
"-" {
  std::cout << "MINUS\n"; 
  *yylval = Tokens::Token(Tokens::TokType::MINUS, yytext);
}
"*" {
  std::cout << "STAR\n";
  *yylval = Tokens::Token(Tokens::TokType::STAR, yytext);
}
"/" {
  std::cout << "SLASH\n";
  *yylval = Tokens::Token(Tokens::TokType::SLASH, yytext);
}
"=" {
  std::cout << "ASSIGNMENT\n";
  *yylval = Tokens::Token(Tokens::TokType::ASSIGNMENT, yytext);
}
"(" {
  std::cout << "LPAREN\n"; 
  *yylval = Tokens::Token(Tokens::TokType::LPAREN, yytext);
}
")" {
  std::cout << "RPAREN\n"; 
  *yylval = Tokens::Token(Tokens::TokType::RPAREN, yytext);
}
"{" {
  std::cout << "LBRACE\n";
  *yylval = Tokens::Token(Tokens::TokType::LBRACE, yytext);
}
"}" {
  std::cout << "RBRACE\n"; 
  *yylval = Tokens::Token(Tokens::TokType::RBRACE, yytext);
}
"," {
  std::cout << "COMMA\n";
  *yylval = Tokens::Token(Tokens::TokType::COMMA, yytext);
}
":" {
  std::cout << "COLON\n";
  *yylval = Tokens::Token(Tokens::TokType::COLON, yytext);
}
";" {
  std::cout << "SEMICOLON\n";
  *yylval = Tokens::Token(Tokens::TokType::SEMICOLON, yytext);
}
[ \t\n]+ {
  /* Ignore whitespace */ 
}

.   {
  std::cout << "UNKNOWN TOKEN: " << yytext << "\n"; 
}

%%


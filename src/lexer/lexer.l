%{
#include "tokens.hpp"
#include "parser.hpp"

#include "froglexer.hpp"

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <memory>

#undef  YY_DECL
#define YY_DECL int FrogLexer::yylex(FrogTok *const yylval)

using namespace Tokens;

const std::unordered_map<std::string, std::pair<Tokens::TokType, int> > keywords = {
  {"return", {Tokens::TokType::RETURN, yy::parser::token::RETURN}},
  {"function", {Tokens::TokType::FUNCTION, yy::parser::token::FUNCTION}},
  {"if", {Tokens::TokType::IF, yy::parser::token::IF}},
  {"else", {Tokens::TokType::ELSE, yy::parser::token::ELSE}},
  {"true", {Tokens::TokType::TRUE, yy::parser::token::TRUE}},
  {"false", {Tokens::TokType::FALSE, yy::parser::token::FALSE}},
  {"while", {Tokens::TokType::WHILE, yy::parser::token::WHILE}},
};

std::ostream* diagnostic_stream = &std::cout;

std::unordered_set<std::string> types = {
  "i1",
  "i32",
  "f32",
};
%}

%option c++
%option noyywrap
%option yyclass="FrogLexer"

%x DECL
%%

[a-zA-Z_]+[a-zA-Z_0-9]* {
  if(keywords.contains(yytext)) {
    auto&[type, yy_type] = keywords.at(yytext);
    yylval->token = std::make_shared<Tokens::Token>(type, yytext);
    return yy_type; 
  }
  else if(types.contains(yytext)) {
    yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::TYPE_ID, yytext);
    return yy::parser::token::TYPE_ID;
  }
  else {
    yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::IDENTIFIER, yytext);
    return yy::parser::token::IDENTIFIER;
  }
}
[0-9]+ {
  yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::NUMBER, yytext);
  return yy::parser::token::NUMBER;
}
"->" {
  yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::ARROW, yytext);
  return yy::parser::token::ARROW;
}
"+" { 
  yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::PLUS, yytext);
  return yy::parser::token::PLUS;
}
"-" {
  yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::MINUS, yytext);
  return yy::parser::token::MINUS;
}
"*" {
  yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::STAR, yytext);
  return yy::parser::token::STAR;
}
"/" {
  yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::SLASH, yytext);
  return yy::parser::token::SLASH;
}
"=" {
  yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::ASSIGNMENT, yytext);
  return yy::parser::token::ASSIGNMENT;
}
"(" {
  yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::LPAREN, yytext);
  return yy::parser::token::LPAREN;
}
")" {
  yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::RPAREN, yytext);
  return yy::parser::token::RPAREN;
}
"{" {
  yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::LBRACE, yytext);
  return yy::parser::token::LBRACE;
}
"}" {
  yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::RBRACE, yytext);
  return yy::parser::token::RBRACE;
}
"[" {
  yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::LBRACKET, yytext);
  return yy::parser::token::LBRACKET;
}
"]" {
  yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::RBRACKET, yytext);
  return yy::parser::token::RBRACKET;
}
"," {
  yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::COMMA, yytext);
  return yy::parser::token::COMMA;
}
":" {
  yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::COLON, yytext);
  return yy::parser::token::COLON;
}
";" {
  yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::SEMICOLON, yytext);
  return yy::parser::token::SEMICOLON;
}
"<" {
  yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::LESS, yytext);
  return yy::parser::token::LESS;
}
">" {
  yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::GREATER, yytext);
  return yy::parser::token::GREATER;
}
"<=" {
  yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::LESSEQ, yytext);
  return yy::parser::token::LESSEQ;
}
">=" {
  yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::GREATEREQ, yytext);
  return yy::parser::token::GREATEREQ;
}
"==" {
  yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::EQUALITY, yytext);
  return yy::parser::token::EQUALITY;
}
"!=" {
  yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::INEQUALITY, yytext);
  return yy::parser::token::INEQUALITY;
}
"&&" {
  yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::LOGICAL_AND, yytext);
  return yy::parser::token::LOGICAL_AND;
}
"||" {
  yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::LOGICAL_OR, yytext);
  return yy::parser::token::LOGICAL_OR;
}
"~" {
  yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::NEGATION, yytext);
  return yy::parser::token::NEGATION;
}

[ \t\n]+ {
  /* Ignore whitespace */ 
}

.   {
  *diagnostic_stream << "UNKNOWN TOKEN: " << yytext << "\n"; 
}

%%


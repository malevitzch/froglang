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
};

std::ostream* diagnostic_stream = &std::cout;

std::unordered_set<std::string> types = {
  "i32",
  "f32",
  "bool",
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
    *diagnostic_stream << "Keyword : [" << yytext << "]\n";
    yylval->token = std::make_shared<Tokens::Token>(type, yytext);
    return yy_type; 
  }
  else if(types.contains(yytext)) {
    *diagnostic_stream << "TYPE_ID(" << yytext << ")\n";
    yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::TYPE_ID, yytext);
    return yy::parser::token::TYPE_ID;
  }
  else {
    *diagnostic_stream << "IDENTIFIER(" << yytext << ")\n";
    yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::IDENTIFIER, yytext);
    return yy::parser::token::IDENTIFIER;
  }
}
[0-9]+ {
  *diagnostic_stream << "NUMBER(" << yytext << ")\n";
  yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::NUMBER, yytext);
  return yy::parser::token::NUMBER;
}
"->" {
  *diagnostic_stream << "ARROW\n";
  yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::ARROW, yytext);
  return yy::parser::token::ARROW;
}
"+" { 
  *diagnostic_stream << "PLUS\n";
  yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::PLUS, yytext);
  return yy::parser::token::PLUS;
}
"-" {
  *diagnostic_stream << "MINUS\n"; 
  yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::MINUS, yytext);
  return yy::parser::token::MINUS;
}
"*" {
  *diagnostic_stream << "STAR\n";
  yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::STAR, yytext);
  return yy::parser::token::STAR;
}
"/" {
  *diagnostic_stream << "SLASH\n";
  yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::SLASH, yytext);
  return yy::parser::token::SLASH;
}
"=" {
  *diagnostic_stream << "ASSIGNMENT\n";
  yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::ASSIGNMENT, yytext);
  return yy::parser::token::ASSIGNMENT;
}
"(" {
  *diagnostic_stream << "LPAREN\n"; 
  yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::LPAREN, yytext);
  return yy::parser::token::LPAREN;
}
")" {
  *diagnostic_stream << "RPAREN\n"; 
  yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::RPAREN, yytext);
  return yy::parser::token::RPAREN;
}
"{" {
  *diagnostic_stream << "LBRACE\n";
  yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::LBRACE, yytext);
  return yy::parser::token::LBRACE;
}
"}" {
  *diagnostic_stream << "RBRACE\n"; 
  yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::RBRACE, yytext);
  return yy::parser::token::RBRACE;
}
"," {
  *diagnostic_stream << "COMMA\n";
  yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::COMMA, yytext);
  return yy::parser::token::COMMA;
}
":" {
  *diagnostic_stream << "COLON\n";
  yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::COLON, yytext);
  return yy::parser::token::COLON;
}
";" {
  *diagnostic_stream << "SEMICOLON\n";
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
"==" {
  yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::EQUALITY, yytext);
  return yy::parser::token::EQUALITY;
}
"!=" {
  yylval->token = std::make_shared<Tokens::Token>(Tokens::TokType::INEQUALITY, yytext);
  return yy::parser::token::INEQUALITY;
}
[ \t\n]+ {
  /* Ignore whitespace */ 
}

.   {
  *diagnostic_stream << "UNKNOWN TOKEN: " << yytext << "\n"; 
}

%%


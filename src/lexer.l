%{
#include "tokens.hpp"
#include "parser.hpp"

#include <iostream>
#include <fstream>
#include <map>
#include <memory>

using namespace Tokens;

FlexLexer* lexer = nullptr;
extern Token* yylval;

std::map<std::string, TokType> keywords = {
  {"if", TokType::IF},
  {"loop", TokType::LOOP},
  {"return", TokType::RETURN},
};
%}

%option c++
%option noyywrap

%%

[a-zA-Z_]+[a-zA-Z_0-9]* {
  if(keywords.contains(yytext)) {
    std::cout << "Keyword : [" << yytext << "]\n";
    yylval = new Token(keywords[yytext]);
  }
  else {
    std::cout << "IDENTIFIER(" << yytext << ")\n";
    yylval = new Token(TokType::IDENTIFIER, yytext);
  }
}
[0-9]+ {
  std::cout << "NUMBER(" << yytext << ")\n";
  yylval = new Token(TokType::NUMBER, yytext);
}
"->" {
  std::cout << "ARROW\n";
  yylval = new Token(TokType::ARROW, yytext);
}
"+" { 
  std::cout << "PLUS\n";
  yylval = new Token(TokType::PLUS, yytext);
}
"-" {
  std::cout << "MINUS\n"; 
  yylval = new Token(TokType::MINUS, yytext);
}
"*" {
  std::cout << "STAR\n";
  yylval = new Token(TokType::STAR, yytext);
}
"/" {
  std::cout << "SLASH\n";
  yylval = new Token(TokType::SLASH, yytext);
}
"=" {
  std::cout << "ASSIGNMENT\n";
  yylval = new Token(TokType::ASSIGNMENT, yytext);
}
"(" {
  std::cout << "LPAREN\n"; 
  yylval = new Token(TokType::LPAREN, yytext);
}
")" {
  std::cout << "RPAREN\n"; 
  yylval = new Token(TokType::RPAREN, yytext);
}
"{" {
  std::cout << "LBRACE\n";
  yylval = new Token(TokType::LBRACE, yytext);
}
"}" {
  std::cout << "RBRACE\n"; 
  yylval = new Token(TokType::RBRACE, yytext);
}
"," {
  std::cout << "COMMA\n";
  yylval = new Token(TokType::COMMA, yytext);
}
":" {
  std::cout << "COLON\n";
  yylval = new Token(TokType::COLON, yytext);
}
";" {
  std::cout << "SEMICOLON\n";
  yylval = new Token(TokType::SEMICOLON, yytext);
}
[ \t\n]+ {
  /* Ignore whitespace */ 
}

.   {
  std::cout << "UNKNOWN TOKEN: " << yytext << "\n"; 
}

%%

void lex_file(std::string filename) {
  std::shared_ptr<std::ifstream> in = std::make_shared<std::ifstream>(std::ifstream(filename));
  if(!in->is_open()) {
    std::cerr << "Cannot open file: " << filename << "\n";
    return;
  }
  lexer = new yyFlexLexer(in.get());

  while(lexer->yylex() != 0) {}

  delete lexer;
}

int main(int argc, char** argv) {
  std::istream* in;
  std::ifstream* file;
  if(argc > 1) {
    // If a file is provided, open it and set as input stream
    file = new std::ifstream(argv[1]);
    if(!file->is_open()) {
      std::cerr << "Cannot open file: " << argv[1] << "\n";
      return 1;
    }
    in = file;
  } else {
    std::cerr << "No file input given\n";
    return 0;
  }

  // Create a lexer object
  lexer = new yyFlexLexer(in);
  // Call the lexer
  while(lexer->yylex() != 0) {
  }

  delete lexer;
  delete file;
  return 0;
}

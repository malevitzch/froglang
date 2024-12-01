#include <iostream>
#include <memory>
#include <fstream>
#include "froglexer.hpp"
#include "tokens.hpp"
#include "parser.hpp"
void lex_file(std::string filename) {
  std::shared_ptr<std::ifstream> in = std::make_shared<std::ifstream>(std::ifstream(filename));
  if(!in->is_open()) {
    std::cerr << "Cannot open file: " << filename << "\n";
    return;
  }
}

int main(int argc, char** argv) {
  std::istream* in;
  std::ifstream file;
  if(argc > 1) {
    // If a file is provided, open it and set as input stream
    file = std::ifstream(argv[1]);
    if(!file.is_open()) {
      std::cerr << "Cannot open file: " << argv[1] << "\n";
      return 1;
    }
    //in = file;
  } else {
    std::cerr << "No file input given\n";
    return 0;
  }

  // Create a lexer object
  FrogLexer lexer(&file);
  // Call the lexer
  Tokens::Token *yylval = new Tokens::Token();
  yy::parser parser(lexer);
  parser();
  /*while(lexer.yylex(yylval)) {
  }*/

  return 0;
}

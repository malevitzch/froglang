#include <iostream>
#include <memory>
#include <fstream>
#include <FlexLexer.h>
#include "tokens.hpp"
#include "parser.hpp"

std::shared_ptr<ast::Node> ast_root;

void lex_file(std::string filename) {
  std::shared_ptr<std::ifstream> in = std::make_shared<std::ifstream>(std::ifstream(filename));
  if(!in->is_open()) {
    std::cerr << "Cannot open file: " << filename << "\n";
    return;
  }
}

void traverse(std::shared_ptr<ast::Node> node, int depth, std::ostream& out) {
  for(int i = 0; i < depth; i++) out << "  ";
  out << node->get_name();
  if(node->final) {
    out << "\n";
    return;
  }
  out << " {\n";
  for(std::shared_ptr<ast::Node> child : node->get_children()) traverse(child, depth+1, out);
  for(int i = 0; i < depth; i++) out << "  ";
  out<<"}\n";
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
  //std::shared_ptr<ast::Node> root;
  yy::parser p(lexer);
  p();
  std::ofstream ast_out("tree_output.txt");
  traverse(ast_root, 0, ast_out);
  /*while(lexer.yylex(yylval)) {
  }*/

  return 0;
}

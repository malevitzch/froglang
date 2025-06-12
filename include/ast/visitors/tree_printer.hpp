#ifndef AST_VISITORS_TREE_PRINTER_HPP
#define AST_VISITORS_TREE_PRINTER_HPP

#include "ast.hpp"

namespace ast {
  class TreePrinter {
  private:
    std::ostream* output_stream;
  public:
    TreePrinter(std::ostream* output_stream);
  };
}

#endif

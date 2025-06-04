#ifndef AST_VISITORS_TREE_PRINTER_HPP
#define AST_VISITORS_TREE_PRINTER_HPP

#include "ast.hpp"

namespace ast {
  class TreePrinter {
  private:
  public:
    void codegen(ProgramNode& program_node);
  };
}

#endif

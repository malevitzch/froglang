#ifndef AST_VISITORS_TREE_VISITOR_HPP
#define AST_VISITORS_TREE_VISITOR_HPP

#include "ast.hpp"

namespace ast {
  class TreeVisitor {
  private:
  public:
    void visit_node(Node& node);
    virtual void visit_program_node(ProgramNode& node) = 0;
  };
}

#endif

#ifndef AST_VISITORS_TREE_VISITOR_HPP
#define AST_VISITORS_TREE_VISITOR_HPP

#include "ast.hpp"

namespace ast {
  class TreeVisitor {
  private:
  public:
    void visit_node(Node& node);
    void visit_node(std::shared_ptr<Node> node);
    virtual void default_visit(Node& node) = 0;
    virtual void visit_program_node(ProgramNode& node) = 0;
    virtual void visit_function_declaration_node(FunctionDeclaration& node) = 0;
  };
}

#endif

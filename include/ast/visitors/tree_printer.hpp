#ifndef AST_VISITORS_TREE_PRINTER_HPP
#define AST_VISITORS_TREE_PRINTER_HPP

#include "ast.hpp"

#include "ast/visitors/tree_visitor.hpp"

namespace ast {


  class TreePrinter : public TreeVisitor {
  private:
    std::ostream* output_stream;
    int indent_depth = 0;
    std::string indent();
  public:
    TreePrinter(std::ostream* output_stream);
    virtual void visit_program_node(ProgramNode& program_node) override;
  };
}

#endif

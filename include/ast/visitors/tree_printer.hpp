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
    void line(std::initializer_list<std::string> text);
    void line(std::string text);
  public:
    TreePrinter(std::ostream* output_stream);
    virtual void default_visit(Node& node) override;
    virtual void visit_program_node(ProgramNode& program_node) override;
  };
}

#endif

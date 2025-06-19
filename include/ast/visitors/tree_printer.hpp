#ifndef AST_VISITORS_TREE_PRINTER_HPP
#define AST_VISITORS_TREE_PRINTER_HPP
#include <stack>

#include "ast.hpp"

#include "ast/visitors/tree_visitor.hpp"

namespace ast {


  class TreePrinter : public TreeVisitor {
  private:
    std::ostream* output_stream;
    int indent_depth = 0;

    int indents_to_skip = 0;
    std::stack<bool> indents;

    std::string indentation();
    void indent();
    void unindent();
    void skip_indent();

    void write(std::initializer_list<std::string> text);
    void write(std::string text);
    void line(std::initializer_list<std::string> text);
    void line(std::string text);
  public:
    TreePrinter(std::ostream* output_stream);
    virtual void default_visit(Node& node) override;
    virtual void visit_program_node(ProgramNode& program_node) override;
    virtual void visit_function_declaration_node(FunctionDeclaration& node) override;
    virtual void visit_function_node(FunctionGlobject& node) override;

    virtual void visit_declaration_node(DeclarationNode& node) override;
    virtual void visit_block_node(Block& node) override;

    virtual void visit_binary_operator_node(BinaryOperator& node) override;
  };
}

#endif

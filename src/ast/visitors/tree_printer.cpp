#include "ast/visitors/tree_printer.hpp"

namespace ast {

  std::string TreePrinter::indentation() {
    return std::string(indent_depth, '\t');
  }
  void TreePrinter::indent() {
    indent_depth++;
  }
  void TreePrinter::unindent() {
    indent_depth--;
  }
  void TreePrinter::write(std::initializer_list<std::string> text) {
    for(std::string s : text) {
      *output_stream << s;
    }
  }
  void TreePrinter::write(std::string text) {
    write({text});
  }
  void TreePrinter::line(std::initializer_list<std::string> text) {
    *output_stream << indentation();
    write(text);
    *output_stream << "\n";
  }
  void TreePrinter::line(std::string text) {
    line({text});
  }
  TreePrinter::TreePrinter(std::ostream* output_stream) : output_stream(output_stream) {}
  void TreePrinter::default_visit(Node& node) {
    line("Unknown Node");
  }
  void TreePrinter::visit_program_node(ProgramNode& node) {
    line("Program Node");
    indent();
    for(std::shared_ptr<Node> child : node.get_children()) {
      visit_node(child);
    }
    unindent();
  }
  void TreePrinter::visit_function_declaration_node(FunctionDeclaration& node) {
    //FIXME: maybe no need for friend, maybe just expose the info
    // in the nodes themselves (here it can be done easily)
    line({"Declaration of function \"", node.function_name, "\""});
    indent();
    //FIXME: there is no type wrapper yet unfortunately so it's just raw LLVM types
    line({"Return type: ", "UNIMPLEMENTED"});
    line("Arguments:");
    indent();
    for(auto& arg : node.get_args()) {
      arg->accept_visitor(*this);
    }
    unindent();
    unindent();
  }
  void TreePrinter::visit_function_node(FunctionGlobject& node) {
    line({"Function ",  "\"", node.decl->get_varname(), "\""});
    indent();
    visit_node(*(node.decl));
    visit_node(*(node.body));
    unindent();
  }
  void TreePrinter::visit_declaration_node(DeclarationNode& node) {
    line({node.get_varname(), ": ", "UNIMPLEMENTED"});
  }
}

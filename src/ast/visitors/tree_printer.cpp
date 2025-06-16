#include "ast/visitors/tree_printer.hpp"

namespace ast {

  std::string TreePrinter::indent() {
    return std::string(indent_depth, '\t');
  }
  void TreePrinter::line(std::initializer_list<std::string> text) {
    *output_stream << indent();
    for(std::string s : text) {
      *output_stream << s;
    }
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
    indent_depth++;
    for(std::shared_ptr<Node> child : node.get_children()) {
      visit_node(child);
    }
    indent_depth--;
  }
  void TreePrinter::visit_function_declaration_node(FunctionDeclaration& node) {
    //FIXME: maybe no need for friend, maybe just expose the info
    // in the nodes themselves (here it can be done easily)
    line({"Declaration of function \"", node.function_name, "\""});
    //FIXME: there is no type wrapper yet unfortunately so it's just raw LLVM types
    line({"Return type: ", "UNIMPLEMENTED"});
    line("Arguments:");
    indent_depth++;
    for(auto& arg : node.get_args()) {
      arg->accept_visitor(*this);
    }
    indent_depth--;
  }
}

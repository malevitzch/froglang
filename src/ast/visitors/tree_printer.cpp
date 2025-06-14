#include "ast/visitors/tree_printer.hpp"

namespace ast {

  std::string TreePrinter::indent() {
    return std::string(indent_depth, '\t');
  }
  void TreePrinter::line(std::initializer_list<std::string> text) {
    for(std::string s : text) {
      *output_stream << indent() << s;
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
}

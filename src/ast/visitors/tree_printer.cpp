#include "ast/visitors/tree_printer.hpp"

namespace ast {

  std::string TreePrinter::indent() {
    return std::string(indent_depth, '\t');
  }
  void TreePrinter::line(std::initializer_list<std::string> text) {
    indent();
    for(std::string s : text)
      (*output_stream) << s;
  }
  void TreePrinter::line(std::string text) {
    line({text});
  }
  TreePrinter::TreePrinter(std::ostream* output_stream) : output_stream(output_stream) {}
  void TreePrinter::visit_program_node(ProgramNode& node) {
    line("Program Node");
  }
}

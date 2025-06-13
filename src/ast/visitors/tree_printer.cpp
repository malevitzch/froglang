#include "ast/visitors/tree_printer.hpp"

namespace ast {

  std::string TreePrinter::indent() {
    return std::string(indent_depth, '\t');
  }
  TreePrinter::TreePrinter(std::ostream* output_stream) : output_stream(output_stream) {}
  void TreePrinter::visit_program_node(ProgramNode& node) {
    *output_stream << indent() << "Program Node";
  }
}

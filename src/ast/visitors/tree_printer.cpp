#include "ast/visitors/tree_printer.hpp"

namespace ast {
  TreePrinter::TreePrinter(std::ostream* output_stream) : output_stream(output_stream) {}
  void TreePrinter::visit_program_node(ProgramNode& node) {
    *output_stream << "Program Node";
  }
}

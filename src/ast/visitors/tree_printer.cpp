#include "ast/visitors/tree_printer.hpp"

namespace ast {
  TreePrinter::TreePrinter(std::ostream* output_stream) : output_stream(output_stream) {}
}

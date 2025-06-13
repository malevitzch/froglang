#include "ast/visitors/tree_visitor.hpp"

namespace ast {
  void TreeVisitor::visit_node(Node& node) {
    node.accept_visitor(*this);
  }
}

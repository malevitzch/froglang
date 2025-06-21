#include "ast/visitors/tree_visitor.hpp"

namespace ast {
  void TreeVisitor::visit_node(Node& node) {
    node.accept_visitor(*this);
  }
  void TreeVisitor::visit_node(Node* node) {
    visit_node(*node);
  }
  void TreeVisitor::visit_node(std::shared_ptr<Node> node) {
    visit_node(*node);
  }
}

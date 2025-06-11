#include "ast/node.hpp"
#include "ast/visitors/tree_visitor.hpp"

namespace ast {
  void Node::accept_visitor(TreeVisitor& visitor) {
    //FIXME: this should do something like throw unimplemented method
  }
  std::string Node::get_name() {
    return "Uninitialized Name";
  }
  std::vector<std::shared_ptr<Node>> Node::get_children() {
    throw std::runtime_error(
      "The method get_children() is not implemented for type "
      "\"" + get_name() + "\"");
  }

}

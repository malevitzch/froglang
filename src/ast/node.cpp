#include "ast/node.hpp"

namespace ast {
  std::string Node::get_name() {
    return "Uninitialized Name";
  }
  //TODO: consider this throwing an error or emitting a warning
  std::vector<std::shared_ptr<Node>> Node::get_children() {
    throw std::runtime_error(
      "The method get_children() is not implemented for type "
      "\"" + get_name() + "\"");
  }

}

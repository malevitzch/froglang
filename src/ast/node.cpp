#include "ast/node.hpp"

namespace ast {
  void Node::codegen() {
    throw std::runtime_error(
    "The method codegen() is not implemented for type \"" + get_name() + "\"");
  }
  std::string Node::get_name() {
    return "Uninitialized Name";
  }
  //TODO: consider this throwing an error or emitting a warning
  std::vector<std::shared_ptr<Node>> Node::get_children() {
    throw std::runtime_error(
    "The method get_children() is not implemented for type \"" + get_name() + "\"");
  }

  DeclarationNode::DeclarationNode(std::string var_type, std::string var_name) 
  : var_type(var_type), var_name(var_name) {}
  std::string DeclarationNode::get_name() {
    return "Declaration Node";
  }
  std::vector<std::shared_ptr<Node>> DeclarationNode::get_children() {
    return {};
  }
}

#include "ast/node.hpp"

namespace ast {

  std::string Node::get_name() {
    return "Uninitialized Name";
  }
  //TODO: consider this throwing an error or emitting a warning
  std::vector<std::shared_ptr<Node>> Node::get_children() {
    throw std::runtime_error(
    "The method get_children() is not implemented for type \"" + get_name() + "\"");
  }

  llvm::Value* GlobjectNode::codegen() {
    //TODO: implement
  }
  std::string GlobjectNode::get_name() {
    return "Globject Node";
  }


  llvm::Value* ProgramNode::codegen() {
    //TODO: implement
  }
  std::string ProgramNode::get_name() {
    return "Program Node";
  }
  void ProgramNode::add_obj(std::shared_ptr<GlobjectNode> globject) {
    globjects.push_back(globject);
  }
  ProgramNode::ProgramNode() {}
  std::vector<std::shared_ptr<Node>> ProgramNode::get_children() {
    return globjects;
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

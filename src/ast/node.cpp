#include "ast/node.hpp"

namespace ast {

  std::string Node::get_name() {
    return "Uninitialized Name";
  }
  llvm::Value* GlobjectNode::codegen() {
    //TODO: implement
  }
  GlobjectNode::GlobjectNode() {}
  std::string GlobjectNode::get_name() {
    return "Globject Node";
  }


  llvm::Value* ProgramNode::codegen() {
    //TODO: implement
  }
  std::string ProgramNode::get_name() {
    return "Program Node";
  }
  void ProgramNode::add_obj(GlobjectNode* globject) {
    globjects.push_back(globject);
  }

  ProgramNode::ProgramNode() {}
}

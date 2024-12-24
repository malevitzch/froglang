#include "ast/node.hpp"

namespace ast {

  std::string Node::get_name() {
    return "Uninitialized Name";
  }
  llvm::Value* GlobjectNode::codegen() {
    //TODO: implement
  }
  GlobjectNode::GlobjectNode() {}


  llvm::Value* ProgramNode::codegen() {
    //TODO: implement
  }

  void ProgramNode::add_obj(GlobjectNode* globject) {
    globjects.push_back(globject);
  }

  ProgramNode::ProgramNode() {}
}

#include "ast/node.hpp"

Node::Node(std::string name) : name(name) {}

std::string Node::get_name() {
  return name;
}
llvm::Value* GlobjectNode::codegen() {
  //TODO: implement
}
GlobjectNode::GlobjectNode() : Node("Global Object") {}


llvm::Value* ProgramNode::codegen() {
  //TODO: implement
}

void ProgramNode::add_obj(GlobjectNode* globject) {
  globjects.push_back(globject);
}

ProgramNode::ProgramNode() : Node("Program Node") {}

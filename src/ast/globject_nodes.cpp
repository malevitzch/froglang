#include "ast/globject_nodes.hpp"

namespace ast {

  void GlobjectNode::codegen() {
    //TODO: implement
  }
  std::string GlobjectNode::get_name() {
    return "Globject Node";
  }

  void ProgramNode::codegen() {
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
  std::vector<std::shared_ptr<Node>> children;
    for(std::shared_ptr<GlobjectNode> globject : globjects) children.push_back(static_pointer_cast<Node>(globject));
    return children;
  }

  FunctionGlobject::FunctionGlobject(std::shared_ptr<FunctionDeclaration> decl, std::shared_ptr<Block> body) 
  : decl(decl), body(body) {}
}

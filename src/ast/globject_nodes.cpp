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
    for(std::shared_ptr<GlobjectNode> globject : globjects) 
      children.push_back(static_pointer_cast<Node>(globject));
    return children;
  }

  FunctionArgs::FunctionArgs() {
    final = true;
  }
  void FunctionArgs::add_arg(std::shared_ptr<DeclarationNode> arg) {
    args.push_back(arg);
  }
  std::string FunctionArgs::get_name() {
    return "Function Args";
  }

  FunctionDeclaration::FunctionDeclaration(std::string name, std::shared_ptr<FunctionArgs> args, std::string return_type) 
  : name(name), args(args), return_type(return_type) {}
  void FunctionDeclaration::codegen() {
  }
  std::string FunctionDeclaration::get_name() {
    return "Function Declaration";
  }
 
  FunctionGlobject::FunctionGlobject(std::shared_ptr<FunctionDeclaration> decl, std::shared_ptr<Block> body) 
  : decl(decl), body(body) {}
  std::string FunctionGlobject::get_name() {
    return "Function Globject";
  }
  std::vector<std::shared_ptr<Node>> FunctionGlobject::get_children() {
    return {decl, body};
  }
}

#include "ast/globject_nodes.hpp"

namespace ast {

  std::string GlobjectNode::get_name() {
    return "Globject Node";
  }

  void ProgramNode::codegen() {
    for(std::shared_ptr<GlobjectNode> globject : globjects)
      globject->codegen();
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

  std::string FunctionArgs::get_name() {
    return "Function Args";
  }
  std::vector<std::shared_ptr<Node>> FunctionArgs::get_children() {
    std::vector<std::shared_ptr<Node>> children;
    for(std::shared_ptr<DeclarationNode> arg : args) 
      children.push_back(arg);
    return children;
  }
  void FunctionArgs::add_arg(std::shared_ptr<DeclarationNode> arg) {
    args.push_back(arg);
  }
  std::vector<std::shared_ptr<DeclarationNode>> FunctionArgs::get_args() {
    return args;
  }
  std::vector<std::shared_ptr<DeclarationNode>> FunctionArglist::get_args() {
    return args->get_args();
  }

  FunctionArglist::FunctionArglist(std::shared_ptr<FunctionArgs> args)
  : args(args) {}
  std::string FunctionArglist::get_name() {
    return "Function Arglist";
  }
  std::vector<std::shared_ptr<Node>> FunctionArglist::get_children() {
    return {args};
  }
  std::vector<llvm::Type*> FunctionArglist::get_arg_types() {
    std::vector<llvm::Type*> types;
    for(std::shared_ptr<DeclarationNode> arg : get_args()) {
      types.push_back(arg->get_var_type());
    }
    return types;
  }

  FunctionDeclaration::FunctionDeclaration(std::string name, std::shared_ptr<FunctionArglist> args, llvm::Type* return_type) 
  : name(name), args(args), return_type(return_type) {}
  void FunctionDeclaration::codegen() {
  }
  llvm::Function* FunctionDeclaration::get_func() {

    std::vector<llvm::Type*> arg_types = args->get_arg_types();

    llvm::FunctionType* func_type =
      llvm::FunctionType::get(return_type, arg_types, false);

    llvm::Function* this_function =
      llvm::Function::Create(func_type, llvm::Function::ExternalLinkage, name, CompilerContext::TheModule.get());
    return this_function;

  }
  std::string FunctionDeclaration::get_name() {
    return "Function Declaration";
  }
  std::vector<std::shared_ptr<Node>> FunctionDeclaration::get_children() {
    return {args};
  }

  FunctionGlobject::FunctionGlobject(std::shared_ptr<FunctionDeclaration> decl, std::shared_ptr<Block> body) 
  : decl(decl), body(body) {}
  void FunctionGlobject::codegen() {

    std::vector<std::string> variables_to_clean_up;

    //TODO: maybe add a FunctionDeclarationg get_args instead of having it
    // have function globject as a friend 
    for(std::shared_ptr<DeclarationNode> arg : decl->args->get_args()) {
      variables_to_clean_up.push_back(arg->get_varname());
    }
    llvm::BasicBlock *EntryBlock = llvm::BasicBlock::Create(*CompilerContext::TheContext, decl->name, decl->get_func());
    body->codegen();

    //TODO: Probably in reverse in the future in case of type checks? Doesn't really matter though on a second thought. Stil, better to keep this in mind
    for(std::string varname : variables_to_clean_up) {
      CompilerContext::NamedValues->remove_val(varname);
    }

  }
  std::string FunctionGlobject::get_name() {
    return "Function Globject";
  }
  std::vector<std::shared_ptr<Node>> FunctionGlobject::get_children() {
    return {decl, body};
  }
}

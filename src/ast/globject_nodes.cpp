#include "ast/globject_nodes.hpp"
#include "ast/globals.hpp"

#include "libgen/print.hpp"

namespace ast {

  std::string GlobjectNode::get_name() {
    return "Globject Node";
  }

  void ProgramNode::codegen() {

    libgen::register_print_i32_decl();
    std::ranges::for_each(globjects, &GlobjectNode::codegen);

  }
  std::string ProgramNode::get_name() {
    return "Program Node";
  }
  void ProgramNode::add_obj(std::shared_ptr<GlobjectNode> globject) {
    globjects.push_back(globject);
  }
  ProgramNode::ProgramNode() {}
  std::vector<std::shared_ptr<Node>> ProgramNode::get_children() {
    return {globjects.begin(), globjects.end()};
  }

  std::string FunctionArgs::get_name() {
    return "Function Args";
  }
  std::vector<std::shared_ptr<Node>> FunctionArgs::get_children() {
    return {args.begin(), args.end()};
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
    std::ranges::transform(
      get_args(),
      std::back_inserter(types),
      &DeclarationNode::get_var_type);
    return types;
  }

  FunctionDeclaration::FunctionDeclaration(
    std::string var_name,
    std::shared_ptr<FunctionArglist> args,
    llvm::Type* return_type)
  : var_name(var_name), args(args), return_type(return_type) {}
  void FunctionDeclaration::codegen() {
    if(CompilerContext::Functions->contains(var_name)) {
      return;
    }
    std::vector<llvm::Type*> arg_types = args->get_arg_types();

    llvm::FunctionType* func_type =
      llvm::FunctionType::get(return_type, arg_types, false);

    llvm::Function* this_function =
      llvm::Function::Create(
        func_type,
        llvm::Function::ExternalLinkage,
        var_name,
        CompilerContext::TheModule.get());

    (*CompilerContext::Functions)[var_name] = this_function;
  }
  llvm::Function* FunctionDeclaration::get_func() {
    if(!CompilerContext::Functions->contains(var_name))
      codegen();
    return CompilerContext::Functions->at(var_name);
  }
  std::string FunctionDeclaration::get_name() {
    return "Function Declaration";
  }
  std::vector<std::shared_ptr<Node>> FunctionDeclaration::get_children() {
    return {args};
  }
  std::vector<std::shared_ptr<DeclarationNode>> 
  FunctionDeclaration::get_args() {
    return args->get_args();
  }
  std::string FunctionDeclaration::get_varname() {
    return var_name;
  }

  FunctionDeclarationGlobject::FunctionDeclarationGlobject(
    std::shared_ptr<FunctionDeclaration> decl)
  : decl(decl) {}
  void FunctionDeclarationGlobject::codegen() {
    decl->codegen();
  }
  std::string FunctionDeclarationGlobject::get_name() {
    return "Function Declaration Globject";
  }
  std::vector<std::shared_ptr<Node>> 
  FunctionDeclarationGlobject::get_children() {
    return {decl};
  }

  void FunctionGlobject::ensure_return(llvm::Function* func) {
    llvm::BasicBlock *last_block = &func->back();
    if(last_block->getTerminator()) return;

    CompilerContext::Builder->SetInsertPoint(last_block);
    llvm::Type *return_type = func->getReturnType();
    if(return_type->isVoidTy()) {
      CompilerContext::Builder->CreateRetVoid();
    }
    else {
      llvm::Value *undef_val = llvm::UndefValue::get(return_type);
      CompilerContext::Builder->CreateRet(undef_val);
    }
  }
  FunctionGlobject::FunctionGlobject(
    std::shared_ptr<FunctionDeclaration> decl,
    std::shared_ptr<Block> body) 
  : decl(decl), body(body) {}
  void FunctionGlobject::codegen() {

    std::vector<std::string> variables_to_clean_up;

    for(std::shared_ptr<DeclarationNode> arg : decl->get_args()) {
      variables_to_clean_up.push_back(arg->get_varname());
    }

    llvm::Function* f = decl->get_func();

    int i = 0;
    for(auto& arg : f->args()) {
      CompilerContext::NamedValues->add_val(variables_to_clean_up[i++], &arg);
    }

    llvm::BasicBlock *EntryBlock = llvm::BasicBlock::Create(
      *CompilerContext::TheContext,
      decl->get_varname(),
      f);
    CompilerContext::Builder->SetInsertPoint(EntryBlock);
    body->codegen();
    ensure_return(f);

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

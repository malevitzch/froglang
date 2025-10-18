#include "ast/visitors/tree_compiler.hpp"
#include <stdexcept>
#include <format>

#include "ast/globals.hpp"
#include "libgen/print.hpp"

namespace ast {

  void TreeCompiler::ensure_return(llvm::Function* func) {
    // FIXME: implement
    llvm::BasicBlock *last_block = &func->back();
    if(last_block->getTerminator()) return;

    Builder.SetInsertPoint(last_block);
    llvm::Type *return_type = func->getReturnType();
    if(return_type->isVoidTy()) {
      Builder.CreateRetVoid();
    }
    else {
      llvm::Value *undef_val = llvm::UndefValue::get(return_type);
      Builder.CreateRet(undef_val);
    }
  }

  TreeCompiler::TreeCompiler() :
    TheModule("ProgramModule", TheContext),
    Builder(TheContext),
    Types(CompilerContext::get_primitive_types()) {
    // FIXME: implement
  }
  void TreeCompiler::default_visit(Node& node) {
    throw std::runtime_error(
      std::format("the tree compiler does not currently support the {} node\n",
                  node.get_name()));
  }
  void TreeCompiler::visit_program_node(ProgramNode& program_node) {
    // TODO: this should be packed inside some function
    // and also probably shouldn't be done like this
    libgen::register_print_i32_decl();

    std::ranges::for_each(program_node.get_globjects(), &GlobjectNode::codegen);
  }
  void TreeCompiler::visit_function_declaration_node(FunctionDeclaration& node) {
    if(Functions.contains(node.get_function_name())) {
      throw std::runtime_error(std::format("Redeclaration of function {}", node.get_function_name()));
    }
    std::vector<llvm::Type*> arg_types = node.args->get_arg_types();

    llvm::FunctionType* func_type =
      llvm::FunctionType::get(node.return_type, arg_types, false);

        llvm::Function* this_function =
      llvm::Function::Create(
        func_type,
        llvm::Function::ExternalLinkage,
        node.get_function_name(),
        TheModule);

    Functions[node.get_function_name()] = this_function;
  }
  void TreeCompiler::visit_function_node(FunctionGlobject& node) {
    FunctionDeclaration& decl = *node.get_decl();

    // TODO: investigate whether somewhere we actually check the vars not being
    // duplicate cause that's a bit annoying
    std::vector<std::string> vars;
    for(auto arg : decl.get_args()) {
      vars.push_back(arg->get_varname());
    }
    llvm::Function* f = decl.get_func();
    int i = 0;
    for(auto& arg : f->args()) {
      NamedValues.add_val(vars[i++], &arg);
    }
    llvm::BasicBlock *EntryBlock = llvm::BasicBlock::Create(
      TheContext,
      node.get_decl()->get_function_name(),
      f);
    Builder.SetInsertPoint(EntryBlock);
    visit_node(node.get_body());
    ensure_return(f);

    for(std::string varname : vars) {
      NamedValues.remove_val(varname);
    }
  }

  void TreeCompiler::visit_declaration_node(DeclarationNode& node) {
    NamedValues.add_val(node.get_varname());
  }

  void TreeCompiler::visit_declaration_statement(DeclarationStatement& node) {
    NamedValues.add_val(node.get_varname());
  }

  void TreeCompiler::visit_declaration_assignment_statement(
    DeclarationAssignmentStatement& statement) {
    //FIXME: eval_expr?
    //NamedValues.add_val(statement.get_decl(), statement.get_expr().eval());
  }
}

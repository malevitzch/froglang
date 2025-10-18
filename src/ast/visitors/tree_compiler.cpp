#include "ast/visitors/tree_compiler.hpp"
#include <stdexcept>
#include <format>

#include "ast/globals.hpp"
#include "libgen/print.hpp"

namespace ast {
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
    if(CompilerContext::Functions->contains(node.get_function_name())) {
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
}

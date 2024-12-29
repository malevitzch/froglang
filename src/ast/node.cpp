#include "ast/node.hpp"

namespace CompilerContext {
  std::unique_ptr<llvm::LLVMContext> TheContext = std::make_unique<llvm::LLVMContext>();
  std::unique_ptr<llvm::IRBuilder<>> Builder;
  std::unique_ptr<llvm::Module> TheModule = std::make_unique<llvm::Module>("Program", *TheContext);
}

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

#pragma once
#include <llvm/IR/Value.h>
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"

namespace CompilerContext {
  extern std::unique_ptr<llvm::LLVMContext> TheContext;
  extern std::unique_ptr<llvm::IRBuilder<>> Builder;
  extern std::unique_ptr<llvm::Module> TheModule;
}

namespace ast {
  // Abstract class
  class Node {
  private:
  protected:
    Node() = default;
    virtual ~Node() = default;
  public:
    virtual void codegen();
    virtual std::string get_name();
    virtual std::vector<std::shared_ptr<Node>> get_children();
  };
}

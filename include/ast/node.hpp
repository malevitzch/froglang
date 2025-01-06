#pragma once
#include <map>
#include <llvm/IR/Value.h>
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"

namespace CompilerContext {
  extern std::unique_ptr<llvm::LLVMContext> TheContext;
  extern std::unique_ptr<llvm::IRBuilder<>> Builder;
  extern std::unique_ptr<llvm::Module> TheModule;
  extern std::map<std::string, llvm::Value* > NamedValues;
}

namespace ast {
  // Abstract class
  class Node {
  private:
  protected:
    Node() = default;
    virtual ~Node() = default;
  public:
    bool final = false;
    virtual void codegen();
    virtual std::string get_name();
    virtual std::vector<std::shared_ptr<Node>> get_children();
  };
}

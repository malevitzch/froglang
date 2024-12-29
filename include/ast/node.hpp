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
  public:
    virtual llvm::Value* codegen() = 0;
    virtual std::string get_name();
  };

  class GlobjectNode : public Node {
  private:
  public:
    llvm::Value* codegen() override;
    std::string get_name() override;
    GlobjectNode();
  };

  class ProgramNode : public Node {
  private:
    std::vector<Node*> globjects;
  public:
    llvm::Value* codegen() override;
    std::string get_name() override;
    void add_obj(GlobjectNode* globject);
    ProgramNode();
  };
}

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
    ~Node() = default;
  public:
    virtual llvm::Value* codegen() = 0;
    virtual std::string get_name();

    virtual std::vector<std::shared_ptr<Node>> get_children();
  };

  class DeclarationNode : public Node {
  private:
    std::string var_type;
    std::string var_name;
  public:
    DeclarationNode(std::string var_type, std::string var_name);
    std::string get_name() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override;
  friend class DeclarationStatement;

  };

  class FunctionDeclaration : public Node {
  private:
    //TODO: replace with type representation system
    std::string return_type;
    //FIXME: ARGS
  public:
    FunctionDeclaration();
    llvm::Value* codegen() override;
  };

}

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

  // Maybe should be an abstract class
  class GlobjectNode : public Node {
  private:
  protected:
    GlobjectNode() = default;
  public:
    llvm::Value* codegen() override;
    std::string get_name() override;
  };

  class ProgramNode : public Node {
  private:
    std::vector<std::shared_ptr<GlobjectNode>> globjects;
  public:
    ProgramNode();

    llvm::Value* codegen() override;
    std::string get_name() override;
    void add_obj(std::shared_ptr<GlobjectNode> globject);

    std::vector<std::shared_ptr<Node>> get_children() override;
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

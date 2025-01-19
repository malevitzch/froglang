#pragma once
#include "node.hpp"
#include "statement_nodes.hpp"

namespace ast {

  class GlobjectNode : public Node {
  private:
  protected:
    GlobjectNode() = default;
  public:
    virtual void codegen() = 0;
    std::string get_name() override;
  };

  class ProgramNode : public Node {
  private:
    std::vector<std::shared_ptr<GlobjectNode>> globjects;
  public:
    ProgramNode();
    virtual std::string get_name() override;
    virtual void codegen();
    void add_obj(std::shared_ptr<GlobjectNode> globject);

    std::vector<std::shared_ptr<Node>> get_children() override;
  };

  class FunctionArgs : public Node {
  private:
    std::vector<std::shared_ptr<DeclarationNode>> args;
  public:
    FunctionArgs() = default;
    virtual ~FunctionArgs() = default;
    virtual std::string get_name() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override;

    void add_arg(std::shared_ptr<DeclarationNode> arg);
    std::vector<std::shared_ptr<DeclarationNode>> get_args();
  };
  
  class FunctionArglist : public Node {
  private:
    std::shared_ptr<FunctionArgs> args;
  public:
    FunctionArglist() = default;
    FunctionArglist(std::shared_ptr<FunctionArgs> args);
    virtual ~FunctionArglist() = default;
    virtual std::string get_name() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override;

    std::vector<std::shared_ptr<DeclarationNode>> get_args();
    std::vector<llvm::Type*> get_arg_types();
  };

  class FunctionDeclaration : public GlobjectNode {
  private:
    std::string var_name;
    std::shared_ptr<FunctionArglist> args;
    llvm::Type* return_type;
  public:
    FunctionDeclaration(std::string var_name, std::shared_ptr<FunctionArglist> args, llvm::Type* return_type);
    virtual void codegen() override;
    virtual std::string get_name() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override;

    std::vector<std::shared_ptr<DeclarationNode>> get_args();
    llvm::Function* get_func();
    std::string get_varname();
  };

  class FunctionGlobject : public GlobjectNode {
  private:
    std::shared_ptr<FunctionDeclaration> decl;
    std::shared_ptr<Block> body;
  protected:
  public:
    FunctionGlobject(std::shared_ptr<FunctionDeclaration> decl, std::shared_ptr<Block> body);
    virtual void codegen() override;
    virtual std::string get_name() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override;
  };
}

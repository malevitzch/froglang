#ifndef AST_GLOBJECT_NODES_HPP
#define AST_GLOBJECT_NODES_HPP

#include <optional>

#include "node.hpp"
#include "statement_nodes.hpp"

namespace ast {

  class TreePrinter;

  class GlobjectNode : public Node {
  private:
  protected:
    GlobjectNode() = default;
  public:
    virtual std::optional<std::string> codegen() = 0;
    std::string get_name() override;
  };

  class ProgramNode : public Node {
  private:
    std::vector<std::shared_ptr<GlobjectNode>> globjects;
  public:
    ProgramNode();
    virtual void accept_visitor(TreeVisitor& visitor) override;
    virtual std::string get_name() override;
    virtual std::optional<std::string> codegen();
    void add_obj(std::shared_ptr<GlobjectNode> globject);

    std::vector<std::shared_ptr<Node>> get_children() override;
  };

  class FunctionArglist : public Node {
  private:
    std::vector<std::shared_ptr<DeclarationNode>> args;
  public:
    FunctionArglist() = default;
    virtual ~FunctionArglist() = default;
    virtual std::string get_name() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override;

    void add_arg(std::shared_ptr<DeclarationNode> arg);
    std::vector<std::shared_ptr<DeclarationNode>> get_args();
    std::vector<llvm::Type*> get_arg_types();
  };

  class FunctionDeclaration : public GlobjectNode {
  private:
    std::string function_name;
    std::shared_ptr<FunctionArglist> args;
    llvm::Type* return_type;
  public:
    FunctionDeclaration(
      std::string var_name,
      std::shared_ptr<FunctionArglist> args,
      llvm::Type* return_type);
    virtual void accept_visitor(TreeVisitor& visitor) override;
    virtual std::optional<std::string> codegen() override;
    virtual std::string get_name() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override;

    std::vector<std::shared_ptr<DeclarationNode>> get_args();
    llvm::Function* get_func();
    std::string get_function_name();
  };

  class FunctionDeclarationGlobject : public GlobjectNode {
  private:
    std::shared_ptr<FunctionDeclaration> decl;
  public:
    FunctionDeclarationGlobject(std::shared_ptr<FunctionDeclaration> decl);
    virtual void accept_visitor(TreeVisitor& visitor) override;
    virtual std::optional<std::string> codegen() override;
    virtual std::string get_name() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override;

    std::shared_ptr<FunctionDeclaration> get_decl();
  };

  class FunctionGlobject : public GlobjectNode {
  private:
    std::shared_ptr<FunctionDeclaration> decl;
    std::shared_ptr<Block> body;
    void ensure_return(llvm::Function* func);
  protected:
  public:
    FunctionGlobject(
      std::shared_ptr<FunctionDeclaration> decl,
      std::shared_ptr<Block> body);
    virtual void accept_visitor(TreeVisitor& visitor) override;
    virtual std::optional<std::string> codegen() override;
    virtual std::string get_name() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override;

    std::shared_ptr<FunctionDeclaration> get_decl();
    std::shared_ptr<Block> get_body();
  };
}

#endif

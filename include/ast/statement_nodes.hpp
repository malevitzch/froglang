#pragma once
#include "node.hpp"
#include "expression_nodes.hpp"

namespace ast {

  // Abstract class
  class StatementNode : public Node {
  private:
  protected:
    StatementNode() = default;
    ~StatementNode() = default;
  public:
    virtual std::string get_name() override;
  };

  // This inherits from Node directly because a bundle of statements is not a statement.
  class Statements : public Node {
  private:
    std::vector<std::shared_ptr<Node>> statements;
  public:
    virtual std::string get_name() override;
    virtual llvm::Value* codegen() override;
    void add_statement(std::shared_ptr<StatementNode> statement);
    virtual std::vector<std::shared_ptr<Node>> get_children() override;
  };

  class Block : public StatementNode {
  private:
    std::shared_ptr<StatementNode> statements;
  public:
    virtual llvm::Value* codegen() override;

  };

  class ExpressionStatement : public StatementNode {
  private:
    std::shared_ptr<ExprNode> expr;
  public:
  };

  //FIXME: this needs a declaration node to even be implemented
  class DeclarationStatement : public StatementNode {
  private:
    std::shared_ptr<DeclarationNode> decl;
  public:
  };

}

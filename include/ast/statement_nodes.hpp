#pragma once
#include "node.hpp"
#include "expression_nodes.hpp"

namespace ast {

  // Abstract class
  class StatementNode : public Node {
  private:
  protected:
    StatementNode() = default;
    virtual ~StatementNode() = default;
  public:
    virtual std::string get_name() override;
  };

  // This inherits from Node directly because a bundle of statements is not a statement.
  class Statements : public Node {
  private:
    std::vector<std::shared_ptr<Node>> statements;
  public:
    virtual std::string get_name() override;
    virtual void codegen() override;
    void add_statement(std::shared_ptr<StatementNode> statement);
    virtual std::vector<std::shared_ptr<Node>> get_children() override;
  };

  class Block : public StatementNode {
  private:
    std::shared_ptr<Statements> statements;
  public:
    Block() = default;
    virtual ~Block() = default;
    virtual void codegen() override;

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

  class ExpressionStatement : public StatementNode {
  private:
    std::shared_ptr<ExprNode> expr;
  public:
  };

  class DeclarationStatement : public StatementNode {
  private:
    std::shared_ptr<DeclarationNode> decl;
  public:
  };

}

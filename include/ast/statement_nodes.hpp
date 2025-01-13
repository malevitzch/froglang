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
    virtual void codegen() = 0;
  public:
    virtual std::string get_name() override;
  };

  // This inherits from Node directly because a bundle of statements is not a statement.
  class Statements : public Node {
  private:
    std::vector<std::shared_ptr<StatementNode>> statements;
  public:
    Statements() = default;
    virtual ~Statements() = default;
    const std::vector<std::shared_ptr<StatementNode>>& get();
    virtual std::string get_name() override;
    void add_statement(std::shared_ptr<StatementNode> statement);
    virtual std::vector<std::shared_ptr<Node>> get_children() override;
  };

  class Block : public StatementNode {
  private:
    std::shared_ptr<Statements> statements;
  public:
    Block(std::shared_ptr<Statements> statements);
    virtual ~Block() = default;
    virtual void codegen() override;
    virtual std::string get_name() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override;

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
    ExpressionStatement(std::shared_ptr<ExprNode> expr);
    virtual void codegen() override;
    virtual std::string get_name() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override; 
  };

  class DeclarationStatement : public StatementNode {
  private:
    std::shared_ptr<DeclarationNode> decl;
  public:
    DeclarationStatement(std::shared_ptr<DeclarationNode> decl);
    virtual void codegen() override;
    virtual std::string get_name() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override;
  };

  class DeclarationAssignmentStatement : public StatementNode {
  private:
    std::shared_ptr<DeclarationNode> decl;
    std::shared_ptr<ExprNode> expr;
  public:
    DeclarationAssignmentStatement(std::shared_ptr<DeclarationNode> decl, std::shared_ptr<ExprNode> expr);
    ~DeclarationAssignmentStatement() = default;
    virtual void codegen() override;
    virtual std::string get_name() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override;
  };

  class ReturnStatement : public StatementNode {
  private:
    std::string type;
    std::shared_ptr<ExprNode> val;
  public:
    ReturnStatement(); // For void returns
    ReturnStatement(std::string type, std::shared_ptr<ExprNode> val); // For everything else
    virtual ~ReturnStatement() = default;
    virtual void codegen() override;
    virtual std::string get_name() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override;

  };

}

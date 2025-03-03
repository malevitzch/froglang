#ifndef AST_STATEMENT_NODES_HPP
#define AST_STATEMENT_NODES_HPP

#include "node.hpp"
#include "expression_nodes.hpp"

#include "llvm/IR/Type.h"

namespace ast {

  // Abstract class
  class StatementNode : public Node {
  private:
  protected:
    StatementNode() = default;
    virtual ~StatementNode() = default;
  public:
    virtual void codegen() = 0;
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
    llvm::Type* var_type;
    std::string var_name;
  public:
    DeclarationNode(llvm::Type* var_type, std::string var_name);
    virtual void codegen();
    std::string get_name() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override;

    std::string get_varname();
    llvm::Type* get_var_type();

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

    std::string get_varname();
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

    std::string get_varname();
  };

  class ReturnStatement : public StatementNode {
  private:
    std::shared_ptr<ExprNode> val;
  public:
    ReturnStatement(); // For void returns
    ReturnStatement(std::shared_ptr<ExprNode> val); // For everything else
    virtual ~ReturnStatement() = default;
    virtual void codegen() override;
    virtual std::string get_name() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override;
  };

  class IfStatement : public StatementNode {
  private:
    std::shared_ptr<ExprNode> condition;
    std::shared_ptr<StatementNode> if_body;
    std::shared_ptr<StatementNode> else_body;
  public:
    IfStatement(std::shared_ptr<ExprNode> condition, std::shared_ptr<StatementNode> if_body);
    IfStatement(std::shared_ptr<ExprNode> condition, std::shared_ptr<StatementNode> if_body, std::shared_ptr<StatementNode> else_body);
    virtual ~IfStatement() = default;
    virtual void codegen() override;
    virtual std::string get_name() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override;
  };

  class WhileLoop : public StatementNode {
  private:
    std::shared_ptr<ExprNode> condition;
    std::shared_ptr<StatementNode> body;
  public:
    WhileLoop(std::shared_ptr<ExprNode> condition, std::shared_ptr<StatementNode> body);
    virtual ~WhileLoop() = default;
    virtual void codegen() override;
    virtual std::string get_name() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override;
  };

}

#endif

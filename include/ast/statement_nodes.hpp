#ifndef AST_STATEMENT_NODES_HPP
#define AST_STATEMENT_NODES_HPP

#include <optional>

#include "node.hpp"
#include "expression_nodes.hpp"

#include "llvm/IR/Type.h"

namespace ast {

  class TreeVisitor;

  // Abstract class
  class StatementNode : public Node {
  private:
  protected:
    StatementNode() = default;
    virtual ~StatementNode() = default;
  public:
    virtual std::optional<std::string> codegen() = 0;
    virtual std::string get_name() override;
  };

  class Statements : public Node {
  private:
    std::vector<std::shared_ptr<StatementNode>> statements;
  public:
    Statements() = default;
    virtual ~Statements() = default;
    const std::vector<std::shared_ptr<StatementNode>>& get();
    virtual std::string get_name() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override;

    void add_statement(std::shared_ptr<StatementNode> statement);
  };

  class Block : public StatementNode {
  private:
    std::shared_ptr<Statements> statements;
  public:
    Block(std::shared_ptr<Statements> statements);
    virtual ~Block() = default;
    virtual void accept_visitor(TreeVisitor& visitor) override;
    virtual std::optional<std::string> codegen() override;
    virtual std::string get_name() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override;

    std::shared_ptr<Statements> get_statements();

  };

  class DeclarationNode : public Node {
  private:
    llvm::Type* var_type;
    std::string var_name;
  public:
    DeclarationNode(llvm::Type* var_type, std::string var_name);
    virtual void accept_visitor(TreeVisitor& visitor) override;
    virtual std::optional<std::string> codegen();
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
    virtual void accept_visitor(TreeVisitor& visitor) override;
    virtual std::optional<std::string> codegen() override;
    virtual std::string get_name() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override;

    std::shared_ptr<ExprNode> get_expr();
  };

  class DeclarationStatement : public StatementNode {
  private:
  protected:
    std::shared_ptr<DeclarationNode> decl;
  public:
    DeclarationStatement(std::shared_ptr<DeclarationNode> decl);
    virtual std::optional<std::string> codegen() override;
    virtual std::string get_name() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override;

    std::string get_varname();
  };

  class DeclarationAssignmentStatement : public DeclarationStatement {
  private:
  protected:
    std::shared_ptr<ExprNode> expr;
  public:
    DeclarationAssignmentStatement(
      std::shared_ptr<DeclarationNode> decl, std::shared_ptr<ExprNode> expr);
    ~DeclarationAssignmentStatement() = default;
    virtual std::optional<std::string> codegen() override;
    virtual std::string get_name() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override;

    std::string get_varname();
  };

  class ReturnStatement : public StatementNode {
  private:
    std::shared_ptr<ExprNode> val = nullptr;
  public:
    // Constructor for void returns
    ReturnStatement(); 
    // Constructor for everything else
    ReturnStatement(std::shared_ptr<ExprNode> val); 
    virtual ~ReturnStatement() = default;
    virtual void accept_visitor(TreeVisitor& visitor) override;
    virtual std::optional<std::string> codegen() override;
    virtual std::string get_name() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override;

    bool is_void();
    std::shared_ptr<ExprNode> get_return_val();
  };

  class IfStatement : public StatementNode {
  private:
    std::shared_ptr<ExprNode> condition;
    std::shared_ptr<StatementNode> if_body;
    std::shared_ptr<StatementNode> else_body = nullptr;
  public:
    IfStatement(
      std::shared_ptr<ExprNode> condition,
      std::shared_ptr<StatementNode> if_body);
    IfStatement(
      std::shared_ptr<ExprNode> condition,
      std::shared_ptr<StatementNode> if_body,
      std::shared_ptr<StatementNode> else_body);
    virtual ~IfStatement() = default;
    virtual void accept_visitor(TreeVisitor& visitor) override;
    virtual std::optional<std::string> codegen() override;
    virtual std::string get_name() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override;

    friend class TreePrinter;
  };

  class WhileLoop : public StatementNode {
  private:
    std::shared_ptr<ExprNode> condition;
    std::shared_ptr<StatementNode> body;
  public:
    WhileLoop(
      std::shared_ptr<ExprNode> condition,
      std::shared_ptr<StatementNode> body);
    virtual ~WhileLoop() = default;
    virtual std::optional<std::string> codegen() override;
    virtual std::string get_name() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override;
  };

}

#endif

#ifndef AST_EXPRESSION_NODES_HPP
#define AST_EXPRESSION_NODES_HPP

#include "ast/node.hpp"

namespace ast {

  // Abstract class
  class ExprNode : public Node {
  private:
  protected:
    ExprNode() = default;
    virtual ~ExprNode() = default;
  public:
    // The main function for all expression nodes
    // Evaluates the expression to an llvm::Value*
    virtual llvm::Value* eval() = 0;
    virtual std::string get_name() override;
  };

  class IversonExpr : public ExprNode {
  private:
    std::shared_ptr<ExprNode> expr;
  public:
    IversonExpr(std::shared_ptr<ExprNode> operand);
    virtual void accept_visitor(TreeVisitor& visitor) override;
    virtual llvm::Value* eval() override;
    virtual std::string get_name() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override;

    std::shared_ptr<ExprNode> get_expr();
  };

  class UnaryOperator : public ExprNode {
  private:
    std::string operator_type;
  protected:
    std::shared_ptr<ExprNode> operand;
  public:
    static std::shared_ptr<UnaryOperator> create(
      std::string operator_type,
      std::shared_ptr<ExprNode> operand);
    UnaryOperator(
      std::string operator_type,
      std::shared_ptr<ExprNode> operand);
    virtual void accept_visitor(TreeVisitor& visitor) override;
    virtual std::string get_name() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override;

    std::shared_ptr<ExprNode> get_operand();
    std::string get_operator_type();
  };

  // Abstract class, inherits the pure virtual method from ExprNode
  class BinaryOperator : public ExprNode {
  private:
    std::string operator_type;
  // The constructor is protected because the objects are created using
  // the create method which returns a binary operator of the proper class
  protected:
    BinaryOperator(
      std::string operator_type,
      std::shared_ptr<ExprNode> left,
      std::shared_ptr<ExprNode> right);
    std::shared_ptr<ExprNode> left, right;
  public:
    static std::shared_ptr<BinaryOperator> create(
      std::string operator_type,
      std::shared_ptr<ExprNode> left,
      std::shared_ptr<ExprNode> right);
    virtual void accept_visitor(TreeVisitor& visitor) override;
    virtual std::string get_name() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override;

    std::string get_operator_type();

    std::shared_ptr<ExprNode> get_LHS();
    std::shared_ptr<ExprNode> get_RHS();
  };

  //TODO: some of the constant functionality should probably be handled 
  // by this class, probably stuff like getType()
  class Constant : public ExprNode {
  private:
  public:
  };

  class IntegerConstant : public Constant {
  private:
    unsigned precision;
    std::string data;
  public:
    IntegerConstant(unsigned precision, std::string data);
    virtual void accept_visitor(TreeVisitor& visitor) override;
    virtual llvm::Value* eval() override;
    virtual ~IntegerConstant() = default;
    virtual std::string get_name() override;

    std::string get_data();
  };

  class VariableIdentifier : public ExprNode {
  private:
    std::string var_name;
  public:
    VariableIdentifier(std::string var_name);
    virtual void accept_visitor(TreeVisitor& visitor) override;
    virtual llvm::Value* eval() override;
    virtual std::string get_name() override;

    std::string get_variable_name();
  };

  class FunctionCallArglist : public Node {
  private:
    std::vector<std::shared_ptr<ExprNode>> args;
  public:
    FunctionCallArglist() = default;
    FunctionCallArglist(std::vector<std::shared_ptr<ExprNode>> args);
    virtual ~FunctionCallArglist() = default;
    virtual std::string get_name() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override;

    std::vector<std::shared_ptr<ExprNode>> get_args();
    void add_arg(std::shared_ptr<ExprNode> arg);
  };

  class FunctionCallExpr : public ExprNode {
  private:
    std::string function_name;
    std::shared_ptr<FunctionCallArglist> args;
  public:
    FunctionCallExpr(
      std::string function_name,
      std::shared_ptr<FunctionCallArglist> args);
    virtual ~FunctionCallExpr() = default;
    virtual void accept_visitor(TreeVisitor& visitor) override;
    virtual llvm::Value* eval() override;
    virtual std::string get_name() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override;

    std::string get_function_name();
  };
}

#endif

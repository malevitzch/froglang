#ifndef AST_EXPRESSION_NODES_HPP
#define AST_EXPRESSION_NODES_HPP

#include "ast/node.hpp"

namespace ast {

  class ExprNode : public Node {
  private:
  protected:
    ExprNode() = default;
    virtual ~ExprNode() = default;
    //TODO: this might not exist in the future
  public:
    virtual llvm::Value* eval() = 0;
    virtual std::string get_name() override;
  };

  class IversonExpr : public ExprNode {
  private:
    std::shared_ptr<ExprNode> expr;
  public:
    IversonExpr(std::shared_ptr<ExprNode> operand);
    virtual llvm::Value* eval() override;
    virtual std::string get_name() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override;
  };

  class UnaryOperator : public ExprNode {
  private:
    std::string operator_type;
    std::shared_ptr<ExprNode> operand;
  public:
    UnaryOperator(std::string operator_type, std::shared_ptr<ExprNode> operand);
    virtual llvm::Value* eval() override;
    virtual std::string get_name() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override;
};

  class BinaryOperator : public ExprNode {
  private:
    std::string operator_type;
    std::shared_ptr<ExprNode> left, right;
  public: 
    BinaryOperator(std::string operator_type, std::shared_ptr<ExprNode> left, std::shared_ptr<ExprNode> right);
    virtual llvm::Value* eval() override;
    virtual std::string get_name() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override;
  };

  class IntegerConstant : public ExprNode {
  private:
    unsigned precision;
    std::string data;
    // TODO: This might be replaced by an arbitrarily-large bit array
    // so that we can have arbitrary size integers later
  public:
    IntegerConstant(unsigned precision, std::string data);
    virtual llvm::Value* eval() override;
    virtual ~IntegerConstant() = default;
    virtual std::string get_name() override;
  };

  class VariableIdentifier : public ExprNode {
  private:
    std::string var_name;
  public:
    VariableIdentifier(std::string var_name);
    virtual llvm::Value* eval() override;
    virtual std::string get_name() override;
  };

  class FunctionCallArgs : public Node {
  private:
    std::vector<std::shared_ptr<ExprNode>> args;
  public:
    FunctionCallArgs() = default;
    ~FunctionCallArgs() = default;
    void add_arg(std::shared_ptr<ExprNode> arg);
    virtual std::string get_name() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override;
    std::vector<std::shared_ptr<ExprNode>> get_args();
  };

  class FunctionCallArglist : public Node {
  private:
    std::shared_ptr<FunctionCallArgs> args;
  public:
    FunctionCallArglist() = default;
    FunctionCallArglist(std::shared_ptr<FunctionCallArgs> args);
    virtual ~FunctionCallArglist() = default;
    virtual std::string get_name() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override;

    std::vector<std::shared_ptr<ExprNode>> get_args();
  };

  class FunctionCallExpr : public ExprNode {
  private:
    std::string function_name;
    std::shared_ptr<FunctionCallArglist> args;
  public:
    FunctionCallExpr(std::string function_name, std::shared_ptr<FunctionCallArglist> args);
    virtual ~FunctionCallExpr() = default;
    virtual llvm::Value* eval() override;
    virtual std::string get_name() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override;
  };

}

#endif

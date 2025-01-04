#pragma once
#include "ast/node.hpp"

namespace ast {

  class ExprNode : public Node {
  private:
  protected:
    ExprNode() = default;
    ~ExprNode() = default;
    //TODO: this might not exist in the future
    ExprNode(std::string type);
    // TODO: Type might eventually become something more complicated than a string
    std::string type;
  public:
    virtual std::string get_name() override;
    virtual std::string get_type();
  };

  class BinaryOperator : public ExprNode {
  private:
    std::string operator_type;
    std::shared_ptr<ExprNode> left, right;
  public: 
    BinaryOperator(std::string operator_type, std::shared_ptr<ExprNode> left, std::shared_ptr<ExprNode> right);
    virtual void codegen() override;
    virtual std::string get_type() override;
    virtual std::string get_name() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override;
  };

  class IntegerConstant : public ExprNode {
  private:
    long long value;
    // TODO: This might be replaced by an arbitrarily-large bit array
    // so that we can have arbitrary size integers later
  public:
    IntegerConstant(std::string data, std::string type);
    virtual void codegen() override;
    virtual std::string get_name() override;
  };

  class VariableIdentifier : public ExprNode {
  private:
    std::string var_name;
  public:
    VariableIdentifier(std::string var_name, std::string type);
    virtual void codegen() override;
    virtual std::string get_name() override;
  };

  class FunctionCallArgs : public Node {
  private:
    std::vector<std::shared_ptr<ExprNode>> args;
  public:
    FunctionCallArgs() = default;
    ~FunctionCallArgs() = default;
    void add_arg(std::shared_ptr<ExprNode> arg);
    virtual void codegen() override;
    virtual std::string get_name() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override;
  };

  class FunctionCallArglist : public Node {
  private:
    std::shared_ptr<FunctionCallArgs> args;
  public:
    FunctionCallArglist() = default;
    FunctionCallArglist(std::shared_ptr<FunctionCallArgs> args);
    virtual ~FunctionCallArglist() = default;
    virtual void codegen() override;
    virtual std::string get_name() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override;
  };

  class FunctionCallExpr : public ExprNode {
  private:
    std::string function_name;
    std::shared_ptr<FunctionCallArglist> args;
  public:
    FunctionCallExpr(std::string function_name, std::shared_ptr<FunctionCallArglist> args);
    virtual ~FunctionCallExpr() = default;
    virtual void codegen() override;
    virtual std::string get_name() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override;
  };

}
